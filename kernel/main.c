/* Entry of the kernel */
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <dev/tty.h>
#include <dev/pit.h>
#include <asm/ioports.h>
#include <asm/cmos.h>
#include <cpu/interrupt.h>
#include <cpu/memory.h>
#include <stdio.h>
#include <stdlib.h>

struct tty *kernel_tty=NULL;

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
	kernel_tty=(struct tty*)kmalloc(sizeof(struct tty));
	tty_create(kernel_tty);
	int_init();
	tty_init(NULL);
	pmem_init(NULL);
	vmem_init(NULL);
	pit_init(1000);
	int_enable();
	check_floppy();
	return 0;
}
