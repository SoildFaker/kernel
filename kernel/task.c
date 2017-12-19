#include "task.h"
#include "tools.h"
#include "common.h"
#include "types.h"

struct task_ctl test_ab[2];
struct task_env test_env_ab[2];
u32 task_stack1[64];
u32 task_stack2[64];
u8 frist = 1;
u8 flag = 0;

void init_task()
{
  test_ab[0].current_env = &test_env_ab[0];
  test_ab[0].current_env->esp = (u32)&task_stack1+sizeof(task_stack1);
  test_ab[0].current_env->eip = (u32)test_a;
  test_ab[0].current_env->eflags = 0x200;
  test_ab[0].task_next = &test_ab[1];

  test_ab[1].current_env = &test_env_ab[1];
  test_ab[1].current_env->esp = (u32)&task_stack2+sizeof(task_stack2);
  test_ab[1].current_env->eip = (u32)test_b;
  test_ab[1].current_env->eflags = 0x200;
  test_ab[1].task_next = &test_ab[0];
  task_now = &test_ab[0];

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
    if (flag == 1){
      display_putc('A',COLOR_GREEN, COLOR_BLACK);
      flag = 0;
    }
    sw++;
  }
}

void test_b()
{
  while(1){
    if (flag == 0){
      display_putc('B',COLOR_RED, COLOR_BLACK);
      flag = 1;
    }
    sw++;
  }
}
