#ifndef __TASK__
#define __TASK__

#include "common.h"
#include "page.h"

// process state
typedef
enum task_state {
  TASK_UNINIT = 0,    // uninitilized
  TASK_SLEEPING = 1,
  TASK_RUNNABLE = 2,  // runnable or running
  TASK_ZOMBIE = 3,
} task_state;

struct task_env {
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
  page_entry_t *pdt_proc; // process's pdt
};

struct task_ctl {
  u32 pid;
  void *stack;
  volatile task_state state;    // task state
  struct mm_struct *mm;         // task memory space
  struct task_env context;      // task content
  struct task_ctl *task_next;
};

// schedulable process list
extern struct task_ctl *running_proc_head;
extern struct task_ctl *wait_proc_head;
// current process
extern struct task_ctl *current;
// Global pid
extern u32 pid_now;

extern void switch_task(struct task_env *next, struct task_env *current);
u32  kthread_start(u32 (*fn)(void *), void *arg);
void kthread_exit();
void init_task();
void schedule();
void switch_to(struct task_ctl *next);

#endif
