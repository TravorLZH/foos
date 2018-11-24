/* Entry of the kernel */
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <foos/device.h>
#include <dev/pit.h>
#include <dev/ramdisk.h>
#include <asm/ioports.h>
#include <asm/cmos.h>
#include <cpu/interrupt.h>
#include <cpu/memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int kernel_main(struct kernel_conf *conf)
{
	int_init();
	pmem_init(NULL);
	vmem_init(NULL);
	pit_init(1000);
	int_enable();
	dev_open(DEV_TTY,0);
	check_floppy();
	if(conf->flags & KF_RAMDISK){
		size_t rd_size=conf->rd_end - conf->rd_start;
		printf("Setting up %uKB RAM Disk at 0x%x\n",rd_size/1024,
				conf->rd_start);
		dev_open(DEV_RAMDISK,0);
		dev_ioctl(DEV_RAMDISK,RD_SETADDR,&conf->rd_start);
		dev_ioctl(DEV_RAMDISK,RD_SETSIZE,&rd_size);
		dev_ioctl(DEV_RAMDISK,RD_FLUSH,NULL);
		dev_close(DEV_RAMDISK);
	}
	dev_close(DEV_TTY);
	return 0;
}
