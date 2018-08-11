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
	pusha
	movl	$0x100000,%edi
	movl	$0x200000,%esi
	movl	%esi,(%esi)
	movl	%edi,(%edi)
	cmpsd	# See if they are equal
	popa
	jne	a20_ok
enable_a20_fast:
	inb	$0x92,%al
	testb	$0x2,%al
	jne	a20_ok
	orb	$0x2,%al
	andb	$0xFE,%al
	outb	%al,$0x92
a20_ok:
	movl	$0x200000,%esp	# 1MB Stack (0x100000 - 0x200000)
	call	kernel_main
	jmp	.
