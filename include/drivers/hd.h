#ifndef __HARDDISK__
#define __HARDDISK__

#include "common.h"

#define SECTOR_SIZE 512

void readseg(u8* pa, u32 count, u32 offset);

#endif
