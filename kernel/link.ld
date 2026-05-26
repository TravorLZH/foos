OUTPUT_FORMAT(elf32-i386)
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
		. = ALIGN(0x200);	/* To allow faster zeroing instructions */
		__bss_start = .;
		*(COMMON)
		*(.bss)
		*(.bss.*)

		. = ALIGN(0x200);
		__bss_end = .;
	}
}
