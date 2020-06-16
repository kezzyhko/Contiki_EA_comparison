#include "contiki.h"
#include "contiki-net.h"
// #include "uip-debug.h"
#include <stdio.h>



PROCESS(hello_world_process, "Hello world process");

AUTOSTART_PROCESSES(&hello_world_process);

PROCESS_THREAD(hello_world_process, event, data)
{
	PROCESS_BEGIN();
	uip_init();

	uip_ipaddr_t reciever_ip;
	// uip_ip6addr(&reciever_ip, 0xfe80, 0x0, 0x0, 0x0, 0x212, 0x7401, 0x1, 0x101);
	// uip_ip6addr(&reciever_ip, 0xfe80, 0x0, 0x0, 0x0, 0x12, 0x7401, 0x1, 0x101);
	uip_ip6addr(&reciever_ip, 0xff02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1a);
	// uip_debug_ipaddr_print(&reciever_ip); putchar('\n'); // debug print

	struct uip_conn *conn;
	printf("%s\n", "Connecting...");
	conn = tcp_connect(&reciever_ip, UIP_HTONS(8080), NULL);
	if (conn == NULL) {
		printf("%s\n", "Connecting failed");
	}
  
	PROCESS_END();
}