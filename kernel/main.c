/* Entry of the kernel */

int kernel_main(void)
{
	short *ptr=(short*)0xB8000;
	*ptr=0x0F42;
	return 0;
}
