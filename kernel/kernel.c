#include "types.h"
#include "common.h"
#include "init.h"
#include "tools.h"
#include "display.h"
#include "timer.h"

void kmain(){
  flush_screen();
  init_descriptor_tables();

  // use new stack space
  asm volatile ("movw $0x18, %ax");
  asm volatile ("movw %ax, %ss");
  asm volatile ("movl %0, %%esp"::"a"(sizeof(stack)));
  
  kprint("KERNEL LOADED\n");

  asm volatile ("int $0x3");
  asm volatile ("int $0x4");

  /*init_timer(200);*/

  asm volatile ("sti");
  for(;;);
}
