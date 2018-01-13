#ifndef __ELF___
#define __ELF___

#include "common.h"

// Format of an ELF executable file
#define ELF_MAGIC 0x464C457F // "\x7FELF" in little endian

// File header
struct elf_header {
  u32 magic; // must equal ELF_MAGIC
  u8  elf[12];
  u16 type;
  u16 machine;
  u32 version;
  u32 entry;
  u32 phoff;
  u32 shoff;
  u32 flags;
  u16 ehsize;
  u16 phentsize;
  u16 phnum;
  u16 shentsize;
  u16 shnum;
  u16 shstrndx;
}__attribute__((packed));
typedef struct elf_header elf_header_t;

// Program section header
struct elf_section_header {
  u32 type;
  u32 off;
  u32 vaddr;
  u32 paddr;
  u32 filesz;
  u32 memsz;
  u32 flags;
  u32 align;
}__attribute__((packed));
typedef struct elf_section_header elf_section_header_t;

// Values for Proghdr type
#define ELF_PROG_LOAD 1
// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC    1
#define ELF_PROG_FLAG_WRITE   2
#define ELF_PROG_FLAG_READ    4

void *disk_read_elf(struct elf_header *header, u32 header_offset);
#endif
