OUTPUT_FORMAT(elf32-i386)
ENTRY(kernel_entry)
SECTIONS
{
	. = 0x0;
	.text	:
	{
		*(.text)
		. = ALIGN(0x1000);
	}
	.data	:
	{
		*(.data)
		*(.rodata)
		. = ALIGN(0x1000);
	}
	.bss	:
	{
		*(.bss)
		. = ALIGN(0x1000);
	}
}
