#include "timer.h"
#include "common.h"
#include "tools.h"
#include "task.h"
#include "init.h"

u32 tick = 0;
void timer_callback()
{
  /*tick++;*/
  /*kprint_color(COLOR_BLUE, COLOR_BLACK, "Tick: %u\n", tick);*/
  schedule();
}

void init_timer(u32 frequency)
{
  // 注册时间相关的处理函数
  register_interrupt_handler(IRQ0, (interrupt_handler_t)timer_callback);
  // Intel 8253/8254 芯片PIT I/端口地址范围是O40h~43h
  // 输入频率为， 1193180/frequency 即每秒中断次数
  u32 divisor = 1193180 / frequency;
  
  // D7 D6 D5 D4 D3 D2 D1 D0
  // 0 0 1 1 0 1 1 0
  // 即就是 36 H
  // 设置 8253/8254 芯片工作在模式 3 下
  // 然了，屏蔽中断就没办法了。
  // 完成中断请求和定时器中断 · 61 ·
  outb(0x43, 0x36);
  
  // 拆分低字节和高字节
  u8 low = (u8)(divisor & 0xFF);
  u8 hign = (u8)((divisor >> 8) & 0xFF);
  
  // 分别写入低字节和高字节
  outb(0x40, low);
  outb(0x40, hign);

  irq_enable(0);
}
