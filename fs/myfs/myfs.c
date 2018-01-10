#include "fs/myfs.h"
#include "string.h"
#include "drivers/hd.h"

#define TABLE_START_MEM 0x20000
struct myfs_entry *file_entry_table;

void read_file_path(const char *path, u8 *dst)
{
  char tmp_entry_name[16];
  struct myfs_entry *tmp_entry;
  u32 tmp_entry_index = 0;
  u8 i;
READ_PATH:
  i = 0;
  while(*(++path) != '/') {
    if (*(path) == '\0') {
      break;
    }
    if (i < 16) {
      tmp_entry_name[i] = *(path);
      i++;
    }
  }
  tmp_entry = &file_entry_table[tmp_entry_index];
  switch(tmp_entry->entry_type){
    case FS_DIR:
      if (!strcmp((char *)tmp_entry->entry_name, (char *)tmp_entry_name)){
        tmp_entry_index = tmp_entry->child_head_index;
        goto READ_PATH;
      }
      break;
    case FS_FILE:
      if (!strcmp((char *)tmp_entry->entry_name, (char *)tmp_entry_name)){
        readseg(dst, tmp_entry->data_count, tmp_entry->data_sector);
      }
      goto READ_PATH;
      break;
  }
}

struct myfs_entry *
find_file(const char *name)
{
  u8 i;
  for (i = 0; i < 64; i++) {
    if(!strcmp(file_entry_table[i].entry_name, name)) {
      return &file_entry_table[i];
    }
  }
  return &file_entry_table[0];
}

void find_and_read(const char *name, u8 *dst)
{
  struct myfs_entry *tmp = find_file(name);
  readseg(dst, tmp->data_count*SECTOR_SIZE, tmp->data_sector*SECTOR_SIZE);
}

void init_myfs()
{
  file_entry_table = (struct myfs_entry *)(TABLE_START_MEM);
  readseg((u8 *)file_entry_table, sizeof(struct myfs_entry)*64, 10*SECTOR_SIZE);
}
