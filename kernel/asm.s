.code32

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

.global load_segment
load_segment:
# load segment descriptor
  movw $0x10, %ax
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  movw $0x18, %ax
  movw %ax, %ss
  ret
