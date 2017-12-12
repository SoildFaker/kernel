.code16
.global gdt_flush

gdt_flush:
  movl 4(%esp), %eax    # give a parameter (gdt_table entry address)
  lgdt (%eax)           # load the new gdt pointer
  movw $0x10, %ax       # 0x10 is the offset in the GDT to our data segment
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  movw %ax, %ss

  jmp  flush
flush:
  ret

.global idt_flush

idt_flush:
 movl 4(%esp), %eax
 lidt (%eax)
 ret

.globl switch_protect

switch_protect:
  movw $0x0001, %ax
  lmsw %ax 
  ret
  
