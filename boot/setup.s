.code16
.text

SETUPSEG=0x1000

setup:
	movw	$SETUPSEG,%ax
	movw	%ax,%ds
	movw	$welcome_setup,%si
	call	print_string
	jmp	.

# Useful functions
print_string:
	movb	$0x0E,%ah
1:
	lodsb
	cmp	$0x0,%al
	je	1f
	int	$0x10
	jmp	1b
1:
	ret

welcome_setup:
	.ascii	"Hello from setup"
	.byte	0x0D,0x0A,0x0

.space	512-(.-setup)
