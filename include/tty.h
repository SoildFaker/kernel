#ifndef __TTY__
#define __TTY__

#include "common.h"

typedef struct tty {
  u32 buffer_start;
  volatile u16 *buffer;
  struct tty *next;
} tty_t;

extern tty_t *tty_cur;

void switch_tty(tty_t *tty);
void init_tty();
u32 tty_task(void *arg);

#endif
