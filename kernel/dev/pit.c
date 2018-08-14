#include <dev/pit.h>
#include <cpu/interrupt.h>
#include <asm/ioports.h>
#include <stdio.h>

static void pit_irq(struct registers regs)
{
	puts("PIT IRQ");
}

int pit_init(void *ptr)
{
	struct pit_config *config=(struct pit_config*)ptr;
	uint32_t freq=config->frequency;
	uint32_t divisor=PIT_FREQUENCY / freq;
	uint8_t low=divisor & 0xFF;
	uint8_t high=(divisor >> 8) * 0xFF;
	outb(PIT_CMD,PIT_CHANNEL0 | PIT_LOHIBYTE | PIT_MODE3 | PIT_BIN);
	outb(PIT_DATA0,low);
	outb(PIT_DATA0,high);
	int_hook_handler(0x20,pit_irq);
	return 0;
}
