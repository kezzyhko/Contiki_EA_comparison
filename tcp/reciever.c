#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>



PROCESS(hello_world_process, "Hello world process");

AUTOSTART_PROCESSES(&hello_world_process);

PROCESS_THREAD(hello_world_process, event, data)
{
	PROCESS_BEGIN();
	uip_init();

	tcp_listen(UIP_HTONS(8080));
	printf("%s\n", "Listening...");
	while (1) {
		PROCESS_WAIT_EVENT();
		printf("%d\n", event);
	}
	// printf("%d\n", tcpip_event);
	// printf("%s\n", "Client is trying to connect");
  
	PROCESS_END();
}