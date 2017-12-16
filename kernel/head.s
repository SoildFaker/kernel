
start:
  movw $0x18, %ax
  movw %ax, %ss
  jmp $0x8, $kmain

