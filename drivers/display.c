#include "display.h"
#include "common.h"
#include "types.h"
#include "string.h"

volatile u8 cursor_x = 0;
volatile u8 cursor_y = 0;
volatile u16 *video_buffer = (volatile u16*)0xB8000;

static void flush_cursor()
{
  // for default vga support the screen width = 80
  u16 cursor_location = cursor_y * 80 + cursor_x;

  outb(14, 0x3d4); // set vga cursor high 8 bit
  outb(cursor_location >> 8, 0x3d5); // send cursor high 8 bit
  outb(15, 0x3d4); // set vga cursor low 8 bit
  outb(cursor_location, 0x3d5); // send cursor low 8 bit
}

void flush_line(u8 line)
{
  u8 attribute_byte = (COLOR_BLACK << 4) | (COLOR_WHITE);
  u16 blank = 0x20 | (attribute_byte << 8);
  u16 i;
  for (i=0; i<80; i++){
    video_buffer[line*80+i] = blank;
  }
  cursor_x = 0;
  flush_cursor();
}

void flush_screen()
{
  u8 attribute_byte = (COLOR_BLACK << 4) | (COLOR_WHITE);
  u16 blank = 0x20 | (attribute_byte << 8);
  u16 i;
  for (i=0; i<80*25; i++){
    video_buffer[i] = blank;
  }
  cursor_x = 0;
  cursor_y = 0;
  flush_cursor();
}

static void scroll()
{
  if (cursor_y > 24) {
    u16 i;

    for (i = 0 * 80; i < 24 * 80; i++) {
      video_buffer[i] = video_buffer[i+80];
    }
    flush_line(24);

    cursor_y = 24;
  }
}

void display_putc(char c, u8 fg, u8 bg)
{
  u16 attribute = (u16)(fg | bg << 4)<<8;

  if (c == 0x08 && cursor_x) {
    // backspace
    cursor_x--;
  } else if (c == 0x09) {
    //tab
    cursor_x = (cursor_x+8) & ~(8-1);
  } else if (c == '\r') {
    cursor_x = 0;
  } else if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
  } else if (c >= ' ') {
    video_buffer[cursor_y*80 + cursor_x] = c | attribute;
    cursor_x++;
  }
 
  // start new line when cursor_x > 79
  if (cursor_x > 79) {
    cursor_x = 0;
    cursor_y ++;
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

