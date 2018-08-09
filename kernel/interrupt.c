#include <system.h>
#include <cpu/interrupt.h>
#include <string.h>
#include <stdio.h>

static struct idt_entry entries[256];
static struct idt_desc idtr;

static void idt_flush(struct idt_desc *idtr)
{
	__asm__("lidt (%%eax)"::"a"((uint32_t)idtr));
}

static void idt_hook(uint8_t num,void *fp,uint16_t segment,uint8_t flags)
{
	uint32_t base=(uint32_t)fp;
	entries[num].base_lo=base & 0xFFFF;
	entries[num].base_hi=(base >> 16) & 0xFFFF;
	entries[num].segment=segment;
	entries[num].zero=0;
	entries[num].flags=flags;
}

void idt_init(void)
{
	idtr.limit=sizeof(entries)-1;
	idtr.base=&entries;
	memset(&entries,0,sizeof(entries));
	idt_hook(0,int0,CODESEG,0x8E);
	idt_hook(1,int1,CODESEG,0x8E);
	idt_hook(2,int2,CODESEG,0x8E);
	idt_hook(3,int3,CODESEG,0x8E);
	idt_hook(4,int4,CODESEG,0x8E);
	idt_hook(5,int5,CODESEG,0x8E);
	idt_hook(6,int6,CODESEG,0x8E);
	idt_hook(7,int7,CODESEG,0x8E);
	idt_hook(8,int8,CODESEG,0x8E);
	idt_hook(9,int9,CODESEG,0x8E);
	idt_hook(10,int10,CODESEG,0x8E);
	idt_hook(11,int11,CODESEG,0x8E);
	idt_hook(12,int12,CODESEG,0x8E);
	idt_hook(13,int13,CODESEG,0x8E);
	idt_hook(14,int14,CODESEG,0x8E);
	idt_hook(15,int15,CODESEG,0x8E);
	idt_hook(16,int16,CODESEG,0x8E);
	idt_hook(17,int17,CODESEG,0x8E);
	idt_hook(18,int18,CODESEG,0x8E);
	idt_hook(19,int19,CODESEG,0x8E);
	idt_hook(20,int20,CODESEG,0x8E);
	idt_hook(21,int21,CODESEG,0x8E);
	idt_hook(22,int22,CODESEG,0x8E);
	idt_hook(23,int23,CODESEG,0x8E);
	idt_hook(24,int24,CODESEG,0x8E);
	idt_hook(25,int25,CODESEG,0x8E);
	idt_hook(26,int26,CODESEG,0x8E);
	idt_hook(27,int27,CODESEG,0x8E);
	idt_hook(28,int28,CODESEG,0x8E);
	idt_hook(29,int29,CODESEG,0x8E);
	idt_hook(30,int30,CODESEG,0x8E);
	idt_hook(31,int31,CODESEG,0x8E);
	idt_flush(&idtr);
}

void int_handler(struct registers regs)
{
	puts("Interrupt Called!\nYou need a handler");
}
