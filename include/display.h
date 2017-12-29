#ifndef __DISPLAY__
#define __DISPLAY__

#include "common.h"

#define COLOR_BLACK  0
#define COLOR_BLUE  1
#define COLOR_GREEN  2
#define COLOR_CYAN  3
#define COLOR_RED  4
#define COLOR_MAGENTA  5
#define COLOR_BROWN  6
#define COLOR_LIGHT_GREY  7
#define COLOR_DARK_GREY  8
#define COLOR_LIGHT_BLUE  9
#define COLOR_LIGHT_GREEN  10
#define COLOR_LIGHT_CYAN  11
#define COLOR_LIGHT_RED  12
#define COLOR_LIGHT_MAGENTA  13
#define COLOR_LIGHT_BROWN  14
#define COLOR_WHITE  15

#define CRT_ADDR_REG 0x3D4   // CRT controller register - Address Register
#define CRT_DATA_REG 0x3D5   // CRT controller register - Date Register
#define VBUFFER_START_H 0xC  // Video Buffer Start High 8 bit
#define VBUFFER_START_L 0xD  // Video Buffer Start Low 8 bit
#define CURSOR_H 0xE         // Cursor high 8 bit
#define CURSOR_L 0xF         // Cursor low 8 bit

extern volatile u16 *char_buffer;
extern u32 buffer_start;

void draw_line(u16 x1, u16 y1, u16 x2, u16 y2, u8 color);
void draw_rectangle(u16 x, u16 y, u16 width, u16 hight, u8 color);
void print_font8(u16 x, u16 y, u8 ch);

void flush_screen();
void flush_line(u8);
void display_print_color(const char *string, u8 fg, u8 bg);
void display_print(const char *string);
void display_putc(char c, u8 fg, u8 bg);
void display_print_hex(u32 num);

#endif
