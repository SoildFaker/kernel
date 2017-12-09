#generate 16-bit code
.code16

.text
.global _start

_start:
  jmp _boot
  msg: .asciz "Hello, World\r\n"

  .macro putStr str
    leaw  \str, %si
    call  putChar
  .endm

  putChar:
    lodsb
    orb %al, %al
    jz  out
    movb $0x0e, %ah
    int  $0x10
    jmp putChar
  out:
  ret
  
_boot:
  putStr msg

  . = _start + 510    #mov to 510th byte from 0 pos
  .byte 0x55           #append boot signature
  .byte 0xaa           #append boot signature
