#include <tinx/types.h>
#include <boot/gdt.h>

void set_gdt(
  struct gdt *entry,
  u32 base,
  u32 limit,
  u8  access,
  u8  granularity
  )
{
  entry->base_low = base & 0xffff;
  entry->base_mid = (base >> 16) & 0xff;
  entry->base_high = (base >> 24) & 0xff;

  entry->segment_limit = limit;
  entry->granularity = (limit >> 16) & 0xff;
  entry->granularity |= granularity & 0xf0;
  entry->access = access;
}
