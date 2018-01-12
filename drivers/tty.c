#include "drivers/tty.h"
#include "drivers/display.h"

struct tty *tty_cur;

struct tty tty[TTY_NUMBER];

static void set_video_start_addr(u32 addr);

void set_title_bar(struct tty *tty, u8 tty_num)
{
  u16 attribute = (u16)(COLOR_BLUE | COLOR_LIGHT_GREY << 4)<<8;
  volatile u16 *buffer = (volatile u16 *)(tty->buffer);
  u8 i;
  u8 title[6] = {'T', 'T', 'Y', ' ', (u8)(tty_num + '0'), 0};
  for (i=0; i<80; i++) {
    if (i<6) {
      buffer[i] = title[i]  | attribute;
    } else {
      buffer[i] = ' ' | attribute;
    }
  }
}

void init_tty()
{
  u8 i;
  for (i=0; i<TTY_NUMBER; i++ ){
    tty[i].offset = TTY_BUFFER_SIZE * i;
    tty[i].buffer = (volatile u16 *)(VBUFFER_MEM + tty[i].offset);
    tty[i].cursor_x = 0;
    tty[i].cursor_y = 1;
    set_title_bar(&tty[i], i);
  }
  tty_cur = &tty[0];
  tty_print = tty_cur;
  switch_tty(tty_cur);
}

// Switch video buffer start address
// using io function write registers to set VGA memory start address
// There is a confused thing, the start address must be half of video memory size
// Didn't know the reason
void switch_tty(struct tty *tty)
{
  if (tty_cur == tty) return;
  tty_cur = tty;
  set_video_start_addr(tty->offset>>1);
}

static void set_video_start_addr(u32 addr)
{
  cli();
  outb(CRT_ADDR_REG, VBUFFER_START_H);
  outb(CRT_DATA_REG, (addr >> 8) & 0xFF);
  outb(CRT_ADDR_REG, VBUFFER_START_L);
  outb(CRT_DATA_REG, addr & 0xFF);
  sti();
};
