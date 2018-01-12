#include "common.h"
#include "tools.h"
#include "task.h"
#include "syscall.h"

u32 a = 0;

u32 test_a(__UNUSED__ void *arg)
{
  static u32 t = 0;
  switch_to_user_mode();
  while(1){
    sys_print("User");
    //printk_color(COLOR_RED, COLOR_BLACK, "A:%d\n", t);
    t++;
  }
  return 0;
}

u32 test_c(__UNUSED__ void *arg)
{
  static u32 t = 0;
  while(1){
    printk_color(COLOR_RED, COLOR_BLACK, "C:%d\n", t);
    t++;
  }
  return 0;
}

u32 test_b(__UNUSED__ void *arg)
{
  static u32 t = 0;
  while(1){
    printk_color(COLOR_RED, COLOR_BLACK, "B:%d\n", t);
    t++;
  }
  return 0;
}


