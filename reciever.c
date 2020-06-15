#include "contiki.h"
#include "contiki-net.h"

#include <stdio.h>
#include <string.h>



static struct psock ps;
static uint8_t buffer[10];

static
PT_THREAD(handle_connection(struct psock *p))
{
	PSOCK_BEGIN(p);

	PSOCK_SEND_STR(p, "Welcome, please type something and press return.\n");
	PSOCK_READTO(p, '\n');
	
	PSOCK_SEND_STR(p, "Got the following data: ");
	PSOCK_SEND(p, buffer, PSOCK_DATALEN(p));
	PSOCK_SEND_STR(p, "Good bye!\r\n");

	PSOCK_CLOSE(p);
	PSOCK_END(p);
}



PROCESS(listen_process, "Reciever's listener process");

AUTOSTART_PROCESSES(&listen_process);

PROCESS_THREAD(listen_process, event, data) {
	PROCESS_BEGIN();
	uip_init();

	uip_ipaddr_t addr;

	uip_ipaddr(&addr, 192,168,1,2);
	uip_sethostaddr(&addr);

	uip_ipaddr(&addr, 0,0,0,0);
	uip_setdraddr(&addr);

	uip_ipaddr(&addr, 255,255,255,0);
	uip_setnetmask(&addr);
	
	uip_listen(UIP_HTONS(8080));
	printf("Listening...\n");
	PROCESS_WAIT_EVENT_UNTIL(event == tcpip_event);

	if (uip_connected()) {
		printf("Got incoming connection...\n");
		PSOCK_INIT(&ps, buffer, sizeof(buffer));
		while (!uip_aborted() && !uip_closed() && !uip_timedout()) {
			handle_connection(&ps);
		}
	}

	PROCESS_END();
}