#include "contiki.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"



// Constants
#define LOG_MODULE "Sender"
#define LOG_LEVEL LOG_LEVEL_INFO
#define PORT 1357
#define SEND_INTERVAL 10
#define MESSAGE "123456789ABCDEFGHIJKLMNOPQRST"
#define KEY ((unsigned char*) "c0c0c0c0c0c0c0c0")
#define BLOCK_LENGTH 16



// Send will be sent here
static uint8_t buffer[BLOCK_LENGTH];
static PT_THREAD(handle_connection(struct psock *p)) {
	PSOCK_BEGIN(p);

	// TODO: add encryption
	PSOCK_SEND_STR(p, "Data1!\n");
	PSOCK_SEND_STR(p, "Data2!\n");
	PSOCK_SEND_STR(p, "Data3!\n");

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

	// sending data
	static struct psock ps;
	PSOCK_INIT(&ps, buffer, sizeof(buffer));
	do {
		handle_connection(&ps);
		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
	} while (!uip_closed() && !uip_aborted() && !uip_timedout());

	LOG_INFO("Connection closed\n");
	PROCESS_END();
}