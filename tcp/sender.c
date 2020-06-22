#include <stdio.h>
#include "contiki-net.h"
#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"



// Constants
#define LOG_MODULE "Sender"
#define LOG_LEVEL LOG_LEVEL_INFO
#define SENDER_PORT 1357
#define RECIEVER_PORT 2468
#define SEND_INTERVAL 10
#define BLOCK_LENGTH 16
#define MESSAGE "123456789ABCDEFGHIJKLMNOPQRST"
#define KEY ((unsigned char*) "c0c0c0c0c0c0c0c0")



static struct psock ps;
static uint8_t buffer[100];

PROCESS(example_psock_client_process, "Example protosocket client");
AUTOSTART_PROCESSES(&example_psock_client_process);

/*---------------------------------------------------------------------------*/
static int
handle_connection(struct psock *p)
{
	PSOCK_BEGIN(p);

	PSOCK_SEND_STR(p, "GET / HTTP/1.0\r\n");
	PSOCK_SEND_STR(p, "Server: Contiki example protosocket client\r\n");
	PSOCK_SEND_STR(p, "\r\n");

	while(1) {
		PSOCK_READTO(p, '\n');
		printf("Got: %s", buffer);
	}
	
	PSOCK_END(p);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_psock_client_process, ev, data)
{
	uip_ipaddr_t reciever_ip;

	PROCESS_BEGIN();

	// get ip address of the reciever
	static struct etimer periodic_timer;
	// uip_ipaddr_t reciever_ip;
	while (!NETSTACK_ROUTING.node_is_reachable() || !NETSTACK_ROUTING.get_root_ipaddr(&reciever_ip)) {
		LOG_INFO("Unable to find reciever, retry in %d seconds\n", SEND_INTERVAL);
		etimer_set(&periodic_timer, SEND_INTERVAL * CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
	}
	LOG_INFO("Found receiver's ip: ");
	LOG_INFO_6ADDR(&reciever_ip);
	LOG_INFO_("\n");


	tcp_connect(&reciever_ip, UIP_HTONS(1010), NULL);

	printf("Connecting...\n");
	PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

	if(uip_aborted() || uip_timedout() || uip_closed()) {
		printf("Could not establish connection (ABORTED:%d TIMEOUT:%d CLOSED:%d)\n", uip_aborted(), uip_timedout(), uip_closed());
	} else if(uip_connected()) {
		printf("Connected\n");
		
		PSOCK_INIT(&ps, buffer, sizeof(buffer));

		do {
			handle_connection(&ps);
			PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
		} while(!(uip_closed() || uip_aborted() || uip_timedout()));

		printf("\nConnection closed.\n");
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/