#include "mm.h"
#include "tools.h"
#include "init.h"

mmap_entry_t *mmap = (mmap_entry_t *)0x1000;
u32 *count = (u32 *)0x500;

static u32 *pmm_stack;
static u32 pmm_stack_top = 0;
u32 phy_page_count;

u32 placement_address = (u32)kernel_end;

void show_memory_map()
{
  u8 i=0;
  mmap_entry_t *map_entry = mmap;
  kprint("----------MEMORY MAP----------\n");
  for (i = 0; i < *count; i++){
    kprint("BASE: 0x%09X\tLENGTH: 0x%09X\tTYPE:0x%01X\n", 
      ((map_entry+i)->base_low | (map_entry+i)->base_high<<8), 
      ((map_entry+i)->length_low | (map_entry+i)->length_high<<8), 
      (map_entry+i)->type);
  }
}

void init_pmm()
{
  pmm_stack = (u32 *)kmalloc(sizeof(u32)*(PAGE_MAX_NUM+1));
  mmap_entry_t *map_entry = mmap;

  u8 i;
  for (i = 0; i < *count; i++){
    // 如果是可用内存 ( 按照协议，1 表示可用内存，其它数字指保留区域 )
    if ((map_entry+i)->type == 0x1){
      // 把内核结束位置到结束位置的内存段，按页存储到页管理栈里
      // 最多支持512MB的物理内存
      u32 page_addr = (map_entry+i)->base_low;
      u32 length = (map_entry+i)->base_low + (map_entry+i)-> length_low;

      while (page_addr < length && page_addr <= MEMORY_SIZE) {
        if (page_addr < (u32)kernel_start || page_addr > (u32)kernel_end + 0x1000){
          pmm_free_page(page_addr);
          phy_page_count++;
        }
        page_addr += PAGE_SIZE;
      }
    }
  }
}

u32 pmm_alloc_page()
{
  u32 page = pmm_stack[pmm_stack_top--];
  return page;
}

void pmm_free_page(u32 p)
{
  pmm_stack[++pmm_stack_top] = p;
}

u32 kmalloc_attr(u32 size, u8 align, u32* phys)
{
  if (align == 1){
    placement_address &= PAGE_MASK;
    placement_address += PAGE_SIZE;
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
