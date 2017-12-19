#include "task.h"
#include "tools.h"
#include "common.h"
#include "types.h"

struct task_ctl test_ab[2];
struct task_env test_env_ab[2];
u32 task_stack1[24];
u32 task_stack2[24];

void init_task()
{
  test_ab[0].current_env = &test_env_ab[0];
  test_ab[0].current_env->esp = (u32)&task_stack1+sizeof(task_stack1)-sizeof(u32);
  task_stack1[23]=(u32)test_a;
  test_ab[0].current_env->eflags = 0x200;
  test_ab[0].task_next = &test_ab[1];

  test_ab[1].current_env = &test_env_ab[1];
  test_ab[1].current_env->esp = (u32)&task_stack2+sizeof(task_stack2)-sizeof(u32);
  task_stack2[23]=(u32)test_b;
  test_ab[1].current_env->eflags = 0x200;
  test_ab[1].task_next = &test_ab[0];
  task_now = &test_ab[0];

}

void schedule()
{
  if(&(*task_now->task_next)){
    switch_to(&(*task_now->task_next));
  }
}

void switch_to(struct task_ctl *next)
{
  struct task_env *temp = &(*task_now->current_env);
  task_now = task_now->task_next;
  /*kprint("%x\t%x",&(*next->current_env), &(*temp));*/
  switch_task(&(*next->current_env), &(*temp));
}

void test_a()
{
  while(1){
    kprint_color(COLOR_GREEN, COLOR_BLACK, "A");
  }
}

void test_b()
{
  while(1){
    kprint_color(COLOR_RED, COLOR_BLACK, "B");
  }
}
