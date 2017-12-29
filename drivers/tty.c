#include "tty.h"
#include "display.h"

tty_t *tty_cur;

tty_t tty_1;
tty_t tty_2;

void init_tty()
{
  tty_1.offset = 0;
  tty_2.offset = TTY_BUFFER_SIZE;
  tty_1.buffer = (volatile u16 *)(VBUFFER_MEM + tty_1.offset);
  tty_2.buffer = (volatile u16 *)(VBUFFER_MEM + tty_2.offset);
  tty_1.next = &tty_2;
  tty_2.next = &tty_1;

  tty_cur = &tty_1;
  switch_tty(tty_cur);
}

u32 i = 0;
u32 tty_task(void *arg)
{
  for(;;){
    if (i>0x2fffff){
      switch_tty(tty_cur->next);
      i=0;
    }
    i++;
  }
  return 0;
}

void switch_tty(tty_t *tty)
{
  tty_cur = tty;
  outb(VBUFFER_START_H, CRT_ADDR_REG);
  outb(((tty->offset>>1) + VBUFFER_MEM) >> 8, CRT_DATA_REG);
  outb(VBUFFER_START_L, CRT_ADDR_REG);
  outb(((tty->offset>>1) + VBUFFER_MEM), CRT_DATA_REG);
}
