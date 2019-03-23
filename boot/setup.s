.code16
.text
# Constant
SETUPSEG=0x9020
CODESEG=0x8
PMBASE=0x90200
SYSSEG=0x1000
SYSSIZE=0x6400
RAMDISK=0x81	# ramdisk in harddisk #2
RD_SECTORS=1
RDSEG=0x3000
RDEND=0x4000
# Kernel configuration is placed in 0x80000
KCONFSEG=0x8000
# The following variable are the offset from KCONFSEG:0000
KCONF_FLAGS=0		# Flags: bit[1:0] means ramdisk enabled
KCONF_RAMDISKSTART=4	# ramdisk start address
KCONF_RAMDISKEND=8	# ramdisk end address

.globl	_start
_start:
setup:
	movw	$SETUPSEG,%ax
	movw	%ax,%ds
	movw	$KCONFSEG,%ax
	movw	%ax,%fs
	movw	$load_rd,%si
	call	print_string
load_ramdisk:	# Load it to 3000:0000
	stc
	movb	$0x0,%ah
	movb	$RAMDISK,%dl
	int	$0x13
	movw	$RDSEG,%ax
	movw	%ax,%es
	movw	$0x0,%bx
	movb	$0x02,%ah
	movb	$RD_SECTORS,%al
	movw	$0x0001,%cx
	int	$0x13
	jc	no_ramdisk
ramdisk_ok:
	movw	$newline,%si
	call	print_string
	movl	%fs:KCONF_FLAGS,%eax
	orl	$0x1,%eax	# Set RD flag
	movl	%eax,%fs:KCONF_FLAGS
	movl	$RDSEG << 4,%eax
	movl	%eax,%fs:KCONF_RAMDISKSTART
	movl	$RDEND << 4,%eax
	movl	%eax,%fs:KCONF_RAMDISKEND
	jmp	move_system
no_ramdisk:
	movw	$no_rd,%si
	call	print_string
# Remember: No interrupts shall be called after this! We destroyed the IVT
move_system:	# From 1000:0000 to 0000:0000
	movw	$move_sys,%si
	call	print_string
	pushw	%ds
	movw	$SYSSEG,%ax
	movw	%ax,%ds
	xorw	%ax,%ax
	movw	%ax,%es
	xorw	%si,%si
	xorw	%di,%di
	movw	$SYSSIZE,%cx
	rep
	movsb
	popw	%ds
	jmp	switch_pm
switch_pm:
	lgdt	gdt_descriptor
	movl	%cr0,%eax
	orl	$0x1,%eax
	movl	%eax,%cr0
	ljmp	$CODESEG,$0x0

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
gdt_end:
gdt_descriptor:
	.word	gdt_end - gdt_start - 1	# 3 entries
	.long	gdt_start+PMBASE
# Strings
load_rd:
	.ascii	"[setup] loading `ramdisk'..."
	.byte	0x0
no_rd:
	.ascii	"n/a"
	.byte	0x0D,0x0A,0x0
move_sys:
	.ascii	"[setup] moving `system' from 1000:0000 to 0000:0000..."
	.byte	0x0D,0x0A,0x0
newline:
	.byte	0x0D,0x0A,0x0

.space	512-(.-setup)
