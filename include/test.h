#include "common.h"
#include "tools.h"

u32 a = 0;

u32 test_a(__UNUSED__ void *arg)
{
  static u32 t = 0;
  while(1){
    kprint_color(COLOR_RED, COLOR_BLACK, "A:%d\n", t);
    t++;
  }
  return 0;
}

u32 test_c(__UNUSED__ void *arg)
{
  static u32 t = 0;
  while(1){
    kprint_color(COLOR_RED, COLOR_BLACK, "C:%d\n", t);
    t++;
  }
  return 0;
}

u32 test_b(__UNUSED__ void *arg)
{
  static u32 t = 0;
  while(1){
    kprint_color(COLOR_RED, COLOR_BLACK, "B:%d\n", t);
    t++;
  }
  return 0;
}


