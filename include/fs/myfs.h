#ifndef __MYFS__
#define __MYFS__

#include "common.h"

#define CLUSTER_SIZE 512

struct myfs_table{
  u32 *entry_name;
  u8   entry_type;
  u32  offset;
  u32  length;
};

#endif
