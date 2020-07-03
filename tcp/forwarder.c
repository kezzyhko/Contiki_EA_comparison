#include "contiki.h"

PROCESS(forwarding_process, "Forwarding process");
AUTOSTART_PROCESSES(&forwarding_process);
PROCESS_THREAD(forwarding_process, ev, data) {
  PROCESS_BEGIN();  
  PROCESS_END();
}