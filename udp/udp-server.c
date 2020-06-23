#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include "../cryptolibs/aes.h"
#include "../cryptolibs/aes.c"
#include "../cryptolibs/aes/rijndael.c"



// Constants
#define LOG_MODULE "Server"
#define LOG_LEVEL LOG_LEVEL_INFO
#define CLIENT_PORT 1357
#define SERVER_PORT 2468
#define BLOCK_LENGTH 16
#define KEY ((unsigned char*) "c0c0c0c0c0c0c0c0")



// Requests will arrive here
static void request_callback(struct simple_udp_connection *c,
								const uip_ipaddr_t *sender_ip,
								uint16_t sender_port,
								const uip_ipaddr_t *receiver_ip,
								uint16_t receiver_port,
								const uint8_t *data,
								uint16_t datalen) {

	// log
	LOG_INFO("Received \"");
	int j = 0;
	for (j = 0; j < datalen; j++) {
		LOG_INFO_("%x", data[j]);
	}
	LOG_INFO_("\"");

	if (datalen == BLOCK_LENGTH) {
		// decrypt
		setKey(KEY, 128);
		unsigned char block_decrypted[BLOCK_LENGTH+1];
		decrypt(data, block_decrypted);
		block_decrypted[BLOCK_LENGTH] = 0;

		// log
		LOG_INFO_(" (\"%s\") from ", block_decrypted);
		LOG_INFO_6ADDR(sender_ip);
		LOG_INFO_("\n");
	} else {
		LOG_INFO_("\n");
		LOG_INFO("Data has the wrong length, can not decode it.\n");
	}
}



// Main process
PROCESS(udp_server_process, "UDP server");
AUTOSTART_PROCESSES(&udp_server_process);
PROCESS_THREAD(udp_server_process, ev, data) {
	PROCESS_BEGIN();
	NETSTACK_ROUTING.root_start();
	static struct simple_udp_connection conn;
	simple_udp_register(&conn, SERVER_PORT, NULL, CLIENT_PORT, request_callback);
	PROCESS_END();
}