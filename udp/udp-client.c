#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include "../cryptolibs/crypto.h"



// Constants
#define LOG_MODULE "Client"
#define LOG_LEVEL LOG_LEVEL_INFO
#define CLIENT_PORT 1357
#define SERVER_PORT 2468
#define SEND_INTERVAL 10
#define BLOCK_LENGTH 16
#define MESSAGE "123456789ABCDEFGHIJKLMNOPQRST"
#define KEY ((unsigned char*) "c0c0c0c0c0c0c0c0")



// Main process
PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);
PROCESS_THREAD(udp_client_process, ev, data) {
	PROCESS_BEGIN();

	// get ip address of the reciever
	uip_ipaddr_t reciever_ip;
	static struct etimer periodic_timer;
	while (!NETSTACK_ROUTING.node_is_reachable() || !NETSTACK_ROUTING.get_root_ipaddr(&reciever_ip)) {
		LOG_INFO("Unable to find reciever, retry in %d seconds\n", SEND_INTERVAL);
		etimer_set(&periodic_timer, SEND_INTERVAL * CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
	}
	LOG_INFO("Found receiver's ip: ");
	LOG_INFO_6ADDR(&reciever_ip);
	LOG_INFO_("\n");

	// connection
	static struct simple_udp_connection conn;
	simple_udp_register(&conn, CLIENT_PORT, &reciever_ip, SERVER_PORT, NULL);
	
	// encrypt and send message
	setKey(KEY, 128);
	char message[] = MESSAGE;
	int len = strlen(message);
	int i;
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
		simple_udp_send(&conn, block_encrypted, BLOCK_LENGTH);
	}

	PROCESS_END();
}