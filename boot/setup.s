.code16
.text
# Constant
SETUPSEG=0x9020
CODESEG=0x8
PMBASE=0x90200
SYSSEG=0x1000
SYSSIZE=0x6C00
RAMDISK=0x81	# ramdisk in harddisk #2
RD_SECTORS=1
RDSEG=0x3000
RDEND=0x4000
# Kernel configuration is placed in 0x80000
KCONFSEG=0x8000
# The following variable are the offset from KCONFSEG:0000
# Flags:
# bit[1:0]: ramdisk availability
# bit[2:1]: RSDP availability
KCONF_FLAGS=0	
KCONF_RSDPSEG=2		# segment RSDP
KCONF_RAMDISKSTART=4	# ramdisk start address
KCONF_RAMDISKEND=8	# ramdisk end address

.globl	_start
_start:
setup:
	movw	$SETUPSEG,%ax
	movw	%ax,%ds
	movw	$KCONFSEG,%ax
	movw	%ax,%fs
# Find RSDP (Root System Description Pointer) for ACPI driver
# between E000:0000 and FFFF:0000
lookup_rsdp:
	movw	$lookup_rsdp_msg,%si
	call	print_string
	movw	$0xE000,%ax
1:
	movw	%ax,%es
	movw	$rsdp_sig,%si
	xorw	%di,%di
	movw	$4,%cx	# RSDP signature is 4 words long
	repe	cmpsw
	jcxz	found_rsdp
	incw	%ax
	cmpw	$0xFFFF,%ax
	jle	1b
# If not found in the, find in the first 1024 bytes from EBDA
lookup_rsdp2:
	pushw	%ds
	movw	$0x40E,%ax
	movw	%ax,%ds
	movw	%ds:0x0,%ax
	popw	%ds
	movw	%ax,%dx
	addw	$0x400,%dx
1:
	movw	%ax,%es
	movw	$rsdp_sig,%si
	xorw	%di,%di
	movw	$4,%cx
	repe	cmpsw
	jcxz	found_rsdp
	incw	%ax
	cmpw	%dx,%ax
	jle	1b
notfound:
	movw	$rsdp_notfound_msg,%si
	call	print_string
	jmp	load_ramdisk
found_rsdp:
	pushw	%ax
	movw	$rsdp_found_msg,%si
	call	print_string
	movw	%fs:KCONF_FLAGS,%ax
	orw	$0x2,%ax	# Set RSDP flag
	movw	%ax,%fs:KCONF_FLAGS
	popw	%ax
	movw	%ax,%fs:KCONF_RSDPSEG	# Store it to the kernel configuration
load_ramdisk:	# Load it to 3000:0000
	movw	$load_rd,%si
	call	print_string
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
	movw	%fs:KCONF_FLAGS,%ax
	orw	$0x1,%ax	# Set RD flag
	movw	%ax,%fs:KCONF_FLAGS
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
rsdp_sig:
	.ascii	"RSD PTR "
lookup_rsdp_msg:
	.ascii	"[setup] looking for RSDP..."
	.byte	0x0
rsdp_notfound_msg:
	.ascii	"not "
rsdp_found_msg:
	.ascii	"found"
	.byte	0x0D,0x0A,0x0
no_rd:
	.ascii	"n/a"
	.byte	0x0D,0x0A,0x0
move_sys:
	.ascii	"[setup] moving `system' from 1000:0000 to 0000:0000..."
	.byte	0x0D,0x0A,0x0
newline:
	.byte	0x0D,0x0A,0x0

.space	512-(.-setup)
