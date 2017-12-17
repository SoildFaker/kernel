#generate 16-bit code
.code16

.text
.global _start

_start:

  jmp boot
boot:
  movw %cs, %ax # make segment register point to 0x07c0
  movw %ax, %ds
  movw %ax, %ss
  movw $0x0fff, %sp       # stack pointer prepare for copy kernel

load_disk:
  movw $0x0000, %dx       # bios int 0x13 read data from disk
  movw $0x0002, %cx       # second sector
  movw $0x0800, %ax       # move to 0x00100000 in menory
  movw %ax, %es           # ES:BX point to buffer
  xor  %bx, %bx
  movw $0x0200+1, %ax   # AH function index AL number of sector
  int  $0x13
  jnc  ok_loaded
die:  jmp  die

ok_loaded:
  jmp $0x0000, $0x8000


.org 510
  .word 0xaa55
