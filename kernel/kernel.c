#include "common.h"
#include "init.h"
#include "tools.h"
#include "display.h"
#include "timer.h"
#include "keyboard.h"
#include "mm.h"
#include "task.h"
#include "page.h"

static inline void init_stack()
{
  // use new stack space
  asm volatile ("movw $0x18, %ax");
  asm volatile ("movw %ax, %ss");
  asm volatile ("movl %0, %%ebp"::"r"((u32)kstack));
  asm volatile ("movl %0, %%esp"::"r"((u32)kstack + sizeof(kstack)));
}

void show_memory_map()
{
  u8 i=0;
  mmap_entry_t *map_entry = mmap;
  kprint("----------MEMORY MAP----------\n");
  for (i = 0; i < *count; i++){
    kprint("BASE: 0x%09X\tLENGTH: 0x%09X\tTYPE:0x%01X\n", 
      ((map_entry+i)->base_low | (map_entry+i)->base_high<<8), 
      ((map_entry+i)->length_low | (map_entry+i)->length_high<<8), 
      (map_entry+i)->type);
  }
}

u32 sw,a = 0;

u32 test_a(void *arg)
{
  while(1){
    if (sw == 0){
      display_putc('A',COLOR_RED, COLOR_BLACK);
      sw = 1;
    }
    a++;
  }
  return 0;
}


u32 test_b(void *arg)
{
  while(1){
    if (sw == 2){
      display_putc('B', COLOR_BLUE, COLOR_BLACK);
      sw = 0;
    }
    a++;
  }
  return 0;
}

void kmain()
{
  flush_screen();
  init_descriptor_tables();
  init_stack();
  init_page();
  init_pmm();
  kprint("KERNEL LOADED\n");
  kprint("KERNEL START: %x\n", kernel_start);
  kprint("KERNEL END:   %x\n", kernel_end);
  kprint("KERNEL SIZE:  %d kb\n", (kernel_end - kernel_start+1023)/1024);

  show_memory_map();

  init_task();
  kthread_start(test_a, NULL);
  kthread_start(test_b, NULL);

  init_timer(20);
  init_keyboard();

  asm volatile("sti");
  while(1){
    if (sw == 1){
      display_putc('K',COLOR_GREEN, COLOR_BLACK);
      sw = 2;
    }
    a++;
  }
  asm volatile("hlt");

}

void gate_called()
{
  kprint("GATE CALLED\n");
}
  /*kprint_color(COLOR_BROWN, COLOR_BLACK, "\nThe Count of Physical Memory Page is: % u\n\n", phy_page_count);*/
