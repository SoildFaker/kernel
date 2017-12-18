#include "types.h"
#include "init.h"
#include "page.h"
#include "display.h"
#include "tools.h"

__attribute__((aligned(4096))) page_entry_t pdt[1024];
__attribute__((aligned(4096))) page_entry_t pet[1024];
void init_page()
{
  u32 i;
  for(i=0; i<1024;i++){
    pet[i].base = ((0x000000 + i*0x1000)>>12);
    pet[i].flags = 0x1;
  }
  pdt[0].base = (u32)(&pet)>>12;
  pdt[0].flags = 1;

  register_interrupt_handler(14, (interrupt_handler_t)page_fault);

  flush_page_directory(pdt);
  enable_page();
}
void flush_page_directory(page_entry_t *pgdir) {
  asm volatile("mov %%eax, %%cr3":: "a"(pgdir));    // put page table addr
}

void enable_page() {
  u32 cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000;                        // enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void page_fault(pt_regs *regs)
{
  // A page fault has occurred.
  // The faulting address is stored in the CR2 register.
  u32 faulting_address;
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  // The error code gives us details of what happened.
  int present   = !(regs->err_code & 0x1); // Page not present
  int rw = regs->err_code & 0x2;           // Write operation?
  int us = regs->err_code & 0x4;           // Processor was in user-mode?
  int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
  //int id = regs->err_code & 0x10;          // Caused by an instruction fetch?

  // Output an error message.
  display_print("Page fault! ( ");
  if (present) {display_print("present ");}
  if (rw) {display_print("read-only ");}
  if (us) {display_print("user-mode ");}
  if (reserved) {display_print("reserved ");}
  display_print(") at 0x");
  display_print_hex(faulting_address);
  display_print("\n");
  PANIC("Page fault");
}
