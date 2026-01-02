//
// Created by kai on 1/2/26.
//

#ifndef CPP_TUTORIAL_CO_HOOKS_H
#define CPP_TUTORIAL_CO_HOOKS_H
#include <lely/co/time.h>
#include <lely/can/msg.h>

void on_nmt_cs(co_nmt_t *nmt, co_unsigned8_t cs, void *data);
void on_time(co_time_t *time, const struct timespec *tp, void *data);

#endif //CPP_TUTORIAL_CO_HOOKS_H