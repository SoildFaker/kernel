.code16
.global gdt_flush

gdt_flush:
  movl 4(%esp), %eax    # give a parameter (gdt_table entry address)
  lgdt (%eax)           # load the new gdt pointer
  ret

.global idt_flush

idt_flush:
 movl 4(%esp), %eax
 lidt (%eax)
 ret

.globl switch_protect

switch_protect:
  movl %cr0, %eax
  or   $0x01, %al
  movl %eax, %cr0
  jmp  $0x8, $pm

.code32
pm:
  #lmsw %ax 
  ret
  
