.extern	kernel_main
DATASEG=0x10

begin_pm:
	movl	$DATASEG,%eax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	call	kernel_main
	jmp	.	
