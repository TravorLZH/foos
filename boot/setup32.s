.code32
CODESEG=0x8
DATASEG=0x10

begin_pm:
	movl	$DATASEG,%eax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	movl	$0xB8000,%ebx
	movl	$0x0F41,%eax
	movl	%eax,(%ebx)
	jmp	.
.space	256-(.-begin_pm)
