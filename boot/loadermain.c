// Boot loader.
// Part of the boot sector, along with bootasm.S, which calls bootmain().
// bootasm.S has put the processor into protected 32−bit mode.
// bootmain() loads an ELF kernel image from the disk starting at
// sector 3 and then jumps to the kernel entry routine.

#include "common.h"
#include "elf.h"
#include "page.h"
#include "drivers/hd.h"
#include "fs/myfs.h"

void bootmain(void)
{
  elf_header_t *elf;
  elf_section_header_t *ph, *eph;
  void (*entry)(void);
  u8 *pa;
  elf = (elf_header_t *)(0x10000); // scratch space
  init_myfs();
  // Read 1st page off disk 8 * 512
  /*struct myfs_entry *tmp = find_file("kernel.elf");*/
  struct myfs_entry *tmp = (struct myfs_entry *)(0x20000);

  u32 elf_sector = tmp[1].data_sector;
  readseg((u8 *)elf, 4096, elf_sector * SECTOR_SIZE);
  // Is this an ELF executable?
  if(elf->magic != ELF_MAGIC) {
    return; // let bootasm.S handle error
  }
  // Load each program segment (ignores ph flags).
  ph = (elf_section_header_t *)((u8*)elf + elf->phoff);
  eph = ph + elf->phnum;
  for(; ph < eph; ph++){
    pa = (u8 *)ph->paddr;
    readseg(pa, ph->filesz, elf_sector * SECTOR_SIZE + ph->off);
    if(ph->memsz > ph->filesz) {
      stosb(pa + ph->filesz, 0, elf_sector * SECTOR_SIZE + (ph->memsz - ph->filesz));
    }
  }
  // Call the entry point from the ELF header.
  // Does not return!
  entry = (void(*)(void))(elf->entry);
  entry();
}
