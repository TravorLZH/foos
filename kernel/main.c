/* Entry of the kernel */
#include <system.h>
#include <dev/tty.h>
#include <cpu/interrupt.h>
#include <stdio.h>

struct tty kernel_tty;

void timer_irq(struct registers regs)
{
	puts("Timer IRQ");
}

int kernel_main(void)
{
	tty_init(&kernel_tty);
	int_init();
	__asm__("int $0x1");
	int_hook_handler(0x20,timer_irq);
	__asm__("sti");
	return 0;
}
