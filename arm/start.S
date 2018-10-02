.globl	_start
.extern	entry
.text
_start:
	ldr	sp,=stack_top
	bl	entry
hang:
	b	hang
