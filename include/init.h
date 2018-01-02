#ifndef __INIT__
#define __INIT__

#include "common.h"

#define KERN_CODE 0x08
#define KERN_DATA 0x10
#define USER_CODE 0x18
#define USER_DATA 0x20

#define G_32    0xcf
#define G_16    0x0f

#define A_DPL0    0x00
#define A_DPL1    0x20
#define A_DPL2    0x40
#define A_DPL3    0x60

#define A_LDT         0x82
#define A_TaskGate    0x85
#define A_386TSS      0x89
#define A_386CGATE    0x8c
#define A_386IGATE    0x8e
#define A_386TGATE    0x8f

#define A_DR    0x90
#define A_DRW   0x92
#define A_DRWA  0x93
#define A_C     0x98
#define A_CR    0x9a
#define A_CCO   0x9c
#define A_CCOR  0x9e

// 定义IRQ
#define IRQ0 32 // 电脑系统计时器
#define IRQ1 33 // 键盘
#define IRQ2 34 // 与 IRQ9 相接，MPU−401 MD 使用
#define IRQ3 35 // 串口设备
#define IRQ4 36 // 串口设备
#define IRQ5 37 // 建议声卡使用
#define IRQ6 38 // 软驱传输控制使用
#define IRQ7 39 // 打印机传输控制使用
#define IRQ8 40 // 即时时钟
#define IRQ9 41 // 与 IRQ2 相接，可设定给其他硬件
#define IRQ10 42 // 建议网卡使用
#define IRQ11 43 // 建议 AGP 显卡使用
#define IRQ12 44 // 接 PS/2 鼠标，也可设定给其他硬件
#define IRQ13 45 // 协处理器使用
#define IRQ14 46 // IDE0 传输控制使用
#define IRQ15 47 // IDE1 传输控制使用
// 声明中断处理函数 0 ~ 19 属于 CPU 的异常中断
// ISR中断服务程序:(interrupt service routine)
extern void isr0(); // 0 #DE 除 0 异常
extern void isr1(); // 1 #DB 调试异常
extern void isr2(); // 2 NMI
extern void isr3(); // 3 BP 断点异常
extern void isr4(); // 4 #OF 溢出
extern void isr5(); // 5 #BR 对数组的引用超出边界
extern void isr6(); // 6 #UD 无效或未定义的操作码
extern void isr7(); // 7 #NM 设备不可用无数学协处理器()
extern void isr8(); // 8 #DF 双重故障有错误代码()
extern void isr9(); // 9 协处理器跨段操作
extern void isr10(); // 10 #TS 无效TSS有错误代码()
extern void isr11(); // 11 #NP 段不存在有错误代码()
extern void isr12(); // 12 #SS 栈错误有错误代码()
extern void isr13(); // 13 #GP 常规保护有错误代码()
extern void isr14(); // 14 #PF 页故障有错误代码()
extern void isr15(); // 15 CPU 保留
extern void isr16(); // 16 #MF 浮点处理单元错误
extern void isr17(); // 17 #AC 对齐检查
extern void isr18(); // 18 #MC 机器检查
extern void isr19(); // 19 #XM SIMD单指令多数据()浮点异常

// 20 ~ 31 Intel 保留
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// 32 ~ 255 用户自定义异常
extern void isr255();

// IRQ中断请求:(Interrupt Request)
extern void irq0(); // 电脑系统计时器
extern void irq1(); // 键盘
extern void irq2(); // 与 IRQ9 相接，MPU−401 MD 使用
extern void irq3(); // 串口设备
extern void irq4(); // 串口设备
extern void irq5(); // 建议声卡使用
extern void irq6(); // 软驱传输控制使用
extern void irq7(); // 打印机传输控制使用
extern void irq8(); // 即时时钟
extern void irq9(); // 与 IRQ2 相接，可设定给其他硬件
extern void irq10(); // 建议网卡使用
extern void irq11(); // 建议 AGP 显卡使用
extern void irq12(); // 接 PS/2 鼠标，也可设定给其他硬件
extern void irq13(); // 协处理器使用
extern void irq14(); // IDE0 传输控制使用
extern void irq15(); // IDE1 传输控制使用

// A struct describing a Task State Segment.
struct tss_entry_struct
{
  u32 prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
  u32 esp0;       // The stack pointer to load when we change to kernel mode.
  u32 ss0;        // The stack segment to load when we change to kernel mode.
  u32 esp1;       // Unused...
  u32 ss1;
  u32 esp2;
  u32 ss2;
  u32 cr3;
  u32 eip;
  u32 eflags;
  u32 eax;
  u32 ecx;
  u32 edx;
  u32 ebx;
  u32 esp;
  u32 ebp;
  u32 esi;
  u32 edi;
  u32 es;         // The value to load into ES when we change to kernel mode.
  u32 cs;         // The value to load into CS when we change to kernel mode.
  u32 ss;         // The value to load into SS when we change to kernel mode.
  u32 ds;         // The value to load into DS when we change to kernel mode.
  u32 fs;         // The value to load into FS when we change to kernel mode.
  u32 gs;         // The value to load into GS when we change to kernel mode.
  u32 ldt;        // Unused...
  u16 trap;
  u16 iomap_base;
} __attribute__((packed));
typedef struct tss_entry_struct tss_entry_t;

struct pt_regs_t {
  u32 gs, fs, es, ds;                  // Data segment selector
  u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
  u32 int_no, err_code;    // Interrupt number and error code (if applicable)
  u32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
};
typedef struct pt_regs_t pt_regs;

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

typedef void (*interrupt_handler_t)(pt_regs *);

struct call_gate_struct
{
  u16 offset_low;
  u16 selector;
  u8  param_count;
  u8  access;
  u16 offset_high;
};
typedef struct call_gate_struct call_gate_t;

extern u32 kstack[2048];
// These extern directives let us access the addresses of our ASM ISR handlers.
extern void gdt_flush(u32);
extern void idt_flush(u32);

void irq_eoi(u32 nr);
void irq_enable(u8 irq);
void register_interrupt_handler(u8 n, interrupt_handler_t h);
void irq_handler(pt_regs *regs);
void init_gdt();
void init_idt();
void register_interrupt_handler(u8 n, interrupt_handler_t h);

#endif
