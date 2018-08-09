/* Entry of the kernel */
#include <system.h>
#include <dev/tty.h>
#include <stdio.h>

struct tty kernel_tty;

int kernel_main(void)
{
	tty_init(&kernel_tty);
	puts("Hello world");
	return 0;
}
