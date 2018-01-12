#include "common.h"
#include "init.h"
#include "tools.h"
#include "mm.h"
#include "task.h"
#include "page.h"
#include "syscall.h"
#include "test.h" // kernel test function
#include "fs/myfs.h"
#include "drivers/display.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "drivers/tty.h"

static inline void init_stack();
static void print_info();

void kernel_start(void)
{
  // kernel state initialising
  // no print function here
  init_desc();
  init_stack();

  // print function initialized
  init_tty();
  flush_screen();

  // memory management initialized
  init_page();
  init_pmm();

  // print system info
  print_info();

  // multi task init
  init_task();

  // initialises system call
  init_syscall();

  // interrupt handler registed
  init_keyboard();
  init_timer(200);     // schedule() here

  kthread_start(task_idle, &tty[0], 10, NULL);
  /*kthread_start(test_a, &tty[1], 2, NULL);*/
  kthread_start(test_b, &tty[2], 3, NULL);
  kthread_start(test_c, &tty[3], 4, NULL);
  

  // allow interrupt
  sti();
 
  init_myfs(0x20000+PAGE_OFFSET);
  struct myfs_entry *tmp = find_file("kernel.elf");
  printk("%s\nstart:%d\tlength:%d\n", tmp->entry_name, tmp->data_sector, tmp->data_count);
  // print kernel task runs how many times
  while(1){
    printk("Proc Runs:%d\r", current->time_slice);
  }
  asm volatile("hlt");

}

static inline void init_stack()
{
  // use new stack space
  asm volatile ("movw $0x10, %ax");
  asm volatile ("movw %ax, %ss");
  /*asm volatile ("movl %0, %%ebp"::"r"((u32)kernel_stack));*/
  asm volatile ("xorl %ebp, %ebp");
  asm volatile ("movl %0, %%esp"::"r"((u32)kernel_stack + sizeof(kernel_stack)));
}

static void print_info()
{
  u8 i=0;
  mmap_entry_t *map_entry = mmap;

  printk("KERNEL LOADED\n");
  printk("KERNEL START: %x\n", kernel_start_pos);
  printk("KERNEL END:   %x\n", kernel_end_pos);
  printk("KERNEL SIZE:  %d kb\n", (kernel_end_pos - kernel_start_pos+1023)/1024);

  printk("----------MEMORY MAP----------\n");
  for (i = 0; i < *count; i++){
    printk("BASE: 0x%09X\tLENGTH: 0x%09X\tTYPE:0x%01X\n", 
      ((map_entry+i)->base_low | (map_entry+i)->base_high<<8), 
      ((map_entry+i)->length_low | (map_entry+i)->length_high<<8), 
      (map_entry+i)->type);
  }
}


