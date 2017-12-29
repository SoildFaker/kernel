#ifndef __TTY__
#define __TTY__

#include "common.h"

#define TTY_BUFFER_SIZE 0x1000
typedef struct tty {
  u32 offset;
  u8 cursor_x;
  u8 cursor_y;
  volatile u16 *buffer;
  struct tty *next;
} tty_t;

extern tty_t *tty_cur;
extern tty_t tty_1;
extern tty_t tty_2;

void switch_tty(tty_t *tty);
void init_tty();
u32 tty_task(void *arg);

#endif
