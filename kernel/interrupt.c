#include <foos/system.h>
#include <foos/kmalloc.h>
#include <cpu/interrupt.h>
#include <dev/pic.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static struct idt_entry *entries=NULL;
static const size_t entries_sz=sizeof(struct idt_entry)*256;
static struct idt_desc idtr;
static inthandler_t *handlers=NULL;
static const size_t handlers_sz=sizeof(inthandler_t)*256;

static void irq_init(void);

static void zero_division(struct registers regs)
{
	puts("Zero division");
	hang();
}

static void general_protection(struct registers regs)
{
	printf("General Protection Fault on Segment: %d\n",regs.err_code);
	printf("Stack: 0x%x\n",regs.esp);
	printf("At: 0x%x\n",regs.eip);
	printf("DS: 0x%x\n",regs.ds);
	hang();
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

static void idt_init(void)
{
	if(entries==NULL){
		entries=kmalloc(entries_sz);
		memset(entries,0,entries_sz);
	}
	if(handlers==NULL){
		handlers=kmalloc(handlers_sz);
		memset(handlers,0,handlers_sz);
	}
	idtr.limit=entries_sz-1;
	idtr.base=entries;
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
	irq_init();
	__asm__("lidt (%%eax)"::"a"((uint32_t)&idtr));
}

void int_init(void)
{
	puts("Initializing Interrupt");
	idt_init();
	int_hook_handler(0x00,zero_division);
	int_hook_handler(0x0D,general_protection);
}

void int_handler(struct registers regs)
{
	uint8_t color=kernel_tty->color;
	kernel_tty->color=0x0F;
	if(handlers[regs.int_no]){
		inthandler_t h=handlers[regs.int_no];
		h(regs);
	}else{
		char tmp[4];
		tmp[3]='\0';
		itoa(regs.int_no,tmp);
		tty_writestring(NULL,"Unhandled interrupt ");
		tty_writestring(NULL,tmp);
		putchar('\n');
	}
	kernel_tty->color=color;
}

void int_hook_handler(uint8_t no,inthandler_t handler)
{
	handlers[no]=handler;
}

/* IRQ Stuff here */
static void irq_init(void)
{
	pic_remap(0x20,0x28);
	idt_hook(32,irq0,CODESEG,0x8E);
	idt_hook(33,irq1,CODESEG,0x8E);
	idt_hook(34,irq2,CODESEG,0x8E);
	idt_hook(35,irq3,CODESEG,0x8E);
	idt_hook(36,irq4,CODESEG,0x8E);
	idt_hook(37,irq5,CODESEG,0x8E);
	idt_hook(38,irq6,CODESEG,0x8E);
	idt_hook(39,irq7,CODESEG,0x8E);
	idt_hook(40,irq8,CODESEG,0x8E);
	idt_hook(41,irq9,CODESEG,0x8E);
	idt_hook(42,irq10,CODESEG,0x8E);
	idt_hook(43,irq11,CODESEG,0x8E);
	idt_hook(44,irq12,CODESEG,0x8E);
	idt_hook(45,irq13,CODESEG,0x8E);
	idt_hook(46,irq14,CODESEG,0x8E);
	idt_hook(47,irq15,CODESEG,0x8E);
}

void irq_handler(struct registers regs)
{
	pic_send_eoi(regs.int_no-0x20);
	if(handlers[regs.int_no]!=NULL){
		inthandler_t h=handlers[regs.int_no];
		h(regs);
	}
}
