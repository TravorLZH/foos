/* Entry of the kernel */
#include <system.h>
#include <dev/tty.h>
#include <cpu/interrupt.h>
#include <stdio.h>

struct tty kernel_tty;

int kernel_main(void)
{
	tty_init(&kernel_tty);
	int_init();
	__asm__("int $0x1");
	int a=3/0;
	return 0;
}
