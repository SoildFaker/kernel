#include "task.h"
#include "mm.h"
#include "tools.h"
#include "common.h"
#include "types.h"

u8 frist = 1;

u8 *flag;
void init_task()
{
  flag = (u8 *)kmalloc(sizeof(u8));;
  struct task_ctl *task_a = (struct task_ctl *)kmalloc(sizeof(struct task_ctl));
  struct task_ctl *task_b = (struct task_ctl *)kmalloc(sizeof(struct task_ctl));
  struct task_env *task_a_env = (struct task_env *)kmalloc(sizeof(struct task_env));
  struct task_env *task_b_env = (struct task_env *)kmalloc(sizeof(struct task_env));
  u32 *task_stack1 = (u32 *)kmalloc(sizeof(u32)*1024);
  u32 *task_stack2 = (u32 *)kmalloc(sizeof(u32)*1024);

  task_a->current_env = task_a_env;
  task_a->current_env->ecx = 0;
  task_a->current_env->esp = (u32)task_stack1+sizeof(u32)*1024;
  task_a->current_env->eip = (u32)test_a;
  task_a->current_env->eflags = 0x200;
  task_a->task_next = task_b;

  /**task_b->current_env = 0;*/
  task_b->current_env = task_b_env;
  task_b->current_env->esp = (u32)task_stack2+sizeof(u32)*1024;
  task_b->current_env->eip = (u32)test_b;
  task_b->current_env->eflags = 0x200;
  task_b->task_next = task_a;
  task_now = task_a;

}

extern void load_test_a(struct task_env *_test_a);
void schedule()
{
  if (frist){
    frist = 0;
    load_test_a(&(*task_now->task_next->current_env));
  }else{
    if(&(*task_now->task_next)){
      switch_to(&(*task_now->task_next));
    }
  }
}

void switch_to(struct task_ctl *next)
{
  struct task_env *temp = &(*task_now->current_env);
  task_now = task_now->task_next;
  /*kprint("%x\t%x",&(*next->current_env), &(*temp));*/
  switch_task(&(*next->current_env), &(*temp));
}
u32 sw = 0;
void test_a()
{
  while(1){
    if (*flag == 1){
      display_putc('A',COLOR_GREEN, COLOR_BLACK);
      *flag = 0;
    }
    sw++;
  }
}

void test_b()
{
  while(1){
    if (*flag == 0){
      display_putc('B',COLOR_RED, COLOR_BLACK);
      *flag = 1;
    }
    sw++;
  }
}
