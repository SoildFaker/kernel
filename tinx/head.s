.code16 

start:
  lgdt gdt_ptr           # load the new gdt pointer
  lidt idt_ptr

  # prepare to protect move
  movl %cr0, %eax
  or   $0x01, %al
  movl %eax, %cr0

  movl $0x10, %eax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  movw %ax, %ss

# switch to protect mode
  jmp  $0x8, $kmain


gdt: 
  .word 0,0,0,0
  .word 0x07ff, 0x0000, 0x9a00, 0x00c0    # code segment
  .word 0x07ff, 0x0000, 0x9200, 0x00c0    # data segment
  
idt_ptr:
  .word 0, 0, 0

gdt_ptr:
  .word 0x7fff, gdt, 0
