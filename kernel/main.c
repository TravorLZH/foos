/* Entry of the kernel */
#include <system.h>
#include <dev/tty.h>

struct tty kernel_tty;

int kernel_main(void)
{
	tty_init(&kernel_tty);
	tty_writestring(&kernel_tty,"Hello world\n");
	return 0;
}
