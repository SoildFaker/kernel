#include "task.h"
#include "mm.h"
#include "tools.h"
#include "string.h"
#include "common.h"

u32 pid_now = 0;

struct proc_list *running_proc_head = NULL;
struct proc_list *wait_proc_head = NULL;
struct proc *current = NULL;

void init_task()
{
  running_proc_head = (struct proc_list *)kmalloc(sizeof(struct proc_list));
  // current is kernel thread
  current = (struct proc *)kmalloc(sizeof(struct proc));
  current->context = (struct context *)kmalloc(sizeof(struct context));

  current->state = RUNNABLE;
  current->kstack = &kstack;
  current->pid = pid_now++; // kernel pid is 0
  current->mm = NULL;       // do not need this for kernel
  current->tty = tty_cur;

  running_proc_head->proc = current;
  running_proc_head->next = running_proc_head;

}

void schedule()
{
  if (running_proc_head->next->proc->state == RUNNABLE){
    running_proc_head = running_proc_head->next;
    switch_to(running_proc_head->proc);
  }
}

void switch_to(struct proc *next)
{
  if (current != next) {
    struct proc *prev = current;
    current = next;
    prev->state = RUNNABLE;
    next->state = RUNNING;
    tty_print = next->tty;
    switch_task(prev->context, current->context);
  }
}

// Create kernel process
u32 kthread_start(u32 (*fn)(void *), struct tty *tty, void *arg)
{
  struct proc *new_task = (struct proc *)kmalloc(sizeof(struct proc));
  new_task->context = (struct context *)kmalloc(sizeof(struct context));
  u32 *pstack = (u32 *)kmalloc(STACK_SIZE);
  assert(new_task != NULL, "kern_thread: kmalloc error");

  new_task->state = RUNNABLE;
  new_task->kstack = kstack;
  new_task->pid = pid_now++;
  new_task->mm = NULL;
  new_task->tty = tty;

  u32 *stack_top = (u32 *)((u32)pstack + STACK_SIZE);

  *(--stack_top) = (u32)arg;
  *(--stack_top) = (u32)kthread_exit;
  /**(--stack_top) = (u32)fn;*/

  new_task->context->esp = (u32)stack_top;
  new_task->context->ebp = (u32)pstack;
  new_task->context->eip = (u32)fn;
  // let task's eflags = 0x2000 (enable interrupt)
  new_task->context->eflags = 0x200;

  // insert new task to tasklist's tail
  struct proc_list *tail = running_proc_head;
  assert(tail != NULL, "Must init sched!");

  while (tail->next != running_proc_head) {
    tail = tail->next;
  }
  tail->next = (struct proc_list *)kmalloc(sizeof(struct proc_list));
  tail->next->proc = new_task;
  tail->next->next = running_proc_head;

  return new_task->pid;
}

void kthread_exit(u32 val)
{
  /*u32 val; asm ("movl %%eax, %0\n" :"=m"(val));*/
  kprint("Thread exited with value %d\n", val);

  while (1);
}

void switch_to_user_mode()
{
  // Set up a stack structure for switching to user mode.
  asm volatile("  \
      cli; \
      mov $0x23, %ax; \
      mov %ax, %ds; \
      mov %ax, %es; \
      mov %ax, %fs; \
      mov %ax, %gs; \
      \
      mov %esp, %eax; \
      pushl $0x23; \
      pushl %eax; \
      pushf; \
      pushl $0x1B; \
      push $1f; \
      iret; \
      1: \
      ");
}
