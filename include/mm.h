#ifndef __MM__
#define __MM__

#include "types.h"
#include "page.h"

// stack size of thread
#define STACK_SIZE      8192
// max memory
#define MEMORY_SIZE     0x08000000
#define PAGE_MAX_NUM    (MEMORY_SIZE/PAGE_SIZE)

struct mmap_entry {
  u32 base_low;
  u32 base_high;
  u32 length_low;
  u32 length_high;
  u32 type;
}__attribute__((packed));
typedef struct mmap_entry mmap_entry_t;

struct memory_header {
  u8 used;
  struct memory_header *next;
};

extern u8 kernel_start[];
extern u8 kernel_end[];

// 动态分配物理内存页的总数
extern u32 phy_page_count;
extern mmap_entry_t *mmap;
extern u32 *count;

// 初始化物理内存管理
void init_pmm();
void show_memory_map();
void pmm_free_page(u32 p);
u32 pmm_alloc_page();

u32 kmalloc_attr(u32 size, u8 align, u32* phys);
u32 kmalloc_a(u32 size);
u32 kmalloc_p(u32 size, u32 *phys);
u32 kmalloc_ap(u32 size, u32 *phys);
u32 kmalloc(u32 size);

#endif
