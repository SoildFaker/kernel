#include "types.h"
#include "init.h"
#include "page.h"
#include "mm.h"
#include "display.h"
#include "tools.h"

page_entry_t *pdt;
static page_entry_t **pet;

u32 page_addr;
u32 phy_addr = 0x0;;

void init_page()
{
  pdt = (page_entry_t *)kmalloc_a(sizeof(page_entry_t)*1024);
  pet = (page_entry_t **)kmalloc_a(sizeof(page_entry_t *)*PDT_COUNT);

  u32 i;
  for (i=0; i<PDT_COUNT; i++){
    pet[i] = (page_entry_t *)kmalloc_a(sizeof(page_entry_t)*1024);

  }
  for(i=0; i<1024;i++){
    phy_addr = 0x000000 + i*0x1000;
    pet[0][i].base = (phy_addr>>12);
    pet[0][i].flags = PG_PRESENT | PG_WRITE;
  }
  for(i=0; i<1024;i++){
    phy_addr = 0x400000 + i*0x1000;
    pet[1][i].base = (phy_addr>>12);
    pet[1][i].flags = PG_PRESENT | PG_WRITE;
  }
  pdt[0].base = (u32)(pet[0])>>12;
  pdt[0].flags = PG_PRESENT | PG_WRITE;
  pdt[1].base = (u32)(pet[1])>>12;
  pdt[1].flags = PG_PRESENT | PG_WRITE;

  /*kprint("%x", (pet[0][0x100]));*/
  register_interrupt_handler(14, (interrupt_handler_t)page_fault);

  flush_page_directory(pdt);
  enable_page();
}

void map(page_entry_t *pdt_now, u32 va, u32 pa, u32 flags)
{
  u32 pdt_idx = PGD_INDEX(va);
  u32 pet_idx = PTE_INDEX(va);
  
  page_entry_t *pet_now = (page_entry_t *)(pdt_now[pdt_idx].base);
  if (!pet_now) {
    pet_now->base = pmm_alloc_page();
    pdt_now[pdt_idx].base = (u32)pet_now;
    pdt_now[pdt_idx].flags = PG_PRESENT | PG_WRITE;
    
    // 转换到内核线性地址并清 0
    pte = (page_entry_t *)((u32)pte + PAGE_OFFSET);
    bzero(pte, PAGE_SIZE);
  } else {
    // 转换到内核线性地址
    pte = (page_entry_t *)((u32)pte + PAGE_OFFSET);
  }
  
  pte[pte_idx] = (pa & PAGE_MASK) | flags;
  
  // 通知 CPU 更新页表缓存
  asm volatile ("invlpg (%0)" : : "a" (va));
}

void unmap(page_entry_t *pgd_now, u32 va)
{
  u32 pgd_idx = PGD_INDEX(va);
  u32 pte_idx = PTE_INDEX(va);
  
  page_entry_t *pte = (page_entry_t *)(pgd_now[pgd_idx] & PAGE_MASK);
  
  if (!pte) {
    return;
  }
  
  // 转换到内核线性地址
  pte = (page_entry_t *)((u32)pte + PAGE_OFFSET);
  
  pte[pte_idx] = 0;
  
  // 通知 CPU 更新页表缓存
  asm volatile ("invlpg (%0)" : : "a" (va));
}

u32 get_mapping(page_entry_t *pgd_now, u32 va, u32 *pa)
{
  u32 pgd_idx = PGD_INDEX(va);
  u32 pte_idx = PTE_INDEX(va);
  
  page_entry_t *pte = (page_entry_t *)(pgd_now[pgd_idx] & PAGE_MASK);
  if (!pte) {
    return 0;
  }

  // 转换到内核线性地址
  pte = (pte_t *)((u32)pte + PAGE_OFFSET);
  
  // 如果地址有效而且指针不为，则返回地址NULL
  if (pte[pte_idx] != 0 && pa) {
    *pa = pte[pte_idx] & PAGE_MASK;
    return 1;
  }

  return 0;
}

void switch_pgd(u32 pd)
{
  asm volatile ("mov %0, %%cr3" : : "r" (pd));
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
