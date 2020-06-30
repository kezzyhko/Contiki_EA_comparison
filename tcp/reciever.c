#include "contiki.h"
#include "contiki-net.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "../cryptolibs/crypto.h"
#include "statistics.h"
#include "constants.h"

#include "sys/log.h"
#define LOG_MODULE "Reciever"
#define LOG_LEVEL LOG_LEVEL_INFO



// Data will arrive here
static uint8_t buffer[BLOCK_LENGTH];
static PT_THREAD(handle_connection(struct psock *p)) {
	PSOCK_BEGIN(p);

	while (1) {
		// read
		PSOCK_READBUF_LEN(p, BLOCK_LENGTH);

		if (PSOCK_DATALEN(p) == BLOCK_LENGTH) {
			// decrypt
			unsigned char block_decrypted[BLOCK_LENGTH+1];
			decrypt(buffer, block_decrypted);
			block_decrypted[BLOCK_LENGTH] = 0;

			// log
			LOG_INFO("Received \"");
			int j = 0;
			for (j = 0; j < PSOCK_DATALEN(p); j++) {
				LOG_INFO_("%02x", buffer[j]);
			}
			LOG_INFO_("\" (\"%s\")\n", block_decrypted);
		} else {
			LOG_INFO("Data has the wrong length (%d), can not decode it.\n", PSOCK_DATALEN(p));
		}
	}
	
	PSOCK_END(p);
}



// Reciever process
PROCESS(reciever_process, "Reciever");
AUTOSTART_PROCESSES(&reciever_process);
PROCESS_THREAD(reciever_process, ev, data) {
	PROCESS_BEGIN();

	// wait for connection
	NETSTACK_ROUTING.root_start();
	tcp_listen(UIP_HTONS(PORT));
    LOG_INFO("Listening...\n");
	PROCESS_WAIT_EVENT_UNTIL(uip_connected());
	LOG_INFO("Connected\n");
	log_energest_statistics();

	// protothread for recieving data
	setKey((unsigned char *)(KEY), (sizeof KEY - 1) * sizeof(char));
	static struct psock ps;
	PSOCK_INIT(&ps, buffer, sizeof(buffer));
	do {
		handle_connection(&ps);
		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
	} while (!uip_closed() && !uip_aborted() && !uip_timedout());

	LOG_INFO("Connection closed\n");
	log_energest_statistics();
	PROCESS_END();
}