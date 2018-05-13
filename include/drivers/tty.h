#ifndef TTY_H
#define TTY_H

#include "common.h"

// 4 tty
#define TTY_NUMBER      4
// Every tty has 50 lines to buffer
#define TTY_BUFFER_SIZE (80 * 50 * 2)

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
u32 task_tty(__UNUSED__ void *arg);

#endif
