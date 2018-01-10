#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fs/myfs.h"
#include "common.h"

#define SECTOR_SIZE 512

#define UP_ROUND(x) ((x) + SECTOR_SIZE - ((x) % SECTOR_SIZE))
#define DOWN_ROUND(x) ((x) - ((x) % SECTOR_SIZE))

struct myfs_entry entrys[64];

int main(int argc, char **argv)
{
  int entry_num = (argc-1)/2;
  printf("%d", entry_num);
  int sector_count = 0;
  printf("size of entry %d\n", (int)sizeof(struct myfs_entry));
  // put entrys at thrid sector
  sector_count += 10;
  int entry_table_length = (sizeof(struct myfs_entry) * 64) / SECTOR_SIZE + 1;
  sector_count += entry_table_length;
  int i;
  for(i = 0; i < entry_num; i++)
  {
    strcpy(entrys[i].entry_name, argv[i*2+2]);
    entrys[i].data_sector = sector_count;
    FILE *stream = fopen(argv[i*2+1], "r");
    if(stream == 0)
    {
      printf("Error: file not found: %s\n", argv[i*2+1]);
      return 1;
    }
    fseek(stream, 0, SEEK_END);
    entrys[i].data_count = ftell(stream) / SECTOR_SIZE + 1;
    entrys[i].entry_type = FS_FILE;
    sector_count += entrys[i].data_count;
    printf("writing file %s->%s at sector: %ld count: %ld\n", 
        argv[i*2+1], argv[i*2+2], entrys[i].data_sector, entrys[i].data_count);
    fclose(stream);
  }

  FILE *wstream = fopen("./kernel.img", "w");
  unsigned char *data = (unsigned char *)malloc(sector_count * SECTOR_SIZE);

  // write entrys
  fwrite(entrys, sizeof(struct myfs_entry), 64, wstream);

  for(i = 0; i < entry_num; i++)
  {
    FILE *stream = fopen(argv[i*2+1], "r");
    unsigned char *buf = (unsigned char *)malloc(entrys[i].data_count * SECTOR_SIZE);
    fread(buf, 1, entrys[i].data_count * SECTOR_SIZE, stream);
    // seek to data start sector
    fseek(wstream, entrys[i].data_sector * SECTOR_SIZE, SEEK_SET);
    fseek(wstream, -(10 * SECTOR_SIZE), SEEK_CUR);
    fwrite(buf, 1, entrys[i].data_count * SECTOR_SIZE, wstream);
    fclose(stream);
    free(buf);
  }

  fclose(wstream);
  free(data);

  printf("writing finished\n");
  return 0;
}
