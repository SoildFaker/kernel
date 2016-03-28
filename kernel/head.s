.text

extern kern_entry
start_up:
  movl	$65, %al
  int	$0x80
  jmp	start_up
  call	kern_entry


