#ifndef __SYSCALL__
#define __SYSCALL__

#include "common.h"
#include "init.h"

#define SYSCALL_NUM 64

void init_syscall();

int printf(const char *str, ...);
int fork();

#endif
