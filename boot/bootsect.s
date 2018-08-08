.code16
.globl	boot

BOOTSEG=0x7C0

boot:
	ljmp	$BOOTSEG,$boot_main
boot_main:
	movw	$BOOTSEG,%ax
	movw	%ax,%ds
	mov	$welcome,%si
	call	print_string
	jmp	.

reset_floppy:
	pushw	%ax
	movb	$0x0,%ah
	int	$0x13
	popw	%ax

print_string:
	pushw	%ax
	movb	$0x0E,%ah
1:
	lodsb
	cmp	$0x0,%al
	je	1f
	int	$0x10
	jmp	1b
1:
	pop	%ax
	ret

bootdev:		# This stores the boot drive no.
	.byte	0

welcome:
	.ascii	"This is FOOS kernel"
	.byte	0x0D,0x0A,0x0

.space	510-(.-boot)
.word	0xAA55
