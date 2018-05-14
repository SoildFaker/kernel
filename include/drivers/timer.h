#ifndef DRIVERS_TIMER_H
#define DRIVERS_TIMER_H

#include "common.h"

void init_timer(u32 frequency);

extern volatile u32 tick;

#endif
