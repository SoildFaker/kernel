#include "display.h"
#include "common.h"
#include "string.h"
#include "tty.h"

static void flush_cursor()
{
  // for default vga support the screen width = 80
  u16 cursor_location = tty_cur->cursor_y * 80 + tty_cur->cursor_x;

  outb(CURSOR_H, CRT_ADDR_REG); // set vga cursor high 8 bit
  outb(cursor_location >> 8, CRT_DATA_REG); // send cursor high 8 bit
  outb(CURSOR_L, CRT_ADDR_REG); // set vga cursor low 8 bit
  outb(cursor_location, CRT_DATA_REG); // send cursor low 8 bit
}

void flush_line(u8 line)
{
  u8 attribute_byte = (COLOR_BLACK << 4) | (COLOR_WHITE);
  u16 blank = 0x20 | (attribute_byte << 8);
  u16 i;
  for (i=0; i<80; i++){
    tty_cur->buffer[line*80+i] = blank;
  }
  tty_cur->cursor_x = 0;
  flush_cursor();
}

void flush_screen()
{
  u8 attribute_byte = (COLOR_BLACK << 4) | (COLOR_WHITE);
  u16 blank = 0x20 | (attribute_byte << 8);
  u16 i;
  for (i=0; i<80*25; i++){
    tty_cur->buffer[i] = blank;
  }
  tty_cur->cursor_x = 0;
  tty_cur->cursor_y = 0;
  flush_cursor();
}

static void scroll()
{
  if (tty_cur->cursor_y > 24) {
    u16 i;

    for (i = 0 * 80; i < 24 * 80; i++) {
      tty_cur->buffer[i] = tty_cur->buffer[i+80];
    }
    flush_line(24);

    tty_cur->cursor_y = 24;
  }
}

void display_putc(char c, u8 fg, u8 bg)
{
  u16 attribute = (u16)(fg | bg << 4)<<8;

  if (c == 0x08 && tty_cur->cursor_x) {
    // backspace
    tty_cur->cursor_x--;
    tty_cur->buffer[tty_cur->cursor_y*80 + tty_cur->cursor_x] = ' ' | ((COLOR_WHITE|COLOR_BLACK<<4)<<8);
  } else if (c == 0x09) {
    //tab
    tty_cur->cursor_x = (tty_cur->cursor_x+8) & ~(8-1);
  } else if (c == '\r') {
    tty_cur->cursor_x = 0;
  } else if (c == '\n') {
    tty_cur->cursor_x = 0;
    tty_cur->cursor_y++;
  } else if (c >= ' ') {
    tty_cur->buffer[tty_cur->cursor_y*80 + tty_cur->cursor_x] = c | attribute;
    tty_cur->cursor_x++;
  }
 
  // start new line when tty_cur->cursor_x > 79
  if (tty_cur->cursor_x > 79) {
    tty_cur->cursor_x = 0;
    tty_cur->cursor_y ++;
  }
 
  scroll();
  flush_cursor();
}

void display_print(const char *string)
{
  while (*string) {
    display_putc(*string++, COLOR_WHITE, COLOR_BLACK);
  }
}

void display_print_hex(u32 num)
{
  char *pc = (char *)"0x00000000";
  char *pc_t = pc;
    pc += 10;
  do{
    *(--pc) = "0123456789abcdef"[num % 16];
    num/=16;
  }while(*(pc-1)!='x');
  pc = pc_t;
  display_print(pc);
}

void display_print_color(const char *string, u8 fg, u8 bg)
{
  while (*string) {
    display_putc(*string++, fg, bg);
  }
}

