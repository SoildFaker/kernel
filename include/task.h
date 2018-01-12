#ifndef __TASK__
#define __TASK__

#include "common.h"
#include "page.h"
#include "drivers/tty.h"

#define PROC_SIZE 4096

enum taskstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct context {
 	u32 eip;
 	u32 esp;
	u32 ebp;
	u32 ebx;
	u32 ecx;
	u32 edx;
	u32 esi;
	u32 edi;
	u32 eflags;
}__attribute__((packed));

struct mm_struct {
  page_entry_t *pdt_task; // taskess's pdt
};

struct task {
  u32 pid;
  volatile u32 time_slice;
  u8 priority;
  void *kernel_stack;                 // Bottom of kernel stack of this taskess
  enum taskstate state;         // Process state
  struct mm_struct *mm;         // task memory space
  struct tty *tty;
  struct context *context;      // switch() here to run taskess
};

struct task_list {
  struct task *task;
  struct task_list *next;
};

// schedulable taskess list
extern struct task_list *running_task_head;
extern struct task_list *wait_task_head;
// current taskess
extern struct task *current;
// Global pid
extern u32 pid_now;

extern void switch_task(struct context *next, struct context *current);

void switch_to_user_mode();
u32  kthread_start(u32 (*fn)(void *), struct tty *tty, u8 priority, void *arg);
void kthread_exit(u32 val);
void init_task();
void schedule();
void switch_to(struct task *next);
u32 task_idle(void *arg);

#endif
