#ifndef __TTY__
#define __TTY__

#include "common.h"

#define TTY_BUFFER_SIZE 0x1200
#define TTY_NUMBER      4

struct tty {
  u32 offset;
  u8 cursor_x;
  u8 cursor_y;
  volatile u16 *buffer;
};

extern struct tty *tty_cur;
extern struct tty tty[TTY_NUMBER];

void switch_tty(struct tty *tty);
void init_tty();

#endif
