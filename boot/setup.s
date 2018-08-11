.code16
.text
# Constant
SETUPSEG=0x1000
CODESEG=0x8
PMBASE=0x10000
SYSSIZE=0x1E00	# Max kernel size

setup:
	movw	$SETUPSEG,%ax
	movw	%ax,%ds
	movw	$welcome_setup,%si
	call	print_string
move_system:
	cli
	movw	$0x200,%ax
	movw	%ax,%si
	xorw	%ax,%ax
	movw	%ax,%di
	movw	%ax,%es
	movw	$SYSSIZE,%cx
	cld
	rep	movsb
switch_pm:
	lgdt	gdt_descriptor
	movl	%cr0,%eax
	orl	$0x1,%eax
	movl	%eax,%cr0
	ljmp	$CODESEG,$0x0
.code16
# Useful 16-bit functions
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

# Global Descriptor Table
gdt_start:
gdt_null:
	.word	0x0,0x0,0x0,0x0
gdt_code:
	.word	0xFFFF,0x0
	.byte	0x0
	# Access: 10011010 (0x9A), Flags: 1100 (0xC), Limit: 1111 (0xF)
	.byte	0x9A,0xCF
	.byte	0x0
gdt_data:
	.word	0xFFFF,0x0
	.byte	0x0
	# Access: 10010010 (0x92), Flags: 1100 (0xC), Limit: 1111 (0xF)
	.byte	0x92,0xCF
	.byte	0x0
gdt_descriptor:
	.word	0x17	# 3 entries
	.long	gdt_start+PMBASE
# Strings
welcome_setup:
	.ascii	"Moving kernel"
	.byte	0x0D,0x0A,0x0

.space	512-(.-setup)
