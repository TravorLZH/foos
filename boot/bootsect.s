.code16

BOOTDEV=0x80
BOOTSEG=0x7C0
INITSEG=0x9000
SETUPSEG=0x9020
SYSSEG=0x1000	# Load system at 1000:0000
SYSSIZE=0x6400	# System size
SYS_SECTORS=SYSSIZE>>9
SETUP_SECTORS=1


# Declaration of `_start' is only used to avoid warning during the linkage
.globl	_start
_start:

# Once the system is booted at 0x7C00 (07C0:0000), we migrate to 9000:0000
boot:
	cli
	cld
	movw	$BOOTSEG,%ax
	movw	%ax,%ds
	movw	$INITSEG,%ax
	movw	%ax,%es
	xorw	%si,%si
	xorw	%di,%di
	movw	$SETUP_SECTORS,%cx
	shl	$0x8,%cx
	rep
	movsw
	ljmp	$INITSEG,$boot_main
# This is where we really start initializing our system
boot_main:
	movw	%ax,%ds
	cmpb	$BOOTDEV,%dl	# DL should be 0x80 in order to keep going
	je	reset_floppy
	movw	$should_boot_from_hda,%si
	call	print_string
	jmp	.
# Reset the boot drive to avoid errors
reset_floppy:
	movb	$0x0,%ah
	int	$0x13
# Read the setup program that does Protected Mode setup for the kernel
# We can just simply use the BIOS-provided CHS read function
load_setup:
	movw	$loading_setup,%si
	call	print_string
	stc	# Some BIOSes cannot set CF on error
	movw	$SETUPSEG,%ax
	movw	%ax,%es
	movw	$0x0,%bx
	movb	$0x02,%ah	# BIOS read sectors
	movb	$SETUP_SECTORS,%al
	movw	$0x0002,%cx	# From cylinder 0, 2nd sector
	int	$0x13
	jc	disk_error
# This loads system using DAP table, WORKING!!!
load_system_ext:
	movw	$kernel_dap,%si
	movb	$0x42,%ah
	int	$0x13
	jc	disk_error
	cmp	$0x0,%ah
	jne	disk_error

run_setup:
	ljmp	$SETUPSEG,$0x0	# Let's go

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
disk_error:
	movw	$disk_read_failed,%si
	call	print_string
	movb	$0x40,%al
	addb	%ah,%al
	movb	$0xE,%ah
	int	$0x10
	jmp	.

# Kernel disk address packet
.align	4
kernel_dap:
.byte	0x10		# Size of packet
.byte	0
.word	SYS_SECTORS	# no. of sectors
.word	0x0,SYSSEG	# Transfer buffer
.long	0x02,0x0	# Starting LBA

# Strings
should_boot_from_hda:
	.ascii	"[boot error] should be booted from 1st hard disk"
	.byte	0x0D,0x0A,0x0
disk_read_failed:
	.ascii	"Disk reading failed with code "
	.byte	0x0
loading_setup:
	.ascii	"[boot] loading `setup'..."
	.byte	0x0D,0x0A,0x0
loading_system:
	.ascii	"[boot] loading `system'..."
	.byte	0x0D,0x0A,0x0
.space	510-(.-boot)
.word	0xAA55
