#ifndef __PAGING__
#define __PAGING__

#include "common.h"
#include "init.h"
#include "types.h"

#define  PG_PRESENT   0x001
#define  PG_WRITE     0x002
#define  PG_USER      0x004
#define  PG_ACCESSED  0x020
#define  PG_DIRTY     0x040
#define  PG_SIZE      0x080
#define  PAGE_SIZE    0x1000
#define  PAGE_MASK    0xFFFFF000

#define  PDT_COUNT    16
#define  KERNV_START  0x100000
#define  KERNP_START  0x100000

// 获取一个地址的页目录项
#define PGD_INDEX(x) (((x) >> 22) & 0x3FF)
// 获取一个地址的页表项
#define PTE_INDEX(x) (((x) >> 12) & 0x3FF)
// 获取一个地址的页內偏移
#define OFFSET_INDEX(x) ((x) & 0xFFF)

struct page_entry {
  u32 flags  : 9;  // page flags
  u32 avail  : 3;  // available for system
  u32 base   : 20; // base address
};
typedef struct page_entry page_entry_t;

void switch_pgd(u32 pd);
void map(page_entry_t *pgd_now, u32 va, u32 pa, u32 flags);
void unmap(page_entry_t *pgd_now, u32 va);
u32 get_mapping(page_entry_t *pgd_now, u32 va, u32 *pa);
void flush_page_directory(page_entry_t *directory);
void enable_page();
void init_page();
void page_fault(pt_regs *regs);

#endif
