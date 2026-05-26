.extern	kernel_main
.extern __bss_start
.extern __bss_end
.globl	kernel_entry
DATASEG=0x10
RDSEG=0x3000
RDEND=0x4000

.text
kernel_entry:
	movl	$DATASEG,%eax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%gs
	movw	%ax,%ss
check_a20:
	movl	$0x100000,%edi
	movl	$0x200000,%esi
	movl	%esi,(%esi)
	movl	%edi,(%edi)
	cmpsl	# See if they are equal
	jne	a20_ok
enable_a20_fast:
	inb	$0x92,%al
	testb	$0x2,%al
	jne	a20_ok
	orb	$0x2,%al
	andb	$0xFE,%al
	outb	%al,$0x92
a20_ok:		# Now, we clean up BSS and enter kernel_main
	cld
	xorl	%eax,%eax
	movl	$__bss_start,%edi
	movl	$__bss_end,%ecx
	subl	%edi,%ecx
	shrl	$2,%ecx		# We are moving 4 byte each time because the
	rep	stosl		# addresses are aligned (see kernel/link.ld)
enter_kernel_main:
	movl	$0x400000,%esp	# 3MB Stack (0x100000 - 0x400000)
	pushl	$0x80000
	xorl	%ebp,%ebp	# Used for stack tracing
	call	kernel_main
	addl	$0x4,%esp
	jmp	.
