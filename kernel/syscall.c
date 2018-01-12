#include "syscall.h"
#include "debug.h"
#include "drivers/display.h"
#include "tools.h"
#include "init.h"

typedef int (*sysc_func) (pt_regs *r);

static void syscall_handler(pt_regs *regs);

static sysc_func syscalls[SYSCALL_NUM];

int sys_print(const char *str)
{
  int a;
  asm volatile("int $0x80" : "=a" (a) : "0" (0), "b"(str));
  return a;
}

int sys_print_color(u8 color, const char *str)
{
  int a;
  asm volatile("int $0x80" : "=a" (a) : "0" (1), "b"(str), "c"(color));
  return a;
}


static int sys_print_color_call(pt_regs *regs)
{
  char *str = (char *)(regs->ebx);
  u8 color = (u8)(regs->ecx);
  display_print_color(COLOR_BLACK, color, str);
  return 0;
}

static int sys_print_call(pt_regs *regs)
{
  char *str = (char *)(regs->ebx);
  display_print(str);
  return 0;
}

int nosys(pt_regs *regs) {
  printk("SYSCALL NO.%d DOSE NOT EXIST.\n", regs->eax);
  return -1;
}

void init_syscall()
{
  // Register our syscall handler.
  register_interrupt_handler (0x80, &syscall_handler);
  syscalls[0] = &sys_print_call;
  syscalls[1] = &sys_print_color_call;
}

void syscall_handler(pt_regs *regs)
{
  u32 ret;
  sysc_func func = 0;

  // Check if the requested syscall number is valid.
  // The syscall number is found in EAX.
  if (regs->eax > SYSCALL_NUM) {
    PANIC("bad syscall");
  }
  // Get the required syscall location.
  func = syscalls[regs->eax];
  if (func == NULL){
    func = &nosys;
  }
  // Enter syscall
  ret = (*func)(regs);
  regs->eax = ret;
}
