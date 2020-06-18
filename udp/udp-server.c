#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"



// Constants
#define LOG_MODULE "Server"
#define LOG_LEVEL LOG_LEVEL_INFO
#define CLIENT_PORT 1357
#define SERVER_PORT 2468



// Requests will arrive here
static void request_callback(struct simple_udp_connection *c,
								const uip_ipaddr_t *sender_ip,
								uint16_t sender_port,
								const uip_ipaddr_t *receiver_ip,
								uint16_t receiver_port,
								const uint8_t *data,
								uint16_t datalen) {
	LOG_INFO("Received request \"%.*s\" from ", datalen, (char *) data);
	LOG_INFO_6ADDR(sender_ip);
	LOG_INFO_("\n");
	//TODO: decrypt message
}



// Main process
PROCESS(udp_server_process, "UDP server");
AUTOSTART_PROCESSES(&udp_server_process);
PROCESS_THREAD(udp_server_process, ev, data)
{
	PROCESS_BEGIN();
	NETSTACK_ROUTING.root_start();
	static struct simple_udp_connection conn;
	simple_udp_register(&conn, SERVER_PORT, NULL, CLIENT_PORT, request_callback);
	PROCESS_END();
}