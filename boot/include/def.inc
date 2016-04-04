; Descriptor Type
DA_32			equ			4000h	; 32bits segment

DA_DPL0			equ			00h
DA_DPL1			equ			20h
DA_DPL2			equ			40h
DA_DPL3			equ			60h

DA_DRW			equ			92h		; R/W segment
DA_DRWA			equ			93h		; R/W/A Segment
DA_C			equ			98h		; executable

DA_LDT			equ			82h
DA_TaskGate		equ			85h
DA_386TSS		equ			89h
DA_386CGate		equ			8ch
DA_386IGate		equ			8eh
DA_386TGate		equ			8fh


SA_TIL			equ			4

SA_RPL0			equ			0
SA_RPL1			equ			1
SA_RPL2			equ			2
SA_RPL3			equ			3
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

; Gate
; usage: Gate Selector, Offset, Dcount, Attr
;			Selector:	dw
;			Offset:		dd
;			Dcount:		db
;			Attr:		db
%macro Gate 4
	  dw	(%2 & 0ffffh)
	  dw	%1
	  dw	(%3 & 1fh) | ((%4 << 8) & 0ff00h)
	  dw	((%2 >> 16) & 0ffffh)
%endmacro
