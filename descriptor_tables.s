	.file	"descriptor_tables.c"
	.code16gcc
	.comm	gdt_entries,40,32
	.comm	gdt_ptr,6,4
	.comm	idt_entries,2048,32
	.comm	idt_ptr,6,4
	.text
	.globl	init_descriptor_tables
	.type	init_descriptor_tables, @function
init_descriptor_tables:
.LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	call	init_gdt
	call	init_idt
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	init_descriptor_tables, .-init_descriptor_tables
	.globl	gdt_set_gate
	.type	gdt_set_gate, @function
gdt_set_gate:
.LFB7:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movl	20(%ebp), %edx
	movl	24(%ebp), %eax
	movb	%dl, -4(%ebp)
	movb	%al, -8(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movw	%dx, gdt_entries+2(,%eax,8)
	movl	12(%ebp), %eax
	shrl	$16, %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movb	%dl, gdt_entries+4(,%eax,8)
	movl	12(%ebp), %eax
	shrl	$24, %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movb	%dl, gdt_entries+7(,%eax,8)
	movl	16(%ebp), %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movw	%dx, gdt_entries(,%eax,8)
	movl	16(%ebp), %eax
	shrl	$16, %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movb	%dl, gdt_entries+6(,%eax,8)
	movl	8(%ebp), %eax
	movzbl	gdt_entries+6(,%eax,8), %eax
	movl	%eax, %edx
	movzbl	-8(%ebp), %eax
	andl	$-16, %eax
	orl	%edx, %eax
	movl	%eax, %edx
	movl	8(%ebp), %eax
	movb	%dl, gdt_entries+6(,%eax,8)
	movl	8(%ebp), %eax
	movzbl	-4(%ebp), %edx
	movb	%dl, gdt_entries+5(,%eax,8)
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	gdt_set_gate, .-gdt_set_gate
	.globl	idt_set_gate
	.type	idt_set_gate, @function
idt_set_gate:
.LFB8:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$12, %esp
	movl	8(%ebp), %ecx
	movl	16(%ebp), %edx
	movl	20(%ebp), %eax
	movb	%cl, -4(%ebp)
	movw	%dx, -8(%ebp)
	movb	%al, -12(%ebp)
	movzbl	-4(%ebp), %eax
	movl	12(%ebp), %edx
	movw	%dx, idt_entries(,%eax,8)
	movl	12(%ebp), %eax
	shrl	$16, %eax
	movl	%eax, %edx
	movzbl	-4(%ebp), %eax
	movw	%dx, idt_entries+6(,%eax,8)
	movzbl	-4(%ebp), %eax
	movzwl	-8(%ebp), %edx
	movw	%dx, idt_entries+2(,%eax,8)
	movzbl	-4(%ebp), %eax
	movb	$0, idt_entries+4(,%eax,8)
	movzbl	-4(%ebp), %eax
	movzbl	-12(%ebp), %edx
	movb	%dl, idt_entries+5(,%eax,8)
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	idt_set_gate, .-idt_set_gate
	.globl	init_idt
	.type	init_idt, @function
init_idt:
.LFB9:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movw	$2047, idt_ptr
	movl	$idt_entries, %eax
	movl	%eax, idt_ptr+2
	movl	$idt_ptr, %eax
	subl	$12, %esp
	pushl	%eax
	call	idt_flush
	addl	$16, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	init_idt, .-init_idt
	.globl	init_gdt
	.type	init_gdt, @function
init_gdt:
.LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movw	$39, gdt_ptr
	movl	$gdt_entries, %eax
	movl	%eax, gdt_ptr+2
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$0
	call	gdt_set_gate
	addl	$20, %esp
	pushl	$15
	pushl	$154
	pushl	$65535
	pushl	$0
	pushl	$1
	call	gdt_set_gate
	addl	$20, %esp
	pushl	$15
	pushl	$146
	pushl	$65535
	pushl	$0
	pushl	$2
	call	gdt_set_gate
	addl	$20, %esp
	pushl	$15
	pushl	$250
	pushl	$65535
	pushl	$0
	pushl	$3
	call	gdt_set_gate
	addl	$20, %esp
	pushl	$15
	pushl	$242
	pushl	$65535
	pushl	$0
	pushl	$4
	call	gdt_set_gate
	addl	$20, %esp
	movl	$gdt_ptr, %eax
	subl	$12, %esp
	pushl	%eax
	call	gdt_flush
	addl	$16, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	init_gdt, .-init_gdt
	.ident	"GCC: (GNU) 7.2.1 20171128"
	.section	.note.GNU-stack,"",@progbits
