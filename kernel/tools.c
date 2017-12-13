#include <tools.h>
#include <types.h>

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

