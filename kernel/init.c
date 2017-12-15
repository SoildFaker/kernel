#include <types.h>
#include <common.h>
#include <init.h>
#include <string.h>
#include <tools.h>

static void gdt_set_gate(int, u32, u32, u8, u8);
static void idt_set_gate(u8, u32, u16, u8);
static void init_gdt();
static void init_idt();

u32 stack[1280];
gdt_entry_t gdt_entries[8];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;
interrupt_handler_t interrupt_handlers[256];

void init_descriptor_tables()
{
  init_gdt();
  kprint("GDT LOADED\n");
  init_idt();
  kprint("IDT LOADED\n");
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

static void init_8259A()
{
  // 重新映射 IRQ 表
  // 两片级联的 Intel 8259A 芯片
  // 主片端口 0x20 0x21
  // 从片端口 0xA0 0xA1

  // 初始化主片、从片
  // 0001 0001
  outb(0x11, 0x20);
  outb(0x11, 0xA0);
 
  // 设置主片 IRQ 从 0x20(32) 号中断开始
  outb(0x20, 0x21);
 
  // 设置从片 IRQ 从 0x28(40) 号中断开始
  outb(0x28, 0xA1);
 
  // 设置主片 IR2 引脚连接从片
  outb(0x04, 0x21);
 
  // 告诉从片输出引脚和主片 IR2 号相连
  outb(0x02, 0xA1);
 
  // 设置主片和从片按照 8086 的方式工作
  outb(0x01, 0x21);
  outb(0x01, 0xA1);
 
  // 设置主从片允许中断
  outb(0x0, 0x21);
  outb(0x0, 0xA1);
}

static void init_idt()
{
  init_8259A();
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

  // 255 将来用于实现系统调用
  idt_set_gate(255, (u32)isr255, 0x08, 0x8E);
  idt_flush((u32)&idt_ptr);
}

void register_interrupt_handler(u8 n, interrupt_handler_t h)
{
	interrupt_handlers[n] = h;
}
// IRQ 处理函数
/*void irq_handler(u32 esp)*/
void irq_handler(pt_regs *regs)
{
  /*pt_regs *regs = (pt_regs *)((u32)&stack+esp);*/
  // 发送中断结束信号给 PICs
  // 按照我们的设置，从 32 号中断起为用户自定义中断
  // 因为单片的 Intel 8259A 芯片只能处理 8 级中断
  // 故大于等于 40 的中断号是由从片处理的
  if (regs->int_no >= 40) {
  // 发送重设信号给从片
    outb(0x20, 0xA0);
  }
  // 发送重设信号给主片
  outb(0x20, 0x20);
  
  if (interrupt_handlers[regs->int_no]) {
    interrupt_handlers[regs->int_no](regs);
  }
}

/*void isr_handler(u32 esp)*/
void isr_handler(pt_regs *regs)
{
  /*pt_regs *regs = (pt_regs *)((u32)&stack+esp);*/
  display_print_hex(regs->int_no);
  if (interrupt_handlers[regs->int_no]) {
    interrupt_handlers[regs->int_no](regs);
  } else {
    kprint_color(COLOR_BLACK, COLOR_BLUE, "INT: %x\n", regs->int_no);
  }
}

static void init_gdt()
{
  gdt_ptr.limit = sizeof(gdt_entries) - 1;
  gdt_ptr.base  = (u32)&gdt_entries;

  gdt_set_gate(0, 0, 0, 0, 0);
  gdt_set_gate(1, 0, 0xffffffff, A_CR, G_32);
  gdt_set_gate(2, 0, 0xffffffff, A_DRW, G_32);
  gdt_set_gate(3, 0, sizeof(stack), A_DRWA, G_32);
  gdt_set_gate(4, 0, 0xffffffff, A_DPL3|A_CR, G_32);
  gdt_set_gate(5, 0, 0xffffffff, A_DPL3|A_DRW, G_32);
  gdt_set_gate(6, 0, 0xffffffff, A_LDT, G_32);
  gdt_set_gate(7, 0, 0xffffffff, A_DPL3|A_DRW, G_32);
  gdt_flush((u32)&gdt_ptr);

}
  
