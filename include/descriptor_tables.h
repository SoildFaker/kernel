#include <types.h>

struct gdt_entry_struct{
  u16 segment_limit;
  u16 base_low;
  u8  base_mid;
  u8  access;
  u8  granularity;
  u8  base_high;
}__attribute__((packed)); // no compiler optimization
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct{
  u16 limit;
  u32 base;
}__attribute__((packed)); // no compiler optimization
typedef struct gdt_ptr_struct gdt_ptr_t;
// A struct describing an interrupt gate.
struct idt_entry_struct
{
  u16 base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
  u16 sel;                 // Kernel segment selector.
  u8  always0;             // This must always be zero.
  u8  flags;               // More flags. See documentation.
  u16 base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
  u16 limit;
  u32 base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void gdt_flush(u32);
extern void idt_flush(u32);

void init_descriptor_tables();
