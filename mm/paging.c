#include "types.h"
#include "paging.h"

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
