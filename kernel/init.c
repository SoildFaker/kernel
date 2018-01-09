#include "common.h"
#include "init.h"
#include "string.h"
#include "tools.h"

#define GDT_ENTRY_INIT(flags, base, limit) \
	{							\
		.limit0	= (u16) (limit),		\
		.limit1	= ((limit) >> 16) & 0x0F,	\
		.base0	= (u16) (base),			\
		.base1	= ((base) >> 16) & 0xFF,	\
		.base2	= ((base) >> 24) & 0xFF,	\
		.type		= (flags & 0x0f),		\
		.s  		= (flags >> 4) & 0x01,		\
		.dpl		= (flags >> 5) & 0x03,		\
		.p	  	= (flags >> 7) & 0x01,		\
		.avl		= (flags >> 12) & 0x01,		\
		.zero		= (flags >> 13) & 0x01,		\
		.db	  	= (flags >> 14) & 0x01,		\
		.g	  	= (flags >> 15) & 0x01,		\
	}

#define GATE(vector, _segment, _type, _dpl) \
	{						\
		.addr0		= (u16) (vector),		\
		.addr1		= (u16)((vector) >> 16),	\
		.type   	= _type,		\
		.dpl	    = _dpl,			\
		.p		    = 1,  			\
		.segment	= _segment,	\
	}

// Interrupt gate
#define INTG(vector)	\
	GATE(vector, __KERNEL_CS, GATE_INTERRUPT, DPL0)

#define def_isr(x)  extern void isr##x(void)

#define def_irq(x)  extern void irq##x(void)

#define ISR_ENTRY(x)  [x] = INTG((u32)isr##x)

#define IRQ_ENTRY(x)  [IRQ##x] = INTG((u32)irq##x)

def_isr(0);  def_isr(1);  def_isr(2);  def_isr(3);  def_isr(4);  def_isr(5);
def_isr(6);  def_isr(7);  def_isr(8);  def_isr(9);  def_isr(10); def_isr(11);
def_isr(12); def_isr(13); def_isr(14); def_isr(15); def_isr(16); def_isr(17);
def_isr(18); def_isr(19); def_isr(20); def_isr(21); def_isr(22); def_isr(23);
def_isr(24); def_isr(25); def_isr(26); def_isr(27); def_isr(28); def_isr(29);
def_isr(30); def_isr(31); def_isr(255); 

def_irq(0);  def_irq(1);  def_irq(2);  def_irq(3); def_irq(4);  def_irq(5);
def_irq(6);  def_irq(7);  def_irq(8);  def_irq(9); def_irq(10); def_irq(11);
def_irq(12); def_irq(13); def_irq(14); def_irq(15);

struct cpu_struct this_cpu = { 
  .gdt = {
    [GDT_ENTRY_KERNEL_CS] = GDT_ENTRY_INIT(DEC_KERNEL_CS, 0, 0xffffffff),
    [GDT_ENTRY_KERNEL_DS] = GDT_ENTRY_INIT(DEC_KERNEL_DS, 0, 0xffffffff),
    [GDT_ENTRY_USER_CS]   = GDT_ENTRY_INIT(DEC_USER_CS, 0, 0xffffffff),
    [GDT_ENTRY_USER_DS]   = GDT_ENTRY_INIT(DEC_USER_DS, 0, 0xffffffff),
  },
  .gdt_ptr = {
    .limit = GDT_ENTRY_NUM * sizeof(struct gdt_desc_struct),
    .base = (u32)&(this_cpu.gdt),
  },
  .idt = {
    ISR_ENTRY(0), ISR_ENTRY(1), ISR_ENTRY(2), ISR_ENTRY(3), ISR_ENTRY(4), ISR_ENTRY(5),
    ISR_ENTRY(6), ISR_ENTRY(7), ISR_ENTRY(8), ISR_ENTRY(9), ISR_ENTRY(10), ISR_ENTRY(11),
    ISR_ENTRY(12), ISR_ENTRY(13), ISR_ENTRY(14), ISR_ENTRY(15), ISR_ENTRY(16), ISR_ENTRY(17),
    ISR_ENTRY(18), ISR_ENTRY(19), ISR_ENTRY(20), ISR_ENTRY(21), ISR_ENTRY(22), ISR_ENTRY(23),
    ISR_ENTRY(24), ISR_ENTRY(25), ISR_ENTRY(26), ISR_ENTRY(27), ISR_ENTRY(28), ISR_ENTRY(29),
    ISR_ENTRY(30), ISR_ENTRY(31), ISR_ENTRY(255),
    IRQ_ENTRY(0), IRQ_ENTRY(1), IRQ_ENTRY(2), IRQ_ENTRY(3), IRQ_ENTRY(4), IRQ_ENTRY(5),
    IRQ_ENTRY(6), IRQ_ENTRY(7), IRQ_ENTRY(8), IRQ_ENTRY(9), IRQ_ENTRY(10), IRQ_ENTRY(11),
    IRQ_ENTRY(12), IRQ_ENTRY(13), IRQ_ENTRY(14), IRQ_ENTRY(15),
  },
  .idt_ptr = {
    .limit = IDT_ENTRY_NUM * sizeof(struct idt_desc_struct),
    .base = (u32)&(this_cpu.idt),
  },
};

interrupt_handler_t interrupt_handlers[256];

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

void init_desc(void)
{
  gdt_flush((u32)&(this_cpu.gdt_ptr));
  init_8259A();
  irq_enable(2);
  memset((u8 *)&interrupt_handlers , 0, sizeof(interrupt_handlers));
  idt_flush((u32)&(this_cpu.idt_ptr));
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
    printk_color(COLOR_BLUE, COLOR_BLACK, "INT: %d NO HANDLER\n", regs->int_no);
  }
}

