.org 0x7c00
  movw    %cs, %ax
  movw    %ax, %ds
  movw    %ax, %es
  call    DispStr
  jmp     .
DispStr:
  movw    $msg, %ax
  movw    %ax, %bp
  movw    $len, %cx
  movw    $0x1301, %ax
  movw    $0x000c, %bx
  movb    $0, %dl
  int     $0x10
  ret
  msg : .string "Hello Worls!\\n"
  len = . - msg
.fill 510-(.-$0x7c00),1,0
.long 0xaa55

