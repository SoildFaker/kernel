#include "tty.h"
#include "display.h"

tty_t *tty_cur;

tty_t tty_1;
tty_t tty_2;

void init_tty()
{
  tty_1.buffer_start = 0xb8000;
  tty_2.buffer_start = 0xb8000;
  tty_1.buffer = (volatile u16 *)tty_1.buffer_start;
  tty_2.buffer = (volatile u16 *)tty_2.buffer_start;
  tty_1.next = &tty_2;
  tty_2.next = &tty_1;

  tty_cur = &tty_2;
  switch_tty(tty_cur);
}

u32 tty_task(void *arg)
{
  for(;;){
    if(tty_cur->buffer_start != buffer_start){
      /*switch_tty(cur_tty);*/
    }
  }
  return 0;
}

void switch_tty(tty_t *tty)
{
  outb(VBUFFER_START_H, CRT_ADDR_REG);
  outb(tty->buffer_start >> 8, CRT_DATA_REG);
  outb(VBUFFER_START_L, CRT_ADDR_REG);
  outb(tty->buffer_start, CRT_DATA_REG);
}
