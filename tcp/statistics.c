#include "contiki.h"
#include "sys/energest.h"

#include "sys/log.h"
#define LOG_MODULE "Energest"
#define LOG_LEVEL LOG_LEVEL_INFO

void log_energest_statistics(char* str) {
	energest_flush();
	LOG_INFO("%s \t SECOND %lu \t TOTAL %lu \t LISTEN %lu \t TRANSMIT %lu \t CPU %lu \t LPM %lu \t DEEP_LPM %lu\n",
		str,
		(unsigned long)ENERGEST_SECOND,
		(unsigned long)ENERGEST_GET_TOTAL_TIME(),
		(unsigned long)energest_type_time(ENERGEST_TYPE_LISTEN),
		(unsigned long)energest_type_time(ENERGEST_TYPE_TRANSMIT),
		(unsigned long)energest_type_time(ENERGEST_TYPE_CPU),
		(unsigned long)energest_type_time(ENERGEST_TYPE_LPM),
		(unsigned long)energest_type_time(ENERGEST_TYPE_DEEP_LPM)
	);
}