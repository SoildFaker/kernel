#ifndef __MYFS__
#define __MYFS__

#include "common.h"

#define CLUSTER_SIZE 512

#define FS_DIR  0
#define FS_FILE 1

struct myfs_entry {
  char entry_name[16];
  u8   entry_type;
  u32  child_head_index;
  u32  next_index;
  u32  data_sector;
  u32  data_count;
};

void find_and_read(const char *name, u8 *dst);
struct myfs_entry * find_file(const char *name);
void init_myfs();

#endif
