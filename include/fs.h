#ifndef __FS__
#define __FS__

#include "common.h"

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08 // Is the file an active mountpoint?

typedef u32 (*read_type_t)(struct fs_node*,u32,u32,u8*);
typedef u32 (*write_type_t)(struct fs_node*,u32,u32,u8*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent * (*readdir_type_t)(struct fs_node*,u32);
typedef struct fs_node * (*finddir_type_t)(struct fs_node*,char *name);

typedef struct fs_node {
  char name[128]; // The filename
  u32 mask;       // The permissions mask
  u32 uid;        // The owner's id
  u32 gid;        // The owning group
  u32 flags;      // Includes the node's type
  u32 inode;      // This is device-specific - provided a way for a fs to indentify files
  u32 length;     // Size of the file by byte
  u32 impl;       // An implementation-defined number
  read_type_t read;
  write_type_t write;
  open_type_t open;
  close_type_t close;
  readdir_type_t readdir;
  finddir_type_t finddir;
  struct fs_node *ptr; // used by mountpoints and symlinks
} fs_node_t;

// One of these is returned by the readdir call, according to POSIX.
struct dirent
{
  char name[128]; // Filename.
  u32 ino;        // Inode number. Required by POSIX.
};

extern fs_node_t *fs_root; // The root of the filesystem.

// Standard read/write/open/close functions. Note that these are all suffixed with
// _fs to distinguish them from the read/write/open/close which deal with file descriptors
// not file nodes.
u32 read_fs(fs_node_t *node, u32 offset, u32 size, u8 *buffer);
u32 write_fs(fs_node_t *node, u32 offset, u32 size, u8 *buffer);
void open_fs(fs_node_t *node, u8 read, u8 write);
void close_fs(fs_node_t *node);
struct dirent *readdir_fs(fs_node_t *node, u32 index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);

#endif
