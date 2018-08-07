.code16

boot:
	movw	$0x0E41,%ax
	int	$0x10
	jmp	.

.space	510-(.-boot)
.word	0xAA55
