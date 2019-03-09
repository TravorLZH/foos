#include <asm/ioports.h>
#include <dev/pic.h>

void pic_remap(uint8_t offset1,uint8_t offset2)
{
	uint8_t mask,mask2;
	mask=inb(PIC1_DATA);
	mask2=inb(PIC2_DATA);
	outb(PIC1_CMD,ICW1_INIT|ICW1_ICW4);
	outb(PIC2_CMD,ICW1_INIT|ICW1_ICW4);
	outb(PIC1_DATA,offset1);
	outb(PIC2_DATA,offset2);
	outb(PIC1_DATA,4);	// There is a Slave PIC at IRQ2 (0000 0100)
	outb(PIC2_DATA,2);	// Tell Slave PIC its cascade identity
	outb(PIC1_DATA,ICW4_8086);
	outb(PIC2_DATA,ICW4_8086);
	// Restore saved masks
	outb(PIC1_DATA,mask);
	outb(PIC2_DATA,mask2);
}

void pic_send_eoi(uint8_t irq)
{
	if(irq>=8){
		outb(PIC2_CMD,PIC_EOI);
	}
	outb(PIC1_CMD,PIC_EOI);
}

void pic_mask(uint8_t pic,uint8_t offset)
{
	outb(pic+1,inb(pic+1) | (1<<offset));
}

void pic_unmask(uint8_t pic,uint8_t offset)
{
	outb(pic+1,inb(pic+1) & ~(1<<offset));
}

void pic_disable(void)
{
	outb(PIC1_DATA,0xFF);
	outb(PIC2_DATA,0xFF);
}
