#ifndef __TASK__
#define __TASK__

#include "common.h"
#include "page.h"
#include "drivers/tty.h"

#define PROC_SIZE 4096

enum task_state { UNUSED, NEW, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct task_context {
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

struct task_struct {
  u32 pid;
  volatile u32 time_slice;
  u8 priority;
  void *kernel_stack;
  void *user_stack;
  enum task_state state;
  struct mm_struct *mm;         // task memory space
  struct tty *tty;
  struct task_context *context;      // switch() here to run taskess
  struct trap_frame *frame;
  struct task_struct *parent;
};

struct task_list {
  struct task_struct *task;
  struct task_list *next;
};

// schedulable taskess list
extern struct task_list *running_task_head;
extern struct task_list *wait_task_head;
// current taskess
extern struct task_struct *current;
// Global pid
extern u32 pid_now;

extern void switch_task(struct task_context *next, struct task_context *current);

void switch_to_user_mode();
struct task_struct *alloc_task();
u32  kthread_start(u32 (*fn)(void *), struct tty *tty, u8 priority, void *arg);
void kthread_exit();
void init_task();
void schedule();
void switch_to(struct task_struct *next);
u32 task_idle(void *arg);

#endif
