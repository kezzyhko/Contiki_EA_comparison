#include "contiki.h"
#include "contiki-net.h"

#include <stdio.h>
#include <string.h>



static struct psock ps;
static uint8_t buffer[100];

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



PROCESS(connect_process, "Sender's connection process");

AUTOSTART_PROCESSES(&connect_process);

PROCESS_THREAD(connect_process, event, data) {
	PROCESS_BEGIN();

	uip_ipaddr_t addr;

	uip_ipaddr(&addr, 192,168,1,1);
	uip_sethostaddr(&addr);

	uip_ipaddr(&addr, 0,0,0,0);
	uip_setdraddr(&addr);

	uip_ipaddr(&addr, 255,255,255,0);
	uip_setnetmask(&addr);

	uip_ipaddr(&addr, 192,168,1,2);
	uip_connect(&addr, UIP_HTONS(8080));
	printf("Connecting...\n");
	PROCESS_WAIT_EVENT_UNTIL(event == tcpip_event);

	if (uip_aborted() || uip_timedout() || uip_closed()) {
		printf("Could not establish connection\n");
	} else if(uip_connected()) {
		printf("Connected\n");
		
		PSOCK_INIT(&ps, buffer, sizeof(buffer));

		do {
			handle_connection(&ps);
			PROCESS_WAIT_EVENT_UNTIL(event == tcpip_event);
		} while (!uip_closed() && !uip_aborted() && !uip_timedout());

		printf("\nConnection closed.\n");
	}

	PROCESS_END();
}