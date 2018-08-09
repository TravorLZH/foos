/* Entry of the kernel */
#include <system.h>
#include <dev/tty.h>
#include <cpu/interrupt.h>
#include <stdio.h>

struct tty kernel_tty;

int kernel_main(void)
{
	tty_init(&kernel_tty);
	idt_init();
	int a=3/0;
	return 0;
}
