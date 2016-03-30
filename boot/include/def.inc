; Descriptor Type
DA_32			equ			4000h	; 32bits segment

DA_DRW			equ			92h		; R/W segment
DA_C			equ			98h		; executable
; Descriptor
; usage: Descriptor Base, Limit, Attr
;		 Base: dd
;		 Limit: dd (low 20 bits available)
;		 Attr: dw (lower 4 bits of higher byte are always 0)
%macro Descriptor 3
		dw	%2 & 0FFFFh								; Limit1
		dw	%1 & 0FFFFh								; Base1
		db	(%1 >> 16) & 0FFh						; Base2
		dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)		; Attr1 + Limit2 +Attr2
		db	(%1 >> 24) & 0FFh						; Base3
%endmacro
