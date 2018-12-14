/* Entry of the kernel */
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <foos/device.h>
#include <foos/ramfs.h>
#include <dev/tty.h>
#include <dev/pit.h>
#include <dev/ramdisk.h>
#include <cpu/interrupt.h>
#include <cpu/memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int _puts(const char*);
extern int shell_main(void);

char buf[BUFSIZ];	// Buffer for everything
uint8_t default_color;
uint8_t white_on_black=0x0F;

struct inode *fs_root=NULL;

int kernel_main(struct kernel_conf *conf)
{
	int i=0;
	struct inode *tmp=NULL;
	size_t rd_size;
	int_init();
	pmem_init(NULL);
	vmem_init(NULL);
	pit_init(1000);
	int_enable();
	dev_open(DEV_TTY,0);
	if(!(conf->flags & KF_RAMDISK)){
		puts("ramdisk: Not configured, so no ramdisk");
	}
	rd_size=conf->rd_end - conf->rd_start;
	printf("ramdisk: size=%uKB, start=0x%x\n",rd_size/1024,conf->rd_start);
	dev_open(DEV_RAMDISK,0);
	dev_ioctl(DEV_RAMDISK,RD_SETADDR,&conf->rd_start);
	dev_ioctl(DEV_RAMDISK,RD_SETSIZE,&rd_size);
	fs_root=ramfs_init();
	tmp=fs_finddir(fs_root,"greeting.txt");
	fs_open(tmp,0);
	i=fs_read(tmp,buf,tmp->size,0);
	fs_close(tmp);
	buf[tmp->size]='\0';
	dev_ioctl(DEV_TTY,TTY_GETCOL,&default_color);
	dev_ioctl(DEV_TTY,TTY_SETCOL,&white_on_black);
	_puts(buf);
	dev_ioctl(DEV_TTY,TTY_SETCOL,&default_color);
	shell_main();
	dev_close(DEV_RAMDISK);
no_ramdisk:
	dev_close(DEV_TTY);
	return 0;
}
