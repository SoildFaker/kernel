#include "common.h"
#include "init.h"
#include "string.h"
#include "tools.h"

static void gdt_set_gate(int, u32, u32, u8, u8);
static void idt_set_gate(u8, u32, u16, u8);

u32 kstack[2048];
gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;
interrupt_handler_t interrupt_handlers[256];

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

static void init_8259A()
{
  // remap IRQ table
  // master port 0x20 0x21
  // slave port 0xA0 0xA1

  // init 8259A
  // 0001 0001
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
 
  // set master chip int code start from 0x20(30)
  outb(0x21, 0x20);
 
  // set slave chip int code start from 0x28(40)
  outb(0xA1, 0x28);
 
  // master chip IR2 pin connected to slave chip
  outb(0x21, 0x04);
 
  // salve chip out pin connected master chip IR2
  outb(0xA1, 0x02);
 
  // work mode 8086
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
 
  // stop int
  outb(0x21, 0xFF);
  outb(0xA1, 0xFF);
}

void init_gdt()
{
  gdt_ptr.limit = sizeof(gdt_entries) - 1;
  gdt_ptr.base  = (u32)&gdt_entries;

  gdt_set_gate(0, 0, 0, 0, 0);
  gdt_set_gate(1, 0, 0xffffffff, A_CR, G_32);
  gdt_set_gate(2, 0, 0xffffffff, A_DRW, G_32);
  gdt_set_gate(3, 0, 0xffffffff, A_DPL3|A_CR, G_32);
  gdt_set_gate(4, 0, 0xffffffff, A_DPL3|A_DRW, G_32);

  gdt_flush((u32)&gdt_ptr);

}
 
void init_idt()
{
  init_8259A();
  irq_enable(2);
  idt_ptr.limit = sizeof(idt_entries) - 1;
  idt_ptr.base  = (u32)&idt_entries;

  memset((u8 *)&idt_entries, 0, sizeof(idt_entries));
  memset((u8 *)&interrupt_handlers , 0, sizeof(interrupt_handlers));

  idt_set_gate(0, (u32)isr0, 0x08, 0x8E);
  idt_set_gate(1, (u32)isr1, 0x08, 0x8E);
  idt_set_gate(2, (u32)isr2, 0x08, 0x8E);
  idt_set_gate(3, (u32)isr3, 0x08, 0x8E);
  idt_set_gate(4, (u32)isr4, 0x08, 0x8E);
  idt_set_gate(5, (u32)isr5, 0x08, 0x8E);
  idt_set_gate(6, (u32)isr6, 0x08, 0x8E);
  idt_set_gate(7, (u32)isr7, 0x08, 0x8E);
  idt_set_gate(8, (u32)isr8, 0x08, 0x8E);
  idt_set_gate(9, (u32)isr9, 0x08, 0x8E);
  idt_set_gate(10, (u32)isr10, 0x08, 0x8E);
  idt_set_gate(11, (u32)isr11, 0x08, 0x8E);
  idt_set_gate(12, (u32)isr12, 0x08, 0x8E);
  idt_set_gate(13, (u32)isr13, 0x08, 0x8E);
  idt_set_gate(14, (u32)isr14, 0x08, 0x8E);
  idt_set_gate(15, (u32)isr15, 0x08, 0x8E);
  idt_set_gate(16, (u32)isr16, 0x08, 0x8E);
  idt_set_gate(17, (u32)isr17, 0x08, 0x8E);
  idt_set_gate(18, (u32)isr18, 0x08, 0x8E);
  idt_set_gate(19, (u32)isr19, 0x08, 0x8E);
  idt_set_gate(20, (u32)isr20, 0x08, 0x8E);
  idt_set_gate(21, (u32)isr21, 0x08, 0x8E);
  idt_set_gate(22, (u32)isr22, 0x08, 0x8E);
  idt_set_gate(23, (u32)isr23, 0x08, 0x8E);
  idt_set_gate(24, (u32)isr24, 0x08, 0x8E);
  idt_set_gate(25, (u32)isr25, 0x08, 0x8E);
  idt_set_gate(26, (u32)isr26, 0x08, 0x8E);
  idt_set_gate(27, (u32)isr27, 0x08, 0x8E);
  idt_set_gate(28, (u32)isr28, 0x08, 0x8E);
  idt_set_gate(29, (u32)isr29, 0x08, 0x8E);
  idt_set_gate(30, (u32)isr30, 0x08, 0x8E);
  idt_set_gate(31, (u32)isr31, 0x08, 0x8E);
  idt_set_gate(32, (u32)irq0, 0x08, 0x8E);
  idt_set_gate(33, (u32)irq1, 0x08, 0x8E);
  idt_set_gate(34, (u32)irq2, 0x08, 0x8E);
  idt_set_gate(35, (u32)irq3, 0x08, 0x8E);
  idt_set_gate(36, (u32)irq4, 0x08, 0x8E);
  idt_set_gate(37, (u32)irq5, 0x08, 0x8E);
  idt_set_gate(38, (u32)irq6, 0x08, 0x8E);
  idt_set_gate(39, (u32)irq7, 0x08, 0x8E);
  idt_set_gate(40, (u32)irq8, 0x08, 0x8E);
  idt_set_gate(41, (u32)irq9, 0x08, 0x8E);
  idt_set_gate(42, (u32)irq10, 0x08, 0x8E);
  idt_set_gate(43, (u32)irq11, 0x08, 0x8E);
  idt_set_gate(44, (u32)irq12, 0x08, 0x8E);
  idt_set_gate(45, (u32)irq13, 0x08, 0x8E);
  idt_set_gate(46, (u32)irq14, 0x08, 0x8E);
  idt_set_gate(47, (u32)irq15, 0x08, 0x8E);

  // no.255 for syscall
  idt_set_gate(255, (u32)isr255, 0x08, 0x8E);
  idt_flush((u32)&idt_ptr);
}

void register_interrupt_handler(u8 n, interrupt_handler_t h)
{
	interrupt_handlers[n] = h;
}

// send interrupt reset to PICs
// for Intel 8259A chip every chip handled 8 level of interrupt
// so int code over 40 handled by slave chip
void irq_eoi(u32 nr)
{
  // send reset signal to master chip
  outb(0x20, 0x20);
  if(nr >= 40) {
    // send reset to slave chip
    outb(0xA0, 0x20);
  }
}

void irq_enable(u8 irq)
{
  u16 irq_mask = (inb(0xA1)<<8) + inb(0x21);
  irq_mask &= ~(1<<irq);
  outb(0x21, irq_mask);
  outb(0xA1, irq_mask >> 8);
}

void irq_handler(pt_regs *regs)
{
  interrupt_handler_t handler = interrupt_handlers[regs->int_no];
  if (regs->int_no >= 32) {
    irq_eoi(regs->int_no);
  }
  if(handler){
    handler(regs);
  }else{
    kprint_color(COLOR_BLUE, COLOR_BLACK, "INT: %d NO HANDLER\n", regs->int_no);
  }
}
 
