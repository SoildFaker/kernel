#include "syscall.h"
#include "debug.h"
#include "drivers/display.h"
#include "tools.h"
#include "task.h"
#include "init.h"

typedef int (*sysc_func) (struct trap_frame *r);

extern void int_ret_stub(void);
static void syscall_handler(struct trap_frame *frame);

static sysc_func syscalls[SYSCALL_NUM];

int fork()
{
  int a;
  asm volatile("int $0x80" : "=a" (a) : "0" (1));
  return a;
}

int printf(const char *str, ...)
{
  int a;
  va_list args;
	static char buff[1024];
  va_start(args, str);
	a = vsprintf(buff, str, args);
	va_end(args);
	buff[a] = '\0';
  asm volatile("int $0x80" : "=a" (a) : "0" (0), "b"(buff));
  return a;
}

static int sys_fork(struct trap_frame *frame)
{
  struct task_struct *new_task = alloc_task();
  new_task->context->esp -= FRAME_SIZE;
  new_task->frame = (struct trap_frame *)(new_task->context->esp);
  *new_task->frame = *frame;
  new_task->frame->eax = 0;
  new_task->context->eip = (u32)int_ret_stub;
  new_task->state = RUNNABLE;
  return new_task->pid;
}

static int sys_print(struct trap_frame *frame)
{
  char *str = (char *)(frame->ebx);

	display_print(str);
  return 0;
}

int nosys(struct trap_frame *frame) {
  printk("SYSCALL NO.%d DOSE NOT EXIST.\n", frame->eax);
  return -1;
}

void init_syscall()
{
  // Register our syscall handler.
  register_interrupt_handler (0x80, &syscall_handler);
  syscalls[0] = &sys_print;
  syscalls[1] = &sys_fork;
}

void syscall_handler(struct trap_frame *frame)
{
  u32 ret;
  sysc_func func = 0;

  // Check if the requested syscall number is valid.
  // The syscall number is found in EAX.
  if (frame->eax > SYSCALL_NUM) {
    PANIC("bad syscall");
  }
  // Get the required syscall location.
  func = syscalls[frame->eax];
  if (func == NULL){
    func = &nosys;
  }
  // Enter syscall
  ret = (*func)(frame);
  frame->eax = ret;
}
