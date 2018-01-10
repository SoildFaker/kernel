#ifndef __MM__
#define __MM__

#include "common.h"
#include "page.h"

// stack size of thread
#define STACK_SIZE      2048
#define HEAP_START      0xE0000000
// max memory
#define MEMORY_SIZE     0x04000000
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

extern mmap_entry_t *mmap;
extern u32 *count;

extern u32 kernel_stack[STACK_SIZE];
extern u8 kernel_start_pos[];
extern u8 kernel_end_pos[];

// memoey page count
extern u32 phy_page_count;
extern mmap_entry_t *mmap;
extern u32 *count;

void init_pmm();
void pmm_free_page(u32 p);
u32 pmm_alloc_page();

void *kmalloc(u32 len);
void kfree(void *p);

#endif
