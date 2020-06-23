#include "contiki.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include "../cryptolibs/aes.h"
#include "../cryptolibs/aes.c"
#include "../cryptolibs/aes/rijndael.c"



// Constants
#define LOG_MODULE "Sender"
#define LOG_LEVEL LOG_LEVEL_INFO
#define PORT 1357
#define SEND_INTERVAL 10
#define MESSAGE "123456789ABCDEFGHIJKLMNOPQR"
#define KEY ((unsigned char*) "c0c0c0c0c0c0c0c0")
#define BLOCK_LENGTH 16



// Global variables
static uint8_t buffer[BLOCK_LENGTH];
uip_ipaddr_t reciever_ip;
int i;
char message[] = MESSAGE;
int len = sizeof(MESSAGE)/sizeof(char);



// Send will be sent here
static PT_THREAD(handle_connection(struct psock *p)) {
	PSOCK_BEGIN(p);

	// TODO: add encryption
	setKey(KEY, 128);
	for (i = 0; i < len; i += BLOCK_LENGTH) {
		// encrypt
		unsigned char block_plain[BLOCK_LENGTH+1], block_encrypted[BLOCK_LENGTH];
		memcpy(block_plain, message + i, BLOCK_LENGTH);
		block_plain[BLOCK_LENGTH] = 0;
		encrypt(block_plain, block_encrypted);

		// log
		LOG_INFO("Sending \"");
		int j = 0;
		for (j = 0; j < BLOCK_LENGTH; j++) {
			LOG_INFO_("%x", block_encrypted[j]);
		}
		LOG_INFO_("\" (\"%s\") to ", block_plain);
		LOG_INFO_6ADDR(&reciever_ip);
		LOG_INFO_("\n");

		//send
		PSOCK_SEND(p, block_encrypted, BLOCK_LENGTH);
	}

	PSOCK_CLOSE(p);
	PSOCK_END(p);
}



// Main process
PROCESS(example_psock_client_process, "Example protosocket client");
AUTOSTART_PROCESSES(&example_psock_client_process);
PROCESS_THREAD(example_psock_client_process, ev, data) {
	PROCESS_BEGIN();
	static struct etimer periodic_timer;

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
	LOG_INFO("Connected\n");

	// protothread for sending data
	static struct psock ps;
	PSOCK_INIT(&ps, buffer, sizeof(buffer));
	do {
		handle_connection(&ps);
		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
	} while (!uip_closed() && !uip_aborted() && !uip_timedout());

	LOG_INFO("Connection closed\n");
	PROCESS_END();
}