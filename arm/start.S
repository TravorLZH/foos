.globl	_start
.extern	entry
.text
_start:
	ldr	sp,=stack_top
	ldr	r0,=#0x101f1000
	ldr	r1,=#'A'
	str	r1,[r0,#0]
	str	r1,[r0,#0]
	bl	entry
hang:
	b	hang
