#include "mm.h"
#include "tools.h"
#include "page.h"
#include "init.h"

u32 kernel_stack[KERNEL_STACK_SIZE];
mmap_entry_t *mmap = (mmap_entry_t *)(0x1000 + PAGE_OFFSET);
u32 *count = (u32 *)(0x500 + PAGE_OFFSET);

memory_header_t *heap_first = 0;
static void alloc_chunk(u32 start, u32 len);
static void free_chunk(memory_header_t *chunk);
static void split_chunk(memory_header_t *chunk, u32 len);
static void glue_chunk(memory_header_t *chunk);

static u32 heap_max = HEAP_START;

static u32 pmm_stack[PAGE_MAX_NUM];
static u32 pmm_stack_top = 0;
u32 phy_page_count;

void init_pmm()
{
  mmap_entry_t *map_entry = mmap;

  u8 i;
  for (i = 1; i < *count; i++){
    // If this memory section are usable
    if ((map_entry+i)->type == 0x1){
      // store these usable memory page to page mangement stack
      u32 page_addr = (map_entry+i)->base_low;
      u32 length = (map_entry+i)->base_low + (map_entry+i)-> length_low;

      while (page_addr < length && page_addr <= MEMORY_SIZE) {
        if (page_addr < (u32)kernel_start_pos || page_addr > (u32)kernel_end_pos + 0x1000){
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

void *kmalloc(u32 len)
{
  len += sizeof(memory_header_t);
  memory_header_t *header_cur = heap_first;
  memory_header_t *header_prev = 0;

  while (header_cur){
    if (header_cur->allocated == 0 && header_cur->length >= len){
      // Split memory by length
      split_chunk(header_cur, len);
      header_cur->allocated = 1;
      // return memory address memory header is ahead of memory chunk
      return (void *)((u32)header_cur + sizeof(memory_header_t));
    }
    header_prev = header_cur;
    header_cur = header_cur->next;
  }

  u32 chunk_start;

  if (header_prev){
    chunk_start = (u32)header_prev + header_prev->length;
  } else {
    chunk_start = HEAP_START;
    heap_first = (memory_header_t *)chunk_start;
  }
  // need for a new page?
  alloc_chunk(chunk_start , len);
  header_cur = (memory_header_t *)chunk_start;
  header_cur->prev = header_prev;
  header_cur->next = 0;
  header_cur->allocated = 1;
  header_cur->length = len;
  
  if (header_prev) {
    header_prev->next = header_cur;
  }
  
  return (void*)(chunk_start + sizeof(memory_header_t));
}

void kfree(void *p)
{
  // relesse header and reset allocated flag
  memory_header_t *header = (memory_header_t*)((u32)p - sizeof(memory_header_t));
  header->allocated = 0;
  
  // glue memory chunk
  glue_chunk(header);
}

void alloc_chunk(u32 start, u32 len)
{
  // 如果当前堆的位置已经到达界限则申请内存页
  // 必须循环申请内存页直到有到足够的可用内存
  while (start + len > heap_max) {
    u32 page = pmm_alloc_page();
    map(pdt_kernel, heap_max, page, PG_PRESENT | PG_WRITE);
    heap_max += PAGE_SIZE;
  }
}

void free_chunk(memory_header_t *chunk)
{
  if (chunk->prev == 0) {
    heap_first = 0;
  } else {
  chunk->prev->next = 0;
  }
  
  // 空闲的内存超过 1 页的话就释放掉
  while ((heap_max - PAGE_SIZE) >= (u32)chunk) {
    heap_max -= PAGE_SIZE;
    u32 page;
    get_mapping(pdt_kernel, heap_max, &page);
    unmap(pdt_kernel, heap_max);
    pmm_free_page(page);
  }
}

void split_chunk(memory_header_t *chunk, u32 len)
{
// 切分内存块之前得保证之后的剩余内存至少容纳一个内存管理块的大小
  if (chunk->length - len > sizeof (memory_header_t)) {
    memory_header_t *newchunk = (memory_header_t *)((u32)chunk + chunk->length);
    newchunk->prev = chunk;
    newchunk->next = chunk->next;
    newchunk->allocated = 0;
    newchunk->length = chunk->length - len;
    
    chunk->next = newchunk;
    chunk->length = len;
  }
}

void glue_chunk(memory_header_t *chunk)
{
  // 如果该内存块前面有链内存块且未被使用则拼合
  if (chunk->next && chunk->next->allocated == 0) {
    chunk->length = chunk->length + chunk->next->length;
    if (chunk->next->next) {
      chunk->next->next->prev = chunk;
    }
    chunk->next = chunk->next->next;
  }
  
  if (chunk->prev && chunk->prev->allocated == 0) {
    chunk->prev->length = chunk->prev->length + chunk->length;
    chunk->prev->next = chunk->next;
    if (chunk->next) {
      chunk->next->prev = chunk->prev;
    }
    chunk = chunk->prev;
  }
    
  // 假如该内存后面没有链表内存块了直接释放掉
  if (chunk->next == 0) {
    free_chunk(chunk);
  }
}
