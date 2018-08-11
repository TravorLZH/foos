/* Entry of the kernel */
#include <system.h>
#include <dev/tty.h>
#include <asm/ioports.h>
#include <cpu/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

struct tty kernel_tty;

void kbd_irq(struct registers regs)
{
	puts("Keyboard");
	printf("Scancode: 0x%x\n",(uint32_t)inb(0x60));
}

int kernel_main(void)
{
	tty_init(&kernel_tty);
	int_init();
	int_hook_handler(0x21,kbd_irq);
	__asm__("sti");
	return 0;
}
