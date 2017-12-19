#ifndef __PAGING__
#define __PAGING__

#include "common.h"
#include "init.h"
#include "types.h"

#define  PT_PRESENT 0x01
#define  PT_READWRITE 0x02

#define PAGE_SIZE 4096

struct page_entry {
  u32 flags  : 9;  // page flags
  u32 avail  : 3;  // available for system
  u32 base   : 20; // base address
};
typedef struct page_entry page_entry_t;

__attribute__((aligned(PAGE_SIZE))) page_entry_t pdt[1024];
__attribute__((aligned(PAGE_SIZE))) page_entry_t pet[1024];

void flush_page_directory(page_entry_t *directory);
void enable_page();
void init_page();
void page_fault(pt_regs *regs);

#endif
