SECTIONS
{
	. = 0x10000;
	.boot . : { start.o(.text) }
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss COMMON) }
	. = ALIGN(8);
	. = . + 0x1000;
	stack_top = .;
}
