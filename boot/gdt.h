#include <tinx/types.h>

struct GDT{
  u16 segment_limit;
  u16 base_low;
  u8  base_mid;
  u8  access;
  u8  granularity;
  u8  base_high;
}__attribute__((packed)); // no compiler optimization

struct GDT_PTR{
  u16 limit;
  u32 base;
}__attribute__((packed)); // no compiler optimization

void set_gdt( struct GDT *entry, u32 base, u32 limit, u8  access, u8  granularity);
void load_gdt(struct GDT_PTR ptr);
