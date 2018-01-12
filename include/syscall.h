#ifndef __SYSCALL__
#define __SYSCALL__

#include "common.h"
#include "init.h"

#define SYSCALL_NUM 64

void init_syscall();

int sys_print(const char *str);
int sys_print_color(u8 color, const char *str);

#endif
