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
LABEL_DESC_DATA:	Descriptor	0,			DataLen - 1,	DA_DRW
LABEL_DESC_STACK:	Descriptor	0,			TopOfStack,		DA_DRWA + DA_32
LABEL_DESC_LDT:		Descriptor	0,			LDTLen - 1,		DA_LDT
LABEL_DESC_TEST:	Descriptor	0500000h,	0ffffh,			DA_DRW
LABEL_DESC_VIDEO:	Descriptor	0B8000h,	0FFFFh,			DA_DRW

GdtLen		equ		$ - LABEL_GDT
GdtPtr		dw		GdtLen - 1
			dd		0

SelectorNormal		equ		LABEL_DESC_NORMAL - LABEL_GDT
SelectorCode16		equ		LABEL_DESC_CODE16 - LABEL_GDT
SelectorData		equ		LABEL_DESC_DATA - LABEL_GDT
SelectorStack		equ		LABEL_DESC_STACK - LABEL_GDT
SelectorTest		equ		LABEL_DESC_TEST - LABEL_GDT
SelectorCode32		equ		LABEL_DESC_CODE32 - LABEL_GDT
SelectorVideo		equ		LABEL_DESC_VIDEO - LABEL_GDT
SelectorLDT			equ		LABEL_DESC_LDT - LABEL_GDT

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

[SECTION .gs]
ALIGN	32
[BITS 32]
LABEL_STACK:
	  times		512		db	0
TopOfStack:	 equ	$ - LABEL_STACK - 1

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
	  mov	ax,SelectorLDT
	  lldt	ax
	  jmp	SelectorLDTCodeA:0
	  
	  call	DispReturn

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
