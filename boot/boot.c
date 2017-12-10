#include <tinx/types.h>
#include <boot/boot.h>
#include <boot/gdt.h>

asm(".code16gcc\n");
asm("jmp $0x0000, $main\n");

void printStr(const char* pStr) {
  while(*pStr){
    asm volatile (
          "int $0x10" : : "a"(0x0e00 | *pStr), "b"(0x0007)
      );
    ++pStr;
  }
}

void protect_mode(){
  asm volatile ("mov %cr0, %eax\n");
  asm volatile ("or $0x1, %eax\n");
  asm volatile ("mov %eax, %cr0\n");
}

void main(){
  printStr("REAL MODE\r\n");
  struct GDT gdt_table[5];
  struct GDT_PTR _ptr;

  _ptr.limit = sizeof(gdt_table) * 8;
  _ptr.base = (u32)gdt_table;

  set_gdt(&gdt_table[0], 0, 0, 0, 0);
  set_gdt(&gdt_table[1], 0, 0xffff, 0x9a, 0x0f);
  set_gdt(&gdt_table[2], 0, 0xffff, 0x92, 0x0f);
  set_gdt(&gdt_table[3], 0, 0xffff, 0xfa, 0x0f);
  set_gdt(&gdt_table[4], 0, 0xffff, 0xf2, 0x0f);

  load_gdt(_ptr);
  protect_mode();

  printStr("PROTECT MODE");
}
