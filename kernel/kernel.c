#include <types.h>
#include <common.h>
#include <descriptor.h>
#include <tools.h>

extern void load_segment();
void kmain(){
  flush_screen();
  init_descriptor_tables();
  
  kprintf("KERNEL LOADED\n");
  kprintf("KERNEL LOADED\n");
  kprintf("KERNEL LOADED\n");
  kprintf("KERNEL LOADED\n");
  kprint_hex(0x00fea788);
  kprintf("\n");
  /*kprint_hex(0x7448);*/
  for(;;);
}
