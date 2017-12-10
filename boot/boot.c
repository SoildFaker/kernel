#include <tinx/types.h>
#include <boot/gdt.h>

__asm__(".code16gcc\n");
__asm__("jmpl $0x0000, $main\n");

void printStr(const char* pStr) {
  while(*pStr){
    __asm__ __volatile__ (
          "int $0x10" : : "a"(0x0e00 | *pStr), "b"(0x0007)
      );
    ++pStr;
  }
}

void main(){
  struct gdt gdt_table[5];

  set_gdt(&gdt_table[0], 0, 0, 0, 0);
  set_gdt(&gdt_table[1], 0, 0xffff, 0x9a, 0x0f);
  set_gdt(&gdt_table[2], 0, 0xffff, 0x92, 0x0f);
  set_gdt(&gdt_table[3], 0, 0xffff, 0xfa, 0x0f);
  set_gdt(&gdt_table[4], 0, 0xffff, 0xf2, 0x0f);

  printStr("Hello, World");
}
