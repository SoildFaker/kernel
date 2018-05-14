.code16                 # Assemble for 16−bit mode

start:
    cli                   # Disable BIOS interrupts
# Zero data segment registers DS, ES, and SS.
    xorw %ax, %ax # Set %ax to zero
    movw %ax, %ds # −> Data Segment
    movw %ax, %es # −> Extra Segment
    movw %ax, %ss # −> Stack Segment

protected_mode:

# enable A20 line 
    in  $0x92, %al
    or  $0x02, %al
    out %al, $0x92

    lgdt gdt_ptr         # load the new gdt pointer

# Switch from real to protected mode. Use a bootstrap GDT that makes
# virtual addresses map directly to physical addresses so that the
# effective memory map doesn’t change during the transition.
# prepare to protect move
    movl %cr0, %eax
    or   $0x01, %al
    movl %eax, %cr0

# Complete transition to 32−bit protected mode by using long jmp
# to reload %cs and %eip. The segment descriptors are set up with no
# translation, so that the mapping is still the identity mapping.
    jmp $0x8, $start_32

.code32
start_32:
    movw $0x10, %ax 
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw %ax, %fs
    movw %ax, %gs

# Set up the stack pointer and call into C.
    movl $start, %esp
    call loadermain

# If bootmain returns (it shouldn’t), trigger a Bochs
# breakpoint if running under Bochs, then loop.
    movw $0x8a00, %ax # 0x8a00 −> port 0x8a00
    movw %ax, %dx
    outw %ax, %dx
    movw $0x8ae0, %ax # 0x8ae0 −> port 0x8a00
    outw %ax, %dx
spin:
    jmp spin

gdt:
    .word 0,0,0,0
    .word 0xffff, 0x0000, 0x9a00, 0x00cf    # 0x08 code segment
    .word 0xffff, 0x0000, 0x9200, 0x00cf    # 0x10 data segment

    .p2align 2           # force 4 byte alignment
gdt_ptr:
    .word (gdt_ptr - gdt - 1)
    .long gdt
