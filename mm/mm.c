#include "mm.h"
#include "tools.h"
#include "init.h"

mmap_entry_t *mmap = (mmap_entry_t *)0x1000;
u32 *count = (u32 *)0x500;

u32 placement_address = (u32)kernel_end;

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

u32 kmalloc_attr(u32 size, u8 align, u32* phys)
{
  if (align == 1 && (placement_address & 0xfffff000)){
    placement_address &= 0xfffff000;
    placement_address += 0x1000;
  }
  if(phys){
    *phys = placement_address;
  }
  u32 temp = placement_address;
  placement_address += size;
  return temp;
}

u32 kmalloc_a(u32 size)
{
  return kmalloc_attr(size, 1, 0);
}

u32 kmalloc_p(u32 size, u32 *phys)
{
  return kmalloc_attr(size, 0, phys);
}

u32 kmalloc_ap(u32 size, u32 *phys)
{
  return kmalloc_attr(size, 1, phys);
}

u32 kmalloc(u32 size)
{
  return kmalloc_attr(size, 0, 0);
}
