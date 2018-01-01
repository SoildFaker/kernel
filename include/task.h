#ifndef __TASK__
#define __TASK__

#include "common.h"
#include "page.h"
#include "tty.h"

#define PROC_SIZE 4096

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

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
  page_entry_t *pdt_proc; // process's pdt
};

struct proc {
  volatile u32 pid;
  void *kstack;                 // Bottom of kernel stack of this process
  enum procstate state;         // Process state
  struct mm_struct *mm;         // task memory space
  struct tty *tty;
  struct context *context;      // switch() here to run process
};

struct proc_list {
  struct proc *proc;
  struct proc_list *next;
};

// schedulable process list
extern struct proc_list *running_proc_head;
extern struct proc_list *wait_proc_head;
// current process
extern struct proc *current;
// Global pid
extern u32 pid_now;

extern void switch_task(struct context *next, struct context *current);
u32  kthread_start(u32 (*fn)(void *), struct tty *tty, void *arg);
void kthread_exit(u32 val);
void init_task();
void schedule();
void switch_to(struct proc *next);

#endif
