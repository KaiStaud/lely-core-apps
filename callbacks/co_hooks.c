//
// Created by kai on 1/2/26.
//
#include "co_hooks.h"
#include <stdlib.h>
#include "../ulog/ulog.h"
#include <lely/co/nmt.h>
void on_nmt_cs(co_nmt_t *nmt, co_unsigned8_t cs, void *data) {
  (void)data;

  switch (cs) {
  case CO_NMT_CS_START:
    // Reset the TIME indication function, since the service may
    // have been restarted.
    co_time_set_ind(co_nmt_get_time(nmt), &on_time, NULL);
    break;
  case CO_NMT_CS_STOP:
    break;
  case CO_NMT_CS_ENTER_PREOP:
    co_time_set_ind(co_nmt_get_time(nmt), &on_time, NULL);
    break;
  case CO_NMT_CS_RESET_NODE:
    // Initiate a system reset.
    exit(0);
    break;
  case CO_NMT_CS_RESET_COMM:
    break;
  }
}

void on_time(co_time_t *time, const struct timespec *tp, void *data) {
  (void)time;
  (void)data;

  // Update the wall clock, _not_ the monotonic clock used by the CAN
  // network.
  clock_settime(CLOCK_REALTIME, tp);
}