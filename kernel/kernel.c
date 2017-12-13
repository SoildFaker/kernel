#include <types.h>
#include <common.h>
#include <descriptor_tables.h>
#include <tools.h>

void kmain(){
  init_descriptor_tables();
  
  write_string(COLOR_BLACK, COLOR_GREEN, "OK------LOADED");
  for(;;);
}
