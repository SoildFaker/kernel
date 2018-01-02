#include "common.h"
#include "init.h"
#include "tools.h"
#include "display.h"
#include "timer.h"
#include "keyboard.h"
#include "mm.h"
#include "task.h"
#include "page.h"
#include "test.h" // kernel test function
#include "tty.h"

static inline void init_stack();
static void print_info();

void kmain(void)
{
  // kernel state init
  // no print function here
  init_gdt();
  init_idt();
  init_stack();

  // print function inited
  init_tty();
  flush_screen();

  // memory management inited
  init_page();
  init_pmm();

  // print system info
  print_info();

  // multi task init
  init_task();
  kthread_start(test_a, &tty[1], NULL);
  kthread_start(test_b, &tty[1], NULL);
  kthread_start(test_c, &tty[1], NULL);
  
  // interrupt handler registed
  init_timer(10);     // schedule() here
  init_keyboard();

  // allow interrupt
  sti();

  // print kernel task runs how many times
  while(1){
    kprint("Proc Runs:%d\r", a);
  }
  asm volatile("hlt");

}

static inline void init_stack()
{
  // use new stack space
  asm volatile ("movw $0x10, %ax");
  asm volatile ("movw %ax, %ss");
  asm volatile ("movl %0, %%ebp"::"r"((u32)kstack));
  asm volatile ("movl %0, %%esp"::"r"((u32)kstack + sizeof(kstack)));
}

static void print_info()
{
  u8 i=0;
  mmap_entry_t *map_entry = mmap;

  kprint("KERNEL LOADED\n");
  kprint("KERNEL START: %x\n", kernel_start);
  kprint("KERNEL END:   %x\n", kernel_end);
  kprint("KERNEL SIZE:  %d kb\n", (kernel_end - kernel_start+1023)/1024);

  kprint("----------MEMORY MAP----------\n");
  for (i = 0; i < *count; i++){
    kprint("BASE: 0x%09X\tLENGTH: 0x%09X\tTYPE:0x%01X\n", 
      ((map_entry+i)->base_low | (map_entry+i)->base_high<<8), 
      ((map_entry+i)->length_low | (map_entry+i)->length_high<<8), 
      (map_entry+i)->type);
  }
}


