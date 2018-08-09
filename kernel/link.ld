ENTRY(kernel_entry)
SECTIONS
{
	. = 0x0;
	.text	:
	{
		*(.text)
		. = ALIGN(0x200);
	}
	.data	:
	{
		*(.data)
		*(.rodata)
		. = ALIGN(0x200);
	}
	.bss	:
	{
		*(.bss)
		. = ALIGN(0x200);
	}
}
