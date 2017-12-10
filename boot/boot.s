#generate 16-bit code
.code16

.text
.global _start

_start:
  jmp _boot

_boot:
  movw $0x07c0, %ax
  movw %ax, %ds
  lgdt gdt_prt

  cli
  in   $0x92, %al
  or   $0x2, %al
  out  %al, $0x92
  movl %cr0, %eax
  or   $1, %eax
  movl %eax, %cr0

print:
  jmp .

msg: .asciz "Hello, World\r\n"
msg2: .asciz "Protect Mode Now...\r\n"

gdt:
  .word 0,0,0,0 # descriptor 0 deprecated

  # descriptor 1
  .word 0x07ff  # section limit
  .word 0x0000  # section base
  .word 0x9a00  # code scetion read/execute
  .word 0x00c0

  .word 0xb800  # section limit
  .word 0x0000  # section base
  .word 0x9a00  # data scetion read/write
  .word 0x00c0

gdt_len:
  .set GDT_SIZE, . - gdt

gdt_prt: 
  .word GDT_SIZE - 1
  .long 0

.org 510
  .word 0xaa55
