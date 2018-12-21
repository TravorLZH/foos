#include <dev/pit.h>
#include <cpu/interrupt.h>
#include <asm/ioports.h>
#include <stdio.h>
#include <errno.h>

static int stuff=0;
static char busy=0;
static uint32_t ticks=0;

int pit_delay(uint32_t _ticks)
{
	if(busy){
		errno=EBUSY;
		return -errno;
	}
	busy=1;
	ticks=_ticks;
	while(busy);
	return 0;
}

static void pit_irq(struct registers regs)
{
	if(busy && ticks>0){
		ticks--;
	}else{
		busy=0;
	}
}

int pit_init(uint32_t freq)
{
	uint32_t divisor=1193180 / freq;
	uint8_t low=(uint8_t)(divisor & 0xFF);
	uint8_t high=(uint8_t)((divisor >> 8) & 0xFF);
	int_hook_handler(0x20,pit_irq);
	outb(PIT_CMD,PIT_CHANNEL0|PIT_MODE3|PIT_LOHIBYTE);
	outb(PIT_DATA0,low);
	outb(PIT_DATA0,high);
	return 0;
}
