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
	movl	$0x10000,%esp
	call	kernel_main
	jmp	.
