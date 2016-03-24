! boot.s startup file

BOOTSEG	equ	0x07c0

[BITS 32]

section .init.text

[GLOBAL	start]

start:
		jmpi	go,BOOTSEG
go:		mov		ax,cs
		mov		ds,ax
		mov		es,ax
		mov		[msg1+17],ah
		mov		cx,#20
		mov		dx,#0x1004
		mov		bx,#0x000c
		mov		bp,#msg1
		mov		ax,#0x1301
		int		0x10
loop0:	jmp		loop0
msg1:	.ascii	"loading system ..."
		.byte	13,10
.org	510
		.word	0xAA55

