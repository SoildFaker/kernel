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

.macro IRQ num code
  .short \code
  .short \num
  .global irq\num
  irq\num:
    cli                          # Disable interrupts firstly.
    pushl  $0                    # Push a dummy error code.
    pushl  $\code                # Push the interrupt number.
    jmp irq_common_stub          # Go to our common handler code.
.endm


# This macro creates a stub for an ISR which does NOT pass it's own
# error code (adds a dummy errcode byte).
.macro ISR_NOERRCODE code
  .short \code
  .global isr\code
  isr\code:
    cli                         # Disable interrupts firstly.
    pushl  $0                    # Push a dummy error code.
    pushl  $\code                 # Push the interrupt number.
    jmp isr_common_stub         # Go to our common handler code.
.endm

# This macro creates a stub for an ISR which passes it's own
# error code.
.macro ISR_ERRCODE code
  .short \code
  .global isr\code
  isr\code:
    cli                         # Disable interrupts.
    pushl $\code                 # Push the interrupt number
    jmp isr_common_stub
.endm
# In init.c
.extern isr_handler
.extern irq_handler

# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:

  pusha                    # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

  movw  %ds, %ax             # Lower 16-bits of eax = ds.
  pushl %eax                 # save the data segment descriptor

  movw $0x10, %ax  # load the kernel data segment descriptor
  movw %ax, %ds

  pushl %esp
  call isr_handler
  addl $4, %esp

  popl %ebx        # reload the original data segment descriptor
  movw %bx, %ds

  popa                     # Pops edi,esi,ebp...
  addl $8, %esp    # Cleans up the pushed error code and pushed ISR number
  sti
  iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

# This is our common IRQ stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
irq_common_stub:
  pusha                    # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

  movw  %ds, %ax             # Lower 16-bits of eax = ds.
  pushl %eax                 # save the data segment descriptor

  movw $0x10, %ax  # load the kernel data segment descriptor
  movw %ax, %ds

  pushl %esp
  call irq_handler
  addl $4, %esp

  popl %ebx        # reload the original data segment descriptor
  movw %bx, %ds

  popa                     # Pops edi,esi,ebp...
  addl $8, %esp    # Cleans up the pushed error code and pushed ISR number
  sti
  iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 255


IRQ  0, 32 # 电脑系统计时器
IRQ  1, 33 # 键盘
IRQ  2, 34 # 与 IRQ9 相接，MPU−401 MD 使用
IRQ  3, 35 # 串口设备
IRQ  4, 36 # 串口设备
IRQ  5, 37 # 建议声卡使用
IRQ  6, 38 # 软驱传输控制使用
IRQ  7, 39 # 打印机传输控制使用
IRQ  8, 40 # 即时时钟
IRQ  9, 41 # 与 IRQ2 相接，可设定给其他硬件
IRQ 10, 42 # 建议网卡使用
IRQ 11, 43 # 建议 AGP 显卡使用
IRQ 12, 44 # 接 PS/2 鼠标，也可设定给其他硬件
IRQ 13, 45 # 协处理器使用
IRQ 14, 46 # IDE0 传输控制使用
IRQ 15, 47 # IDE1 传输控制使用


