#include <string.h>
#include <stdio.h>
#include "contiki-net.h"
#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"



// Constants
#define LOG_MODULE "Reciever"
#define LOG_LEVEL LOG_LEVEL_INFO
#define SENDER_PORT 1357
#define RECIEVER_PORT 2468
#define BLOCK_LENGTH 16
#define KEY ((unsigned char*) "c0c0c0c0c0c0c0c0")



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


PROCESS(example_psock_server_process, "Example protosocket server");
AUTOSTART_PROCESSES(&example_psock_server_process);
PROCESS_THREAD(example_psock_server_process, ev, data)
{

	PROCESS_BEGIN();
	NETSTACK_ROUTING.root_start();

	tcp_listen(UIP_HTONS(1010));
    printf("Listening...\n");

	while(1) {


		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);


		if(uip_connected()) {
    		printf("Connected\n");
			

			PSOCK_INIT(&ps, buffer, sizeof(buffer));


			while(!(uip_aborted() || uip_closed() || uip_timedout())) {


	PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);


	handle_connection(&ps);
			}
		}
	}
	

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/