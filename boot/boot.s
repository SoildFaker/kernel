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

  movw $0x0000, %dx       # bios int 0x13 read data from disk
  movw $0x0003, %cx       # thrid sector
  movw $0x0900, %ax       # move to 0x00100000 in menory
  movw %ax, %es           # ES:BX point to buffer
  xor  %bx, %bx
  movw $0x0200+17, %ax   # AH function index AL number of sector
  int  $0x13
  jnc  ok_loaded
die:  jmp  die

ok_loaded:

  lgdt gdt_ptr           # load the new gdt pointer

# prepare to protect move
  movl %cr0, %eax
  or   $0x01, %al
  movl %eax, %cr0

# enable A20 line 
  in  $0x92, %al
  or  $0x02, %al
  out %al, $0x92


  jmp $0x8, $0x8000

gdt: 
  .word 0,0,0,0
  .word 0x07ff, 0x0000, 0x9a00, 0x00c0    # 0x08 code segment
  .word 0x07ff, 0x0000, 0x9200, 0x00c0    # 0x10 data segment
  .word 0x07ff, 0x0000, 0x9210, 0x00c0    # 0x18 data segment
  .word 0x07ff, 0x9000, 0x9200, 0x00c0    # 0x20 data segment

gdt_ptr:
  .word 0x7fff, gdt, 0

.org 510
  .word 0xaa55
