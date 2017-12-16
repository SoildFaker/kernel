#include "types.h"
#include "common.h"
#include "init.h"
#include "tools.h"
#include "display.h"
#include "timer.h"

void kmain(){
  init_descriptor_tables();
  flush_screen();

  // use new stack space
  asm volatile ("movw $0x18, %ax");
  asm volatile ("movw %ax, %ss");
  asm volatile ("movl %0, %%esp"::"a"((u32)&stack+sizeof(stack)));
  
  kprint("KERNEL LOADED\n");

  asm volatile ("int $0x3");
  asm volatile ("int $0x4");

  /*init_timer(200);*/

  asm volatile ("sti");
  for(;;);
}

void gate_called()
{
  kprint("GATE CALLED\n");
}
