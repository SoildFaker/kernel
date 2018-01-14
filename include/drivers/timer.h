#ifndef __TIMER__
#define __TIMER__

#include "common.h"

void init_timer(u32 frequency);

extern volatile u32 tick;

#endif // INCLUDE_TIMER_H_
