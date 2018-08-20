.code16

BOOTSEG=0x7C0
INITSEG=0x9000
SETUPSEG=0x9020
SYSSEG=0x1000	# Load system at 1000:0000
SYSSIZE=0x3000	# System size
SYS_SECTORS=SYSSIZE>>9
SETUP_SECTORS=1

.globl	_start
_start:
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
boot_main:
	movw	%ax,%ds
	movb	%dl,bootdev
	xorw	%dx,%dx
check_drive:
	movw	$boot_from,%si
	call	print_string
	cmpb	$0x80,%dl
	jg	print_drive
	andb	$0x7F,%dl
	add	$0x2,%dl
print_drive:
	movw	$0x0E41,%ax
	addb	%dl,%al
	int	$0x10
	movw	$newline,%si
	call	print_string
greeting:
	mov	$welcome,%si
	call	print_string
reset_floppy:
	movb	$0x0,%ah
	movb	bootdev,%dl
	int	$0x80
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
load_system:
	movw	$loading_system,%si
	call	print_string
	stc
	movw	$SYSSEG,%ax
	movw	%ax,%es
	movw	$0x0,%bx
	movb	$0x02,%ah
	movb	$SYS_SECTORS,%al
	movw	$0x0003,%cx
	int	$0x13
	jc	disk_error
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
	jmp	.
# Data
bootdev:		# This stores the boot drive no.
	.byte	0
welcome:
	.ascii	"This is FOOS kernel"
	.byte	0x0D,0x0A,0x0
reset_error:
	.ascii	"Failed to reset floppy"
	.byte	0x0D,0x0A,0x0
boot_from:
	.ascii	"Current drive: "
	.byte	0x0
newline:
	.byte	0x0D,0x0A,0x0
disk_read_failed:
	.ascii	"Failed to read disk"
	.byte	0x0D,0x0A,0x0
loading_setup:
	.ascii	"Loading setup..."
	.byte	0x0D,0x0A,0x0
loading_system:
	.ascii	"Loading system..."
	.byte	0x0D,0x0A,0x0
.space	510-(.-boot)
.word	0xAA55
