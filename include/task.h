#ifndef __TASK__
#define __TASK__

#include "types.h"

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

struct task_ctl {
  struct task_env *current_env;
  struct task_ctl *task_next;
};

struct task_ctl *task_now;
extern void switch_task(struct task_env *next, struct task_env *current);
void init_task();
void schedule();
void switch_to(struct task_ctl *next);
void test_a();
void test_b();
#endif
