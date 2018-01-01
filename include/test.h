#include "common.h"
#include "tools.h"

u32 sw,a = 0;

u32 test_a(__UNUSED__ void *arg)
{
  while(1){
    if (sw == 0){
      display_print_color("A", COLOR_RED, COLOR_BLACK);
      sw = 1;
    }
    a++;
  }
  return 0;
}

u32 test_c(__UNUSED__ void *arg)
{
  while(1){
    if (sw == 2){
      display_print_color("C", COLOR_WHITE, COLOR_BLACK);
      sw = 0;
    }
    a++;
  }
  return 0;
}


u32 test_b(__UNUSED__ void *arg)
{
  while(1){
    if (sw == 1){
      display_print_color("B", COLOR_BLUE, COLOR_BLACK);
      sw = 2;
    }
    a++;
  }
  return 0;
}


