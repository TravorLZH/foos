/* Entry of the kernel */
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <foos/device.h>
#include <foos/ramfs.h>
#include <dev/tty.h>
#include <dev/pit.h>
#include <dev/ramdisk.h>
#include <asm/ioports.h>
#include <asm/cmos.h>
#include <cpu/interrupt.h>
#include <cpu/memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int _puts(const char*);

char buf[BUFSIZ];	// Buffer for everything
uint8_t default_color;
uint8_t white_on_black=0x0F;

static const char* floppy_type[]={
	"Not Applicable",
	"360 KB 5.25\"",
	"1.2 MB 5.25\"",
	"720 KB 3.5\"",
	"1.44 MB 3.5\"",
	"2.88 MB 3.5\""
};

struct inode *fs_root=NULL;

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
	int i=0;
	struct dirent *ent=NULL;
	struct inode *tmp=NULL;
	size_t rd_size;
	int_init();
	pmem_init(NULL);
	vmem_init(NULL);
	pit_init(1000);
	int_enable();
	dev_open(DEV_TTY,0);
	check_floppy();
	if(!(conf->flags & KF_RAMDISK)){
		puts("ramdisk: Not configured, so no ramdisk");
	}
	rd_size=conf->rd_end - conf->rd_start;
	printf("ramdisk: size=%uKB, start=0x%x\n",rd_size/1024,conf->rd_start);
	dev_open(DEV_RAMDISK,0);
	dev_ioctl(DEV_RAMDISK,RD_SETADDR,&conf->rd_start);
	dev_ioctl(DEV_RAMDISK,RD_SETSIZE,&rd_size);
	fs_root=ramfs_init();
	ent=fs_readdir(fs_root,0);
	puts("Files in ramdisk: (name, size in bytes)");
	do{
		tmp=fs_finddir(fs_root,ent->name);
		printf("| %s\t| %d\n",tmp->name,tmp->size);
	}while(ent=fs_readdir(fs_root,++i));
	tmp=fs_finddir(fs_root,"greeting.txt");
	i=fs_read(tmp,buf,tmp->size,0);
	buf[tmp->size]='\0';
	dev_ioctl(DEV_TTY,TTY_GETCOL,&default_color);
	dev_ioctl(DEV_TTY,TTY_SETCOL,&white_on_black);
	_puts(buf);
	dev_ioctl(DEV_TTY,TTY_SETCOL,&default_color);
	dev_close(DEV_RAMDISK);
no_ramdisk:
	dev_close(DEV_TTY);
	return 0;
}
