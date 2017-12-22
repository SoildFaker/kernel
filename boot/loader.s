.code16

start:

  movb $0x13, %al
  movb $0x00, %ah
  int  $0x10

load_kernel:
  movw $0x0000, %dx       # bios int 0x13 read data from disk
  movw $0x0003, %cx       # thrid sector
  movw $0x0900, %ax       # move to 0x00100000 in menory
  movw %ax, %es           # ES:BX point to buffer
  xor  %bx, %bx
  movw $0x0200+34, %ax   # AH function index AL number of sector
  int  $0x13
  jnc  get_memory_map
die: jmp die

# use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
# inputs: es:di -> destination buffer for 24 byte entries
# outputs: bp = entry count, trashes all registers except esi
get_memory_map:
  movw $0, %eax 
  movw %ax, %es
  movw $0x1000, %di 
_loop:
  movw $0xe820, %eax 
  movw $20, %ecx
  movl $0x534d4150, %edx
  int  $0x15 
  jc   _failed 
  add  $20, %di 
  incl (0x0500)
  cmp  $0, %ebx
  jne  _loop
  jmp  protected_mode

_failed:
  movl $0, %es:(%di)

protected_mode:
  lgdt gdt_ptr           # load the new gdt pointer

# prepare to protect move
  movl %cr0, %eax
  or   $0x01, %al
  movl %eax, %cr0

# enable A20 line 
  in  $0x92, %al
  or  $0x02, %al
  out %al, $0x92


  jmp $0x8, $move_kernel

.code32
move_kernel:
  cli                     # interrupt closed
  movw $0x20, %ax         # address copy from selector point to 0x9000
  movw %ax, %ds
  movw $0x18, %ax         # address moved to  selector point to 0x100000
  movw %ax, %es
  movw $0x2000, %cx       # kernel size
  subl %esi, %esi
  subl %edi, %edi
  rep  movsw

  movw $0x10, %ax 
  movw %ax, %ds
  movw %ax, %es
  movw %ax, %fs
  movw %ax, %gs
  jmp $0x8, $0x100000

gdt: 
  .word 0,0,0,0
  .word 0x07ff, 0x0000, 0x9a00, 0x00c0    # 0x08 code segment
  .word 0x07ff, 0x0000, 0x9200, 0x00c0    # 0x10 data segment
  .word 0x07ff, 0x0000, 0x9210, 0x00c0    # 0x18 data segment
  .word 0x07ff, 0x9000, 0x9200, 0x00c0    # 0x20 data segment

gdt_ptr:
  .word 0x7fff, gdt, 0

