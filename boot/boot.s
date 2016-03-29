	org		07c00h
	mov		ax,cs
	mov		es,ax
	mov		ds,ax
	call	dispstr

dispstr:
	mov		ax,msg1
	mov		bp,ax
	mov		cx,12
	mov		ax,01301h
	mov		bx,000ch
	mov		dl,0
	int		10h
	ret

msg1:		db		"hello world!"
times	510 - ($-$$)	db	0
dw		0xaa55
