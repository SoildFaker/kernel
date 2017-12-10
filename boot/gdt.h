#include <tinx/types.h>

struct gdt{
  u16 segment_limit;
  u16 base_low;
  u8  base_mid;
  u8  access;
  u8  granularity;
  u8  base_high;
}__attribute__((packed)); // no compiler optimization

struct gdt_ptr{
  u16 gdt_limit;
  u32 gdt_base;
}__attribute__((packed)); // no compiler optimization

void set_gdt(
  struct gdt *entry,
  u32 base,
  u32 limit,
  u8  access,
  u8  granularity
  );
