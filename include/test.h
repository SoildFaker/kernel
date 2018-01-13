#include "common.h"
#include "tools.h"
#include "task.h"
#include "fs.h"
#include "vfs.h"
#include "syscall.h"

u32 test_a(__UNUSED__ void *arg)
{
  // list the contents of /
  int i = 0;
  struct dirent *node = 0;
  while ( (node = readdir_fs(fs_root, i)) != 0)
  {
    display_print("==============================\n");
    display_print("Found file ");
    display_print(node->name);
    struct fs_node *fsnode = finddir_fs(fs_root, node->name);
    if ((fsnode->flags&0x7) == FS_DIRECTORY){
      display_print("\t(directory)\n");
    } else {
      char buf[256];
      u32 sz = read_fs(fsnode, 0, 256, (u8 *)buf);
      printk("\tlength:%u\n", sz);
      display_print("------------------------------\n");
      printk("%s\n", buf);
    }
    i++;
    display_print("==============================\n");
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


