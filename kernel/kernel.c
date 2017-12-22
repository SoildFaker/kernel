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

void kmain()
{
  flush_screen();
  init_descriptor_tables();
  init_page();
  kprint("PAGE OK\n");

  kprint("KERNEL LOADED\n");
  kprint("KERNEL START: %x\n", kernel_start);
  kprint("KERNEL END:   %x\n", kernel_end);
  kprint("KERNEL SIZE:  %d kb\n", (kernel_end - kernel_start+1023)/1024);

  show_memory_map();
  init_pmm();
  print_font8(10,40,1);
  print_font8(10,30,2);
  print_font8(10,10,5);
  print_font8(10,20,6);
  /*draw_line(0,0,50,50,0xf);;*/
  draw_rectangle(100,100, 60, 60, 0xf);

  kprint_color(COLOR_BROWN, COLOR_BLACK, "\nThe Count of Physical Memory Page is: % u\n\n", phy_page_count);
  
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
