/*#include <tinx/types.h>*/
/*#include <boot/common.h>*/
/*#include <boot/descriptor_tables.h>*/

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

void main(){
  printStr("OK LOADED");
  for(;;);
}
