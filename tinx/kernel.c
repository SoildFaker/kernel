#include <tinx/types.h>
#include <tinx/common.h>
#include <tinx/descriptor_tables.h>

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

// note this example will always write to the top
// line of the screen
void write_string( u8 color_bg, u8 color_fg, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = (color_fg | color_bg << 4);
    }
}

void kmain(){

  init_descriptor_tables();
  
  write_string(COLOR_BLACK, COLOR_GREEN, "OK------LOADED");
  for(;;);
}
