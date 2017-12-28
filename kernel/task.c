#include "task.h"
#include "mm.h"
#include "tools.h"
#include "string.h"
#include "common.h"

u32 pid_now = 0;

struct task_ctl *running_proc_head = NULL;
struct task_ctl *wait_proc_head = NULL;
struct task_ctl *current = NULL;

void init_task()
{
  // current is kernel thread
  current = (struct task_ctl *)kmalloc(sizeof(struct task_ctl));

  current->state = TASK_RUNNABLE;
  current->stack = &kstack;
  current->pid = pid_now++; // kernel pid is 0
  current->mm = NULL;       // do not need this for kernel

  current->task_next = current;
  /*running_proc_head = current;*/

}

void schedule()
{
  if (current){
    /*kprint("-------------\n");*/
    /*kprint("%x\n", (current->context.eip));*/
    /*kprint("%x\n", (current->task_next->context.eip));*/
    switch_to(current->task_next);
  }
}

void switch_to(struct task_ctl *next)
{
  if (current != next) {
    struct task_ctl *prev = current;
    current = next;
    switch_task(&(prev->context), &(current->context));
  }
}

// Create kernel process
u32 kthread_start(u32 (*fn)(void *), void *arg)
{
  struct task_ctl *new_task = (struct task_ctl *)kmalloc(sizeof(struct task_ctl));
  u32 *pstack = (u32 *)kmalloc(STACK_SIZE);
  /*assert(new_task != NULL, "kern_thread: kmalloc error");*/

  // 将栈低端结构信息初始化为 0
  /*memset(new_task, 0, sizeof(struct task_ctl));*/

  new_task->state = TASK_RUNNABLE;
  new_task->stack = pstack;
  new_task->pid = pid_now++;
  new_task->mm = NULL;

  u32 *stack_top = (u32 *)((u32)pstack + STACK_SIZE);

  *(--stack_top) = (u32)arg;
  *(--stack_top) = (u32)kthread_exit;
  *(--stack_top) = (u32)fn;

  new_task->context.esp = (u32)(stack_top - sizeof(u32)*3);
  new_task->context.eip = (u32)fn;
  new_task->context.ecx = 0;
  // 设置新任务的标志寄存器未屏蔽中断，很重要
  new_task->context.eflags = 0x200;

  new_task->task_next = current;

  /*if (current == NULL){*/
    /*new_task->task_next = new_task;*/
    /*[>running_proc_head = new_task;<]*/
    /*current = new_task;*/
  /*}*/

  // 找到当前进任务队列，插入到末尾
  /*struct task_ctl *tail = running_proc_head;*/
  struct task_ctl *tail = current;
  /*assert(tail != NULL, "Must init sched!");*/

  /*while (tail->task_next != running_proc_head) {*/
  while (tail->task_next != current) {
    tail = tail->task_next;
  }
  tail->task_next = new_task;

  return new_task->pid;
}

void kthread_exit()
{
  while (1);
}


