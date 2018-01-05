// Boot loader.
// Part of the boot sector, along with bootasm.S, which calls bootmain().
// bootasm.S has put the processor into protected 32−bit mode.
// bootmain() loads an ELF kernel image from the disk starting at
// sector 3 and then jumps to the kernel entry routine.

#include "common.h"
#include "elf.h"
#include "page.h"

#define SECTSIZE 512

// Temp page directory table
u32 *pdt_kernel = (u32 *)(0x2000);
u32 *pet_kernel = (u32 *)(0x3000);
u32 *pet_loader = (u32 *)(0x4000);
void readseg(u8 *pa, u32 count, u32 offset);
void early_page_init(void);

void bootmain(void)
{
  elf_header_t *elf;
  elf_section_header_t *ph, *eph;
  void (*entry)(void);
  u8 *pa;
  early_page_init();
  elf = (elf_header_t *)(0x10000); // scratch space
  // Read 1st page off disk
  readseg((u8 *)elf, 4096, 2 * SECTSIZE);
  // Is this an ELF executable?
  if(elf->magic != ELF_MAGIC)
    return; // let bootasm.S handle error
  // Load each program segment (ignores ph flags).
  ph = (elf_section_header_t *)((u8*)elf + elf->phoff);
  eph = ph + elf->phnum;
  for(; ph < eph; ph++){
    pa = (u8 *)ph->vaddr;
    readseg(pa, ph->filesz, 2 * SECTSIZE + ph->off);
    if(ph->memsz > ph->filesz)
      stosb(pa + ph->filesz, 0, 2 * SECTSIZE + ph->memsz - ph->filesz);
  }
  // Call the entry point from the ELF header.
  // Does not return!
  entry = (void(*)(void))(elf->entry);
  entry();
}

// Set kernel virtual address start from PAGE_OFFSET
void early_page_init(void)
{
  pdt_kernel[0] = (u32)pet_loader | PG_WRITE | PG_PRESENT;
  pdt_kernel[PDT_INDEX(PAGE_OFFSET)] = (u32)pet_kernel | PG_WRITE | PG_PRESENT;
  u32 i;
  for (i=0; i<1024; i++) {
    pet_loader[i] = (i << 12) | PG_WRITE | PG_PRESENT;
  }
  for (i=0; i<1024; i++) {
    pet_kernel[i] = (i << 12) | PG_WRITE | PG_PRESENT;
  }
  asm volatile ("mov %0, %%cr3" :: "r"(pdt_kernel));   // put page table addr
  u32 cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000;                        // enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void waitdisk(void)
{
  // Wait for disk ready.
  while((inb(0x1F7) & 0xC0) != 0x40) ;
}

// Read a single sector at offset into dst.
void readsect(void *dst, u32 offset)
{
  // Issue command.
  waitdisk();
  outb(0x1F2, 1); // count = 1
  outb(0x1F3, offset);
  outb(0x1F4, offset >> 8);
  outb(0x1F5, offset >> 16);
  outb(0x1F6, (offset >> 24) | 0xE0);
  outb(0x1F7, 0x20); // cmd 0x20 − read sectors
  // Read data.
  waitdisk();
  insl(0x1F0, dst, SECTSIZE/4);
}

// Read ’count’ bytes at ’offset’ from kernel into physical address ’pa’.
// Might copy more than asked.
void readseg(u8* pa, u32 count, u32 offset)
{
  u8* epa;
  epa = pa + count;
  // Round down to sector boundary.
  pa -= offset % SECTSIZE;
  // Translate from bytes to sectors; kernel starts at sector 1.
  offset = (offset / SECTSIZE) + 1;
  // If this is too slow, we could read lots of sectors at a time.
  // We’d write more to memory than asked, but it doesn’t matter −−
  // we load in increasing order.
  for(; pa < epa; pa += SECTSIZE, offset++)
    readsect(pa, offset);
}
