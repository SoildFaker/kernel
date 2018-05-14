#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fs/myfs.h"
#include "common.h"

#define SECTOR_SIZE 512
#define ENTRY_NUM   64

#define UP_ROUND(x) ((x) + SECTOR_SIZE - ((x) % SECTOR_SIZE))
#define DOWN_ROUND(x) ((x) - ((x) % SECTOR_SIZE))

struct myfs_struct header;
struct myfs_entry entries[ENTRY_NUM];

int main(int argc, char **argv)
{
    int entry_num = (argc-1)/2;

    // header 
    header.magic = MYFS_MAGIC;
    header.entry_num = entry_num;

    printf("%d", entry_num);
    int sector_count = 0;
    printf("size of entry %d\n", (int)sizeof(struct myfs_entry));
    // put entries at thrid sector
    sector_count += 10;
    int entry_table_length = (sizeof(struct myfs_entry) * 64) / SECTOR_SIZE + 1;
    sector_count += entry_table_length;

    // root directory
    entries[0].entry_type = MYFS_DIR;
    strcpy(entries[0].entry_name, "root");

    int i;
    for(i = 1; i < entry_num; i++)
    {
        strcpy(entries[i].entry_name, argv[i*2+2]);
        entries[i].data_sector = sector_count;
        FILE *stream = fopen(argv[i*2+1], "r");
        if(stream == 0)
        {
            printf("Error: file not found: %s\n", argv[i*2+1]);
            return 1;
        }
        fseek(stream, 0, SEEK_END);
        entries[i].data_size = ftell(stream);
        entries[i].data_count = ftell(stream) / SECTOR_SIZE + 1;
        entries[i].entry_type = MYFS_FILE;
        sector_count += entries[i].data_count;
        printf("writing file %s->%s at sector: %ld count: %ld\n", 
                argv[i*2+1], argv[i*2+2], entries[i].data_sector, entries[i].data_count);
        fclose(stream);
    }

    FILE *wstream = fopen("./kernel.img", "w");
    unsigned char *data = (unsigned char *)malloc(sector_count * SECTOR_SIZE);

    // write header
    fwrite(&header, sizeof(struct myfs_struct), 1, wstream);
    // write entries
    fwrite(entries, sizeof(struct myfs_entry), 64, wstream);

    for(i = 0; i < entry_num; i++)
    {
        FILE *stream = fopen(argv[i*2+1], "r");
        unsigned char *buf = (unsigned char *)malloc(entries[i].data_count * SECTOR_SIZE);
        fread(buf, 1, entries[i].data_count * SECTOR_SIZE, stream);
        // seek to data start sector
        fseek(wstream, entries[i].data_sector * SECTOR_SIZE, SEEK_SET);
        fseek(wstream, -(10 * SECTOR_SIZE), SEEK_CUR);
        fwrite(buf, 1, entries[i].data_count * SECTOR_SIZE, wstream);
        fclose(stream);
        free(buf);
    }

    fclose(wstream);
    free(data);

    printf("writing finished\n");
    return 0;
}
