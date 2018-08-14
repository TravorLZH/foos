.extern	kernel_main
DATASEG=0x10

.globl	kernel_entry
kernel_entry:
	movl	$DATASEG,%eax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	movw	%ax,%ss
check_a20:
	movl	$0x100000,%edi
	movl	$0x200000,%esi
	movl	%esi,(%esi)
	movl	%edi,(%edi)
	cmpsd	# See if they are equal
	jne	a20_ok
enable_a20_fast:
	inb	$0x92,%al
	testb	$0x2,%al
	jne	a20_ok
	orb	$0x2,%al
	andb	$0xFE,%al
	outb	%al,$0x92
a20_ok:
	movl	$0x400000,%esp	# 1MB Stack (0x100000 - 0x400000)
	push	$0
	call	kernel_main
	addl	$4,%esp
	jmp	.
