#generate 16-bit code
.code16

.text
.global _start

_start:
  jmp boot

boot:
  jmp go
go: movw %cs, %ax # make segment register point to 0x07c0
  movw %ax, %ds
  movw %ax, %ss
  movw $0x0fff, %sp       # stack pointer prepare for copy kernel

load_system:
  movw $0x0000, %dx       # bios int 0x13 read data from disk
  movw $0x0002, %cx       # second sector
  movw $0x07e0, %ax       # move to 0x07e0 in menory
  movw %ax, %es           # ES:BX point to buffer
  xor  %bx, %bx
  movw $0x0200+17, %ax   # AH function index AL number of sector
  int  $0x13
  jnc  ok_load
die:  jmp  die

ok_load:
  movw $0x07e0, %ax       # move to 0x07e0 in menory
  movw %ax, %ds
  jmp  $0x07e0, $0x0000

.org 510
  .word 0xaa55
