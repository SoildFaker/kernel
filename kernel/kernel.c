#include "types.h"
#include "common.h"
#include "init.h"
#include "tools.h"
#include "display.h"
#include "timer.h"
#include "keyboard.h"
#include "mm.h"
#include "page.h"

extern u8 kernel_start[];
extern u8 kernel_end[];
void kmain(){
  flush_screen();
  init_descriptor_tables();
  init_page();
  kprint("PAGE OK\n");

  kprint("KERNEL LOADED\n");
  kprint("KERNEL START: %x\n", kernel_start);
  kprint("KERNEL END:   %x\n", kernel_end);
  kprint("KERNEL SIZE:  %d kb\n", (kernel_end - kernel_start+1023)/1024);

  show_memory_map();

  /*init_timer(200);*/
  init_keyboard();

  asm volatile("sti");
  
  for(;;);
}

void gate_called()
{
  kprint("GATE CALLED\n");
}
