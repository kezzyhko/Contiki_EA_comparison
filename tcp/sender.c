#include "contiki.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "../cryptolibs/crypto.h"
#include "statistics.h"
#include "constants.h"
#include <stdlib.h>

#include "sys/log.h"
#define LOG_MODULE "Sender"
#define LOG_LEVEL LOG_LEVEL_INFO



// Global variables
static uint8_t buffer[PACKET_SIZE];
uip_ipaddr_t reciever_ip;
int k;



// Generating random data
void generate_random_data(unsigned char* result, int size) {
	static const char charset[] = MESSAGE_CHARSET;
    int i;
    for (i = 0; i < size; i++) {
        int key = rand() % (sizeof charset - 1);
        result[i] = charset[key];
    }
}



// Data will be sent here
static PT_THREAD(handle_connection(struct psock *p)) {
	PSOCK_BEGIN(p);

	for (k = 0; k < MESSAGE_SIZE; k += PACKET_SIZE) {
		// generate and encrypt packet
		unsigned char packet_plain[PACKET_SIZE], packet_encrypted[PACKET_SIZE];
		int i;
		for (i = 0; i < PACKET_SIZE; i += BLOCK_LENGTH) {
			generate_random_data(packet_plain + i, BLOCK_LENGTH);
			encrypt(packet_plain + i, packet_encrypted + i);
		}

		// log
		LOG_INFO("Sending  \"");
		int j = 0;
		for (j = 0; j < PACKET_SIZE; j++) {
			LOG_INFO_("%02x", packet_encrypted[j]);
		}
		LOG_INFO_("\" (\"");
		for (j = 0; j < PACKET_SIZE; j++) {
			LOG_INFO_("%c", packet_plain[j]);
		}
		LOG_INFO_("\") to ");
		LOG_INFO_6ADDR(&reciever_ip);
		LOG_INFO_(". Already sent %d bytes of data\n", k + PACKET_SIZE);

		// send
		PSOCK_SEND(p, packet_encrypted, PACKET_SIZE);
	}

	PSOCK_CLOSE(p);
	PSOCK_END(p);
}



// Sender process
PROCESS(sender_process, "Sender");
AUTOSTART_PROCESSES(&sender_process);
PROCESS_THREAD(sender_process, ev, data) {
	PROCESS_BEGIN();
	static struct etimer periodic_timer;

	// prepare the key
	log_energest_statistics("Key generation started");
	setKey((unsigned char *)(KEY), (sizeof KEY - 1) * 8);
	log_energest_statistics("Key generated");

	// get ip address of the reciever
	while (!NETSTACK_ROUTING.node_is_reachable() || !NETSTACK_ROUTING.get_root_ipaddr(&reciever_ip)) {
		LOG_INFO("Unable to find reciever, retry in %d seconds\n", SEND_INTERVAL);
		etimer_set(&periodic_timer, SEND_INTERVAL * CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
	}
	LOG_INFO("Found receiver's ip: ");
	LOG_INFO_6ADDR(&reciever_ip);
	LOG_INFO_("\n");

	// connection
	do {
		tcp_connect(&reciever_ip, UIP_HTONS(PORT), NULL);
		LOG_INFO("Connecting...\n");
		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
		if (!uip_connected()) {
			LOG_INFO("Could not establish connection, retry in %d seconds\n", SEND_INTERVAL);
			printf("Could not establish connection (ABORTED:%d TIMEOUT:%d CLOSED:%d)\n", uip_aborted(), uip_timedout(), uip_closed());
			etimer_set(&periodic_timer, SEND_INTERVAL * CLOCK_SECOND);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
		}
	} while (!uip_connected());
	log_energest_statistics("Connected");

	// protothread for sending data
	static struct psock ps;
	PSOCK_INIT(&ps, buffer, sizeof(buffer));
	do {
		handle_connection(&ps);
		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
	} while (!uip_closed() && !uip_aborted() && !uip_timedout());

	log_energest_statistics("Connection closed");
	PROCESS_END();
}