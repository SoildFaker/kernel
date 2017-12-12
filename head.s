	.file	"head.c"
	.code16gcc
#APP
	.code16gcc

	jmp $0x0000, $kmain

#NO_APP
	.text
	.globl	printStr
	.type	printStr, @function
printStr:
.LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	jmp	.L2
.L3:
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	orb	$14, %ah
	movl	$7, %edx
	movl	%edx, %ebx
#APP
# 10 "tinx/head.c" 1
	int $0x10
# 0 "" 2
#NO_APP
	addl	$1, 8(%ebp)
.L2:
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L3
	nop
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	printStr, .-printStr
	.globl	write_string
	.type	write_string, @function
write_string:
.LFB7:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	$753664, -4(%ebp)
	jmp	.L5
.L6:
	movl	12(%ebp), %edx
	leal	1(%edx), %eax
	movl	%eax, 12(%ebp)
	movl	-4(%ebp), %eax
	leal	1(%eax), %ecx
	movl	%ecx, -4(%ebp)
	movzbl	(%edx), %edx
	movb	%dl, (%eax)
	movl	-4(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -4(%ebp)
	movl	8(%ebp), %edx
	movb	%dl, (%eax)
.L5:
	movl	12(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L6
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	write_string, .-write_string
	.section	.rodata
.LC0:
	.string	"OK"
	.text
	.globl	kmain
	.type	kmain, @function
kmain:
.LFB8:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	call	init_descriptor_tables
	call	switch_protect
	subl	$8, %esp
	pushl	$.LC0
	pushl	$32
	call	write_string
	addl	$16, %esp
.L8:
	jmp	.L8
	.cfi_endproc
.LFE8:
	.size	kmain, .-kmain
	.ident	"GCC: (GNU) 7.2.1 20171128"
	.section	.note.GNU-stack,"",@progbits
