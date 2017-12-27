#include "init.h"
#include "page.h"
#include "mm.h"
#include "display.h"
#include "tools.h"

page_entry_t kpdt[1024] __attribute__((aligned(PAGE_SIZE)));
static page_entry_t pet[KPDT_COUNT][1024] __attribute__((aligned(PAGE_SIZE)));

static page_entry_t *unused_page()
{
  u32 i,j;
  for (i=0; i<1024; i++){
    page_entry_t *pet_cur = (page_entry_t *)((u32)kpdt[i].base<<12);
    if(pet_cur){
      for (j=0; j<1024; j++){
        if (pet_cur[j].flags == 0){
          return &pet_cur[j];
        }
      }
    }
  }
  return 0;
}

void init_page()
{
  u32 address = 0;
  /*u32 address = ((u32)kernel_start & PAGE_MASK) - 0x1000;*/
  while (address < (((u32)kernel_end & PAGE_MASK) + 0x1000)){
    (*pet+(address>>12))->base  = address >> 12;
    (*pet+(address>>12))->flags = PG_PRESENT | PG_WRITE;
    address += PAGE_SIZE;
  }
  u32 i;
  for (i=0; i<KPDT_COUNT; i++){
    kpdt[i].base = (u32)pet[i] >> 12;
    kpdt[i].flags = PG_PRESENT | PG_WRITE;
  }
  register_interrupt_handler(14, (interrupt_handler_t)page_fault);

  switch_pdt(kpdt);
  enable_page();
}

void map(page_entry_t *pdt_now, u32 va, u32 pa, u32 flags)
{
  u32 pdt_idx = PDT_INDEX(va);
  u32 pet_idx = PET_INDEX(va);
  page_entry_t *pet_now;

  pet_now = (page_entry_t *)((u32)pdt_now[pdt_idx].base << 12);
  // if the PET not present
  /*if ((pdt_now[pdt_idx].flags & 0x01) == 0){*/
  if (!pet_now){
    u32 new_pet_addr = pmm_alloc_page();
    // find an existed PET entry for new page
    page_entry_t *new_page = unused_page();
    new_page->base = new_pet_addr >> 12;
    new_page->flags = PG_PRESENT | PG_WRITE;

    // PDT entry point to the PET contained new_page
    pdt_now[pdt_idx].base = (u32)new_page >> 12;
    pdt_now[pdt_idx].flags = PG_PRESENT | PG_WRITE;

    // pet_now entry point to new page
    pet_now = (page_entry_t *)((u32)new_page & PAGE_MASK);
  }
  pet_now[pet_idx].base = (pa >> 12);
  pet_now[pet_idx].flags = flags;
  // 通知 CPU 更新页表缓存
  asm volatile ("invlpg (%0)" : : "a" (va));
}

void unmap(page_entry_t *pdt_now, u32 va)
{
  u32 pdt_idx = PDT_INDEX(va);
  u32 pet_idx = PET_INDEX(va);
  
  if ((pdt_now[pdt_idx].flags & 0x01) == 0) {
    return;
  }
  
  page_entry_t *pet_now = (page_entry_t *)((u32)pdt_now[pdt_idx].base << 12);
  pet_now[pet_idx].flags = 0;
  
  // 通知 CPU 更新页表缓存
  asm volatile ("invlpg (%0)" : : "a" (va));
}

u32 get_mapping(page_entry_t *pdt_now, u32 va, u32 *pa)
{
  u32 pdt_idx = PDT_INDEX(va);
  u32 pet_idx = PET_INDEX(va);
  
  if ((pdt_now[pdt_idx].flags & 0x01) == 0) {
    return 0;
  }

  page_entry_t *pet_now = (page_entry_t *)((u32)pdt_now[pdt_idx].base << 12);
  // 如果地址有效而且指针不为，则返回地址NULL
  if (pet_now[pet_idx].flags != 0 && pa) {
    *pa = pet_now[pet_idx].base << 12;
    return 1;
  }

  return 0;
}

void switch_pdt(page_entry_t *pdt)
{
  asm volatile ("mov %0, %%cr3" :: "r"(pdt));   // put page table addr
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
