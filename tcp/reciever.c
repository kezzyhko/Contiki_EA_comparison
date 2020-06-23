#include "contiki.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"



// Constants
#define LOG_MODULE "Reciever"
#define LOG_LEVEL LOG_LEVEL_INFO
#define PORT 1357
#define KEY ((unsigned char*) "c0c0c0c0c0c0c0c0")
#define BLOCK_LENGTH 16



// Data will arrive here
static uint8_t buffer[BLOCK_LENGTH];
static PT_THREAD(handle_connection(struct psock *p)) {
	PSOCK_BEGIN(p);

	while (1) {
		PSOCK_READTO(p, '\n');
		printf("%s", buffer);
		// TODO: add decryption
	}
	
	PSOCK_END(p);
}



// Main process
PROCESS(example_psock_server_process, "Example protosocket server");
AUTOSTART_PROCESSES(&example_psock_server_process);
PROCESS_THREAD(example_psock_server_process, ev, data) {
	PROCESS_BEGIN();

	// wait for connection
	NETSTACK_ROUTING.root_start();
	tcp_listen(UIP_HTONS(PORT));
    LOG_INFO("Listening...\n");
	PROCESS_WAIT_EVENT_UNTIL(uip_connected());
	LOG_INFO("Connected\n");

	// accepting data
	static struct psock ps;
	PSOCK_INIT(&ps, buffer, sizeof(buffer));
	do {
		handle_connection(&ps);
		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
	} while (!uip_closed() && !uip_aborted() && !uip_timedout());

	LOG_INFO("Connection closed\n");
	PROCESS_END();
}