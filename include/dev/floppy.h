#ifndef	FLOPPY_H
#define	FLOPPY_H
#define	FDC1_BASE	0x3F0
#define	FDC2_BASE	0x370
/* Offset from base */
#define	FDC_STATUSA	0x00
#define	FDC_STATUSB	0x01
#define	FDC_DOR		0x02
#define	FDC_MSR		0x04	/* Read */
#define	FDC_DSR		0x04	/* Write (PS/2) */
#define	FDC_DATA	0x05
#define	FDC_DIR		0x07	/* Read (AT) */
#define	FDC_CCR		0x07	/* Write (AT) */
/* Bits in DOR */
/* Motor Control of D, C, B and A (1 for start, 0 for stop) */
#define	DOR_MOTD	0x80
#define	DOR_MOTC	0x40
#define	DOR_MOTB	0x20
#define	DOR_MOTA	0x10
#define	DOR_DMA		0x08	/* DMA and IRQ channel (1 for enabled) */
#define	DOR_CONTROLLER	0x04	/* 1= Controller Enabled */
/* The last 2 bits are drive select */
extern int floppy_init(void *reserved);
#endif
