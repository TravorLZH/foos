/* Entry of the kernel */
#include <foos/system.h>
#include <foos/kmalloc.h>
#include <foos/device.h>
#include <foos/ramfs.h>
#include <dev/tty.h>
#include <dev/pic.h>
#include <dev/pit.h>
#include <dev/ramdisk.h>
#include <cpu/interrupt.h>
#include <cpu/memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Non-POSIX function is not declared in header */
extern int _puts(const char*);

/* Kernel shell entry (I wouldn't put it in a separate header) */
extern int shell_main(void);

char *buf=NULL;

/* Some color used by FOOS */
uint8_t default_color;
uint8_t white_on_black=0x0F;

struct inode *fs_root=NULL;

int kernel_main(struct kernel_conf *conf)
{
	/* Mess with some variable */
	int i=0;
	struct inode *tmp=NULL;
	size_t rd_size;
	/* Interrupts */
	int_init();

	/* Make sure we can put stuff on the screen */
	dev_open(DEV_TTY,0);

	/* Paging and Frames */
	pmem_init(NULL);
	vmem_init(NULL);

	/* Timer */
	pit_init(1000);

	/* Enable interrupts and IRQs (For keyboards and timer) */
	int_enable();

	/* Allocate the general-purpose kernel buffer */
	buf=(char*)kmalloc(BUFSIZ);

	if(!(conf->flags & KF_RAMDISK)){
		puts("ramdisk: Not available");
		hang();
	}

	/* Initialize ramdisk if available */
	rd_size=conf->rd_end - conf->rd_start;
	printf("ramdisk: size=%uKB, start=0x%x\n",rd_size/1024,conf->rd_start);
	dev_open(DEV_RAMDISK,0);
	dev_ioctl(DEV_RAMDISK,RD_SETADDR,&conf->rd_start);
	dev_ioctl(DEV_RAMDISK,RD_SETSIZE,&rd_size);

#ifndef	USE_TARFS
	/* Initialize file system for ramdisk */
	fs_root=ramfs_init();
#else
	/* Initialize tar (Tape ARchiving) file system */
	fs_root=tarfs_init(conf->rd_start);
#endif
	/* Print the greeting message */
	tmp=fs_finddir(fs_root,"greeting.txt");
	fs_open(tmp,0);
	i=fs_read(tmp,buf,tmp->size,0);
	fs_close(tmp);
	buf[tmp->size]='\0';
	dev_ioctl(DEV_TTY,TTY_GETCOL,&default_color);
	dev_ioctl(DEV_TTY,TTY_SETCOL,&white_on_black);
	_puts(buf);
	dev_ioctl(DEV_TTY,TTY_SETCOL,&default_color);

	/* Enter kernel shell */
	shell_main();

	/* De-initialize FOOS devices */
	puts("kernel: Start halting system");
	dev_close(DEV_RAMDISK);
	dev_close(DEV_TTY);
	pic_disable();
	return 0;
}
