%include	"include/def.inc"
org		0100h
		jmp		LABEL_BEGIN

[SECTION .gdt]
; GDT
;			
LABEL_GDT:			Descriptor	0,			0,				0
LABEL_DESC_NORMAL:	Descriptor	0,			0ffffh,			DA_DRW
LABEL_DESC_CODE16:	Descriptor	0,			0ffffh,			DA_C
LABEL_DESC_CODE32:	Descriptor	0,			Seg32Len -1,	DA_C + DA_32
LABEL_DESC_DATA:	Descriptor	0,			DataLen - 1,	DA_DRW + DA_DPL1
LABEL_DESC_STACK:	Descriptor	0,			TopOfStack,		DA_DRWA + DA_32
LABEL_DESC_LDT:		Descriptor	0,			LDTLen - 1,		DA_LDT
LABEL_DESC_TEST:	Descriptor	0500000h,	0ffffh,			DA_DRW
LABEL_DESC_VIDEO:	Descriptor	0B8000h,	0FFFFh,			DA_DRW + DA_DPL3
LABEL_DESC_CODE_DEST:	Descriptor 0,		SegCodeDestLen - 1,	DA_C + DA_32
LABEL_CALL_GATE_TEST:	Gate	SelectorCodeDest,	0,		0,		DA_386CGate + DA_DPL3
LABEL_DESC_CODE_RING3:	Descriptor 0,		SegCodeRing3 - 1,	DA_C + DA_32 + DA_DPL3
LABEL_DESC_STACK3:	Descriptor	0,			TopOfStack3,	DA_DRWA + DA_32 + DA_DPL3
LABEL_DESC_TSS:		Descriptor	0,			TSSLen - 1,		DA_386TSS


GdtLen		equ		$ - LABEL_GDT
GdtPtr		dw		GdtLen - 1
			dd		0

SelectorNormal		equ		LABEL_DESC_NORMAL - LABEL_GDT
SelectorCode16		equ		LABEL_DESC_CODE16 - LABEL_GDT
SelectorData		equ		LABEL_DESC_DATA - LABEL_GDT + SA_RPL1
SelectorStack		equ		LABEL_DESC_STACK - LABEL_GDT
SelectorTest		equ		LABEL_DESC_TEST - LABEL_GDT
SelectorCode32		equ		LABEL_DESC_CODE32 - LABEL_GDT
SelectorVideo		equ		LABEL_DESC_VIDEO - LABEL_GDT
SelectorLDT			equ		LABEL_DESC_LDT - LABEL_GDT
SelectorCodeDest	equ		LABEL_DESC_CODE_DEST - LABEL_GDT 
SelectorCallGateTest	equ		LABEL_CALL_GATE_TEST - LABEL_GDT + SA_RPL3
SelectorCodeRing3	equ		LABEL_DESC_CODE_RING3 - LABEL_GDT + SA_RPL3
SelectorStack3		equ		LABEL_DESC_STACK3 - LABEL_GDT + SA_RPL3
SelectorTSS			equ		LABEL_DESC_TSS - LABEL_GDT

[SECTION .ldt]
ALIGN 32
LABEL_LDT:
LABEL_LDT_DESC_CODE_A	Descriptor	0,		CodeALen - 1,		DA_C + DA_32
LABEL_LDT_DESC_CODE_B	Descriptor	0,		CodeBLen - 1,		DA_C + DA_32

LDTLen				equ		$ - LABEL_LDT

SelectorLDTCodeA	equ		LABEL_LDT_DESC_CODE_A - LABEL_LDT + SA_TIL
SelectorLDTCodeB	equ		LABEL_LDT_DESC_CODE_B - LABEL_LDT + SA_TIL


[SECTION .data1]
ALIGN	32
[BITS 32]
LABEL_DATA:
SPValueInRealMode		dw		0
PMMessage:				db		"In Protect Mode Now ...", 0
OffsetPMMessage			equ		PMMessage - $$
StrTest:				db		"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0
OffsetStrTest			equ		StrTest - $$
DataLen					equ		$ - LABEL_DATA

[SECTION .tss]
ALIGN 32
[BITS 32]
LABEL_TSS:
	  dd	0				; back
	  dd	TopOfStack		; 0 ring stack
	  dd	SelectorStack	; 
	  dd	0				; 1 ring stack
	  dd	0				;
	  dd	0				; 2 ring stack
	  dd	0				;
	  dd	0				; CR3
	  dd	0				; EIP
	  dd	0				; EFLAGS
	  dd	0				; EAX
	  dd	0				; ECX
	  dd	0				; EDX
	  dd	0				; EBX
	  dd	0				; ESP
	  dd	0				; EBP
	  dd	0				; ESI
	  dd	0				; EDI
	  dd	0				; ES
	  dd	0				; CS
	  dd	0				; SS
	  dd	0				; DS
	  dd	0				; FS
	  dd	0				; GS
	  dd	0				; LDT
	  dw	0				; 
	  dw	$ - LABEL_TSS + 2 ; I/O Addr
	  db	0ffh			;
TSSLen		equ		$ - LABEL_TSS

[SECTION .gs]
ALIGN	32
[BITS 32]
LABEL_STACK:
	  times		512		db	0
TopOfStack:	 equ	$ - LABEL_STACK - 1

[SECTION .s3]
ALIGN	32
[BITS 32]
LABEL_STACK3:
	  times		512		db	0
TopOfStack3:	 equ	$ - LABEL_STACK3 - 1


[SECTION .s16]
[BITS 16]
LABEL_BEGIN:
	  mov	ax,cs
	  mov	ds,ax
	  mov	es,ax
	  mov	ss,ax
	  mov	sp,0100h

	  mov	[LABLE_GO_BACK_TO_REAL + 3],ax

	  mov	ebx,LABEL_SEG_CODE32
	  mov	ecx,LABEL_DESC_CODE32
	  call	INIT_DESC

	  mov	ebx,LABEL_LDT
	  mov	ecx,LABEL_DESC_LDT
	  call	INIT_DESC

	  mov	ebx,LABEL_CODE_A
	  mov	ecx,LABEL_LDT_DESC_CODE_A
	  call	INIT_DESC
	  
	  mov	ebx,LABEL_CODE_B
	  mov	ecx,LABEL_LDT_DESC_CODE_B
	  call	INIT_DESC

	  mov	ebx,LABEL_DATA
	  mov	ecx,LABEL_DESC_DATA
	  call	INIT_DESC

	  mov	ebx,LABEL_STACK
	  mov	ecx,LABEL_DESC_STACK
	  call	INIT_DESC

	  mov	ebx,LABEL_SEG_CODE16
	  mov	ecx,LABEL_DESC_CODE16
	  call	INIT_DESC

	  mov	ebx,LABEL_SEG_CODE_DEST
	  mov	ecx,LABEL_DESC_CODE_DEST
	  call	INIT_DESC

	  mov	ebx,LABEL_CODE_RING3
	  mov	ecx,LABEL_DESC_CODE_RING3
	  call	INIT_DESC

	  mov	ebx,LABEL_STACK3
	  mov	ecx,LABEL_DESC_STACK3
	  call	INIT_DESC

	  mov	ebx,LABEL_TSS
	  mov	ecx,LABEL_DESC_TSS
	  call	INIT_DESC

	  xor	eax,eax
	  mov	ax,ds
	  shl	eax,4
	  add	eax,LABEL_GDT
	  mov	dword [GdtPtr + 2],eax

	  lgdt	[GdtPtr]

	  cli

	  in	al,92h
	  or	al,00000010b
	  out	92h,al

	  mov	eax,cr0
	  or	eax,1
	  mov	cr0,eax

	  jmp	dword SelectorCode32:0
INIT_DESC:
	  xor	eax,eax
	  mov	ax,cs
	  shl	eax,4
	  add	eax,ebx
	  mov	word [ecx + 2],ax
	  shr	eax,16
	  mov	byte [ecx + 4],al
	  mov	byte [ecx + 7],ah
	  ret

LABEL_REAL_ENTRY:
	  mov	ax,cs
	  mov	ds,ax
	  mov	es,ax
	  mov	ss,ax

	  mov	sp,[SPValueInRealMode]

	  in	al,92h
	  and	al,11111101b
	  out	92h,al

	  sti

	  mov	ax,4c00h
	  int	21h
[SECTION .s32]
[BITS 32]

LABEL_SEG_CODE32:
	  mov	ax,SelectorData
	  mov	ds,ax
	  mov	ax,SelectorTest
	  mov	es,ax
	  mov	ax,SelectorVideo
	  mov	gs,ax

	  mov	ax,SelectorStack
	  mov	ss,ax
	  mov	esp,TopOfStack


	  mov	ah,0ch
	  xor	edi,edi
	  xor	esi,esi
	  mov	esi,OffsetPMMessage
	  mov	edi,(80 * 11 + 0) * 2
	  cld

.1:
	  lodsb 
	  test	al,al
	  jz	.2
	  mov	[gs:edi],ax
	  add	edi,2
	  jmp	.1

.2:
	  
	  call	DispReturn

	  mov	ax,SelectorTSS
	  ltr	ax

	  push	SelectorStack3
	  push	TopOfStack3
	  push	SelectorCodeRing3
	  push	0
	  retf

	  call	TestRead
	  call	TestWrite
	  call	TestRead

	  jmp	SelectorCode16:0

TestRead:
	  xor	esi,esi
	  mov	ecx,8

.loop:
	  mov	al,[es:esi]
	  call	DispAll
	  inc	esi
	  loop	.loop
	  call DispReturn
	  ret

TestWrite:
	  push	esi
	  push	edi
	  xor	esi,esi
	  xor	edi,edi
	  mov	esi,OffsetStrTest
	  cld
.1:
	  lodsb
	  test	al,al
	  jz	.2
	  mov	[es:edi],al
	  inc	edi
	  jmp	.1

.2:
	  pop	edi
	  pop	esi
	  ret

DispAll:
	  push	ecx
	  push	edx

	  mov	ah,0ch
	  mov	dl,al
	  shr	al,4
	  mov	ecx,2
.begin:
	  and	al,01111b
	  cmp	al,9
	  ja	.1
	  add	al,'0'
	  jmp	.2
.1:
	  sub	al,0ah
	  add	al,'A'
.2:
	  mov	[gs:edi],ax
	  add	edi,2
	  mov	al,dl
	  loop	.begin
	  add	edi,2

	  pop	edx
	  pop	ecx

	  ret
DispReturn:
	  push	eax
	  push	ebx

	  mov	eax,edi
	  mov	bl,160
	  div	bl
	  and	eax,0ffh
	  inc	eax
	  mov	bl,160
	  mul	bl
	  mov	edi,eax
	  pop	ebx
	  pop	eax
	  ret
Seg32Len		equ		$ - LABEL_SEG_CODE32
[SECTION .s16code]
ALIGN 32
[BITS 16]
LABEL_SEG_CODE16:
	  ; jmp back to real mode
	  mov	ax,SelectorNormal
	  mov	ds,ax
	  mov	es,ax
	  mov	fs,ax
	  mov	gs,ax
	  mov	ss,ax

	  mov	eax,cr0
	  and	al,11111110b
	  mov	cr0,eax

LABLE_GO_BACK_TO_REAL:
	  jmp	0:LABEL_REAL_ENTRY

Code18Len	equ		$ - LABEL_SEG_CODE16
[SECTION .la]
ALIGN 32
[BITS 32]
LABEL_CODE_A:
	  mov	ax,SelectorVideo
	  mov	gs,ax

	  mov	edi,(80 * 12 + 0) * 2
	  mov	ah,0ch
	  mov	al,'A'
	  mov	[gs:edi],al

	  jmp	SelectorLDTCodeB:0

CodeALen		equ		$ - LABEL_CODE_A
[SECTION .la]
ALIGN 32
[BITS 32]
LABEL_CODE_B:
	  mov	ax,SelectorVideo
	  mov	gs,ax

	  mov	edi,(80 * 12 + 1) * 2
	  mov	ah,0ch
	  mov	al,'B'
	  mov	[gs:edi],al

	  jmp	SelectorCode16:0

CodeBLen		equ		$ - LABEL_CODE_B
[SECTION .sdest]
[BITS 32]
LABEL_SEG_CODE_DEST:
	  mov	ax,SelectorVideo
	  mov	gs,ax

	  mov	edi,(80 * 12 + 2) * 2
	  mov	ah,0ch
	  mov	al,'C'
	  mov	[gs:edi],al

	  mov	ax,SelectorLDT
	  lldt	ax
	  jmp	SelectorLDTCodeA:0

	  ;retf

SegCodeDestLen		equ		$ - LABEL_SEG_CODE_DEST

[SECTION .ring3]
ALIGN 32
[BITS 32]
LABEL_CODE_RING3:
	  mov	ax,SelectorVideo
	  mov	gs,ax

	  mov	edi,(80 * 12 + 3) * 2
	  mov	ah,0ch
<<<<<<< ours
	  mov	al,'r'
=======
	  mov	al,'3'
>>>>>>> theirs
	  mov	[gs:edi],al
	  call	SelectorCallGateTest:0

	  jmp	$
SegCodeRing3	equ		$ - LABEL_CODE_RING3
