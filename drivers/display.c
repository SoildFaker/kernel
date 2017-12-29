#include "display.h"
#include "common.h"
#include "font.h"
#include "string.h"
#include "tty.h"

volatile u8 cursor_x = 0;
volatile u8 cursor_y = 0;

volatile u8 *video_buffer = (volatile u8*)0xa0000;

static int abs(int a)
{
  int temp = (a >> 31); 
  return (a + temp) ^ temp;
}

void set_pixel(u16 x, u16 y, u8 color)
{
  video_buffer[y*320 +x] = color;
}

void print_font8(u16 x, u16 y, u8 ch)
{
  u16 i,j;
  for(i=x; i<y+8; i++){
    for(j=y; j<y+8; j++){
      if(((font8[ch][j-y]>>(i-x))&0x1) == 1)
        set_pixel(i,j,0xf);
    }
  }
}

void draw_line(u16 x1, u16 y1, u16 x2, u16 y2, u8 color)
{
  int dx = x2 - x1;
  int dy = y2 - y1;
  int ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
  int uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
  int x = x1, y = y1, eps;//eps为累加误差

  eps = 0;dx = abs(dx); dy = abs(dy); 
  if (dx > dy) {
    for (x = x1; x != x2; x += ux) {
      set_pixel(x,y,color);
      eps += dy;
      if ((eps << 1) >= dx) {
        y += uy; eps -= dx;
      }
    }
  } else {
    for (y = y1; y != y2; y += uy) {
      set_pixel(x,y,color);
      eps += dx;
      if ((eps << 1) >= dy) {
        x += ux; eps -= dy;
      }
    }
  }
}

void draw_rectangle(u16 x, u16 y, u16 width, u16 hight, u8 color)
{
  u16 i,j;
  for (i=x; i<x+width; i++){
    for (j=y; j<y+hight; j++){
      set_pixel(i,j,color);
    }
  }

}

static void flush_cursor()
{
  // for default vga support the screen width = 80
  u16 cursor_location = cursor_y * 80 + cursor_x;

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
  cursor_x = 0;
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
  cursor_x = 0;
  cursor_y = 0;
  flush_cursor();
}

static void scroll()
{
  if (cursor_y > 24) {
    u16 i;

    for (i = 0 * 80; i < 24 * 80; i++) {
      tty_cur->buffer[i] = tty_cur->buffer[i+80];
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
    tty_cur->buffer[cursor_y*80 + cursor_x] = ' ' | ((COLOR_WHITE|COLOR_BLACK<<4)<<8);
  } else if (c == 0x09) {
    //tab
    cursor_x = (cursor_x+8) & ~(8-1);
  } else if (c == '\r') {
    cursor_x = 0;
  } else if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
  } else if (c >= ' ') {
    tty_cur->buffer[cursor_y*80 + cursor_x] = c | attribute;
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

