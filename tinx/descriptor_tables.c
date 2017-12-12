#include <tinx/types.h>
#include <tinx/common.h>
#include <tinx/descriptor_tables.h>

asm(".code16gcc\n");

static void gdt_set_gate(int, u32, u32, u8, u8);
static void idt_set_gate(u8, u32, u16, u8);
static void init_gdt();
static void init_idt();

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

void init_descriptor_tables()
{
  init_gdt();
  init_idt();
}

static void gdt_set_gate(
  int num,
  u32 base,
  u32 limit,
  u8  access,
  u8  granularity
  )
{
  gdt_entries[num].base_low = base & 0xffff;
  gdt_entries[num].base_mid = (base >> 16) & 0xff;
  gdt_entries[num].base_high = (base >> 24) & 0xff;

  gdt_entries[num].segment_limit = limit;
  gdt_entries[num].granularity = (limit >> 16) & 0x0f;
  gdt_entries[num].granularity |= granularity & 0xf0;
  gdt_entries[num].access = access;
}

static void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags)
{
  idt_entries[num].base_lo = base & 0xFFFF;
  idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

  idt_entries[num].sel     = sel;
  idt_entries[num].always0 = 0;
  // We must uncomment the OR below when we get to using user-mode.
  // It sets the interrupt gate's privilege level to 3.
  idt_entries[num].flags   = flags /* | 0x60 */;
}

static void init_idt()
{
  idt_ptr.limit = (sizeof(idt_entry_t) * 256) -1;
  idt_ptr.base  = (u32)&idt_entries;

  /*memset(&idt_entries, 0, sizeof(idt_entry_t)*256);*/

  idt_set_gate( 0, (u32)0 , 0x08, 0x8E);
  /*idt_set_gate( 1, (u32)isr1 , 0x08, 0x8E);*/
  /*idt_set_gate(31, (u32)isr32, 0x08, 0x8E);*/

  idt_flush((u32)&idt_ptr);
}


static void init_gdt(){

  gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
  gdt_ptr.base  = (u32)&gdt_entries;

  gdt_set_gate(0, 0, 0, 0, 0);
  gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf);
  gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf);
  gdt_set_gate(3, 0, 0xffffffff, 0xfa, 0xcf);
  gdt_set_gate(4, 0, 0xffffffff, 0xf2, 0xcf);
  gdt_flush((u32)&gdt_ptr);

}
  
