#ifndef VFS_H
#define VFS_H

#include "common.h"

#define RAMDISK_SIZE (1024*1024*4)      // 4MiB

extern struct myfs_struct *header;
extern struct myfs_entry *entries;

struct fs_node *init_vfs();

#endif
