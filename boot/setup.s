.code16
.text
# Constant
SETUPSEG=0x1000
PMBASE=0x10000	# Base address of protected mode
CODESEG=0x8
DATASEG=0x10

setup:
	movw	$SETUPSEG,%ax
	movw	%ax,%ds
	movw	$welcome_setup,%si
	call	print_string
switch_pm:
	cli	# Critical operation won't need interrupts
	lgdt	gdt_descriptor
	movl	%cr0,%eax
	orb	$0x1,%al
	movl	%eax,%cr0
.code32
	ljmp	$CODESEG,$0x10100
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
	.word	gdt_start,1
# Strings
welcome_setup:
	.ascii	"Switching to protected mode"
	.byte	0x0D,0x0A,0x0

.space	256-(.-setup)
begin_pm:
	jmp	.+PMBASE
	movl	$DATASEG,%eax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	movl	$0xB8000,%ebx
	movl	$0x0F41,%eax
	movl	%eax,(%ebx)
	jmp	.+PMBASE
.space	256-(.-begin_pm)
