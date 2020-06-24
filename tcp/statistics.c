#include "contiki.h"
#include "sys/energest.h"
#include "sys/log.h"



// The following line is needed to fix "overflowed by 2 bytes" error
char *fix = "12345";



// Constants
#define LOG_MODULE "Energest"
#define LOG_LEVEL LOG_LEVEL_INFO



// Statistics process
PROCESS(statistics_process, "Statistics process");
PROCESS_THREAD(statistics_process, ev, data) {
	PROCESS_BEGIN();
	static struct etimer periodic_timer;
	etimer_set(&periodic_timer, CLOCK_SECOND/100);

	while (1) {
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
		etimer_reset(&periodic_timer);

		energest_flush();
		LOG_INFO("SECOND %lu \t TOTAL %lu \t LISTEN %lu \t TRANSMIT %lu \t CPU %lu \t LPM %lu \t DEEP_LPM %lu \n",
			(unsigned long)CLOCK_SECOND,
			(unsigned long)ENERGEST_GET_TOTAL_TIME(),
			(unsigned long)energest_type_time(ENERGEST_TYPE_LISTEN),
			(unsigned long)energest_type_time(ENERGEST_TYPE_TRANSMIT),
			(unsigned long)energest_type_time(ENERGEST_TYPE_CPU),
			(unsigned long)energest_type_time(ENERGEST_TYPE_LPM),
			(unsigned long)energest_type_time(ENERGEST_TYPE_DEEP_LPM)
		);
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/