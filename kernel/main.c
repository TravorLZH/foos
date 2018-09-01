/* Entry of the kernel */
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <foos/device.h>
#include <dev/tty.h>
#include <dev/pit.h>
#include <asm/ioports.h>
#include <asm/cmos.h>
#include <cpu/interrupt.h>
#include <cpu/memory.h>
#include <stdio.h>
#include <stdlib.h>

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
	int_init();
	pmem_init(NULL);
	vmem_init(NULL);
	tty_init(NULL);
	dev_open(DEV_TTY,0);
	pit_init(1000);
	int_enable();
	check_floppy();
	int ret=pit_delay(4000);
	if(!ret){
		puts("4 seconds!");
	}else{
		puts("error on delaying");
	}
	dev_close(DEV_TTY);
	return 0;
}
