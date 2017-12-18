#ifndef __PAGING__
#define __PAGING__

#include "common.h"
#include "types.h"

#define  PT_PRESENT 0x01

struct page_entry {
  u32 flags  : 9;  // page flags
  u32 avail  : 3;  // available for system
  u32 base   : 20; // base address
};
typedef struct page_entry page_entry_t;

struct page_table {
  page_entry_t pages[1024];
}__attribute__((aligned(4096)));
typedef struct page_table page_table_t;

struct page_directory {
  page_table_t page_tables[1024];
}__attribute__((aligned(4096)));
typedef struct page_directory page_directory_t;

void flush_page_directory(page_entry_t* directory);
void enable_page();
void init_page();
#endif
