
start:
  cli                     # interrupt closed
  movw $0x20, %ax         # address copy from selector point to 0x9000
  movw %ax, %ds
  movw $0x18, %ax         # address moved to  selector point to 0x100000
  movw %ax, %es
  movw $0x1000, %cx       # kernel size
  subl %esi, %esi
  subl %edi, %edi
  rep  movsw

  movw $0x10, %ax 
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  jmp $0x8, $0x100000

