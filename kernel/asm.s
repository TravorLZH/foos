.code32
.extern	int_handler
DATASEG=0x10

.macro	INT_NOERRCODE	no
	.globl	int\no
	int\no:
		cli
		push	$0
		push	$\no
		jmp	int_stub
.endm

.macro	INT_ERRCODE	no
	.globl	int\no
	int\no:
		cli
		push	$\no
		jmp	int_stub
.endm

INT_NOERRCODE	0
INT_NOERRCODE	1
INT_NOERRCODE	2
INT_NOERRCODE	3
INT_NOERRCODE	4
INT_NOERRCODE	5
INT_NOERRCODE	6
INT_NOERRCODE	7
INT_ERRCODE	8
INT_NOERRCODE	9
INT_ERRCODE	10
INT_ERRCODE	11
INT_ERRCODE	12
INT_ERRCODE	13
INT_ERRCODE	14
INT_NOERRCODE	15
INT_NOERRCODE	16
INT_NOERRCODE	17
INT_NOERRCODE	18
INT_NOERRCODE	19
INT_NOERRCODE	20
INT_NOERRCODE	21
INT_NOERRCODE	22
INT_NOERRCODE	23
INT_NOERRCODE	24
INT_NOERRCODE	25
INT_NOERRCODE	26
INT_NOERRCODE	27
INT_NOERRCODE	28
INT_NOERRCODE	29
INT_NOERRCODE	30
INT_NOERRCODE	31

.globl	int_stub
int_stub:
	pusha
	movw	%ds,%ax
	pushl	%eax

	movw	$DATASEG,%ax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs

	call	int_handler

	popl	%eax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	popa
	addl	$8,%esp
	iret
