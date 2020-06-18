#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"



// Constants
#define LOG_MODULE "Client"
#define LOG_LEVEL LOG_LEVEL_INFO
#define CLIENT_PORT 1357
#define SERVER_PORT 2468
#define SEND_INTERVAL 10
#define MESSAGE "Hello World"




// Main process
PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);
PROCESS_THREAD(udp_client_process, ev, data)
{
	PROCESS_BEGIN();

	// get ip address of the reciever
	static struct etimer periodic_timer;
	uip_ipaddr_t reciever_ip;
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
	
	// sending message
	LOG_INFO("Sending \"%s\" to ", MESSAGE);
	LOG_INFO_6ADDR(&reciever_ip);
	LOG_INFO_("\n");
	static char str[1024]; //TODO: variable length
	snprintf(str, sizeof(str), MESSAGE); //TODO: encrypt message
	simple_udp_send(&conn, str, strlen(str));

	PROCESS_END();
}