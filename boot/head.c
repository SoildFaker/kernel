#include <tinx/types.h>
#include <boot/common.h>
#include <boot/descriptor_tables.h>

asm(".code16gcc\n");
asm("jmp $0x0000, $kmain\n");

void printStr(const char* pStr) {
  while(*pStr){
    asm volatile (
          "int $0x10" : : "a"(0x0e00 | *pStr), "b"(0x0007)
      );
    ++pStr;
  }
}

// note this example will always write to the top
// line of the screen
void write_string( int colour, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = colour;
    }
}

void kmain(){
  init_descriptor_tables();
  switch_protect();
  
  write_string(32, "OK");
  for(;;);
}
