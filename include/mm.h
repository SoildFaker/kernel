#ifndef __MM__
#define __MM__

#include "common.h"
#include "page.h"

// stack size of thread
#define STACK_SIZE      8192
#define HEAP_START      0xe0000000
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
  struct memory_header *next;
  struct memory_header *prev;
  u32 allocated :1;
  u32 length    :31;
};
typedef struct memory_header memory_header_t;

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

// 内存申请
void *kmalloc(u32 len);

// 内存释放
void kfree(void *p);
#endif
