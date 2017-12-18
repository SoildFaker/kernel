#include "mm.h"
#include "tools.h"
#include "init.h"

mmap_entry_t *mmap = (mmap_entry_t *)0x1000;
u32 *count = (u32 *)0x500;

void show_memory_map()
{
  u8 i=0;
  kprint("----------MEMORY MAP----------\n");
  for (i=0;i<*count;i++){
    kprint("BASE: 0x%09X\tLENGTH: 0x%09X\tTYPE:0x%01X\n", 
      (mmap->BaseL| mmap->BaseH<<8), 
      (mmap->LengthL|mmap->LengthH<<8), 
      mmap->Type);
    mmap++;
  }

}

