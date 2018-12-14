/* This floppy driver is not complete or even working */
#include <dev/dma.h>
#include <dev/floppy.h>
#include <asm/ioports.h>
#include <cpu/interrupt.h>
#include <stdio.h>

bool irq=false;

static void floppy_irq(struct registers regs)
{
	irq=true;
}

void floppy_wait_irq(void)
{
	while(!irq);
	irq=false;
}

int floppy_init(void *reserved)
{
	int_hook_handler(0x26,floppy_irq);
	return 0;
}

void floppy_create(struct floppy *flp)
{
	if(!flp){
		return;
	}
	flp->fdc=FDC1;
}
