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

