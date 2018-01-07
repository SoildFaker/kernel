// Boot loader.
// Part of the boot sector, along with bootasm.S, which calls bootmain().
// bootasm.S has put the processor into protected 32−bit mode.
// bootmain() loads an ELF kernel image from the disk starting at
// sector 3 and then jumps to the kernel entry routine.

#include "common.h"
#include "elf.h"
#include "page.h"

#define SECTSIZE 512

void readseg(u8 *pa, u32 count, u32 offset);
void early_page_init(void);

void bootmain(void)
{
  elf_header_t *elf;
  elf_section_header_t *ph, *eph;
  void (*entry)(void);
  u8 *pa;
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
    pa = (u8 *)ph->paddr;
    readseg(pa, ph->filesz, 2 * SECTSIZE + ph->off);
    if(ph->memsz > ph->filesz)
      stosb(pa + ph->filesz, 0, 2 * SECTSIZE + ph->memsz - ph->filesz);
  }
  // Call the entry point from the ELF header.
  // Does not return!
  entry = (void(*)(void))(elf->entry);
  entry();
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
