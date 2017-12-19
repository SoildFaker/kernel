#include "types.h"
#include "common.h"
#include "init.h"
#include "tools.h"
#include "display.h"
#include "timer.h"
#include "keyboard.h"
#include "mm.h"
#include "task.h"
#include "page.h"

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

  init_task();
  init_timer(20);
  init_keyboard();

  asm volatile("sti");
  
  asm volatile("hlt");
  for(;;);

}

void gate_called()
{
  kprint("GATE CALLED\n");
}
