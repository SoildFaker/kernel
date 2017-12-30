#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct fs {
  unsigned char magic; // The magic number is there to check for consistency.
  char name[128];
  unsigned long offset;
  unsigned long length;
} fs_t;

int main(int argc, char **argv)
{
  int nheaders = (argc-1)/2;
  fs_t headers[64];
  printf("size of header: %d\n", (int)sizeof(fs_t));
  unsigned int off = sizeof(fs_t) * 64 + sizeof(int);
  int i;
  for(i = 0; i < nheaders; i++)
  {
    printf("writing file %s->%s at 0x%x\n", argv[i*2+1], argv[i*2+2], off);
    strcpy(headers[i].name, argv[i*2+2]);
    headers[i].offset = off;
    FILE *stream = fopen(argv[i*2+1], "r");
    if(stream == 0)
    {
      printf("Error: file not found: %s\n", argv[i*2+1]);
      return 1;
    }
    fseek(stream, 0, SEEK_END);
    headers[i].length = ftell(stream);
    off += headers[i].length;
    fclose(stream);
    headers[i].magic = 0xBF;
  }

  FILE *wstream = fopen("./floppy.img", "w");
  unsigned char *data = (unsigned char *)malloc(off);
  fwrite(&nheaders, sizeof(int), 1, wstream);
  fwrite(headers, sizeof(fs_t), 64, wstream);

  for(i = 0; i < nheaders; i++)
  {
    FILE *stream = fopen(argv[i*2+1], "r");
    unsigned char *buf = (unsigned char *)malloc(headers[i].length);
    fread(buf, 1, headers[i].length, stream);
    fwrite(buf, 1, headers[i].length, wstream);
    fclose(stream);
    free(buf);
  }

  fclose(wstream);
  free(data);

  return 0;
}
