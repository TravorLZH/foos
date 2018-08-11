/* Entry of the kernel */
#include <system.h>
#include <dev/tty.h>
#include <asm/ioports.h>
#include <asm/cmos.h>
#include <cpu/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

struct tty kernel_tty;

static void kbd_irq(struct registers regs)
{
	uint8_t scancode=inb(0x60);
	if(scancode & 0x80){
		puts("Key released!");
	}else{
		puts("Key pressed!");
	}
	printf("Scancode: 0x%x\n",(uint32_t)scancode);
}

static const char* floppy_type[]={
	"Not Applicable",
	"360 KB 5.25\"",
	"1.2 MB 5.25\"",
	"720 KB 3.5\"",
	"1.44 MB 3.5\"",
	"2.88 MB 3.5\""
};

static void check_floppy(void)
{
	uint8_t val=cmos_read(0x10);
	printf("Drive A: ");
	puts(floppy_type[(val >> 4) & 0xF]);
	printf("Drive B: ");
	puts(floppy_type[val & 0xF]);
}

int kernel_main(void *reserved)
{
	tty_init(&kernel_tty);
	int_init();
	int_hook_handler(0x21,kbd_irq);
	int_enable();
	check_floppy();
	return 0;
}
