#ifndef __MM__
#define __MM__

#include "types.h"

struct mmap_entry {
	u32 BaseL; // base address uint64_t
	u32 BaseH;
	u32 LengthL; // length uint64_t
	u32 LengthH;
	u32 Type; // entry Type
	//u32 ACPI; // extended
}__attribute__((packed));
typedef struct mmap_entry mmap_entry_t;

void show_memory_map();
#endif
