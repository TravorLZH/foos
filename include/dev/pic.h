#ifndef	PIC_H
#define	PIC_H
#include <inttypes.h>
#define	PIC1	0x20	/* Master PIC */
#define	PIC2	0xA0	/* Slave PIC */
#define	PIC1_CMD	PIC1
#define	PIC1_DATA	(PIC1+1)
#define	PIC2_CMD	PIC2
#define	PIC2_DATA	(PIC2+1)
#define	PIC_EOI	0x20	/* End of Interrupt */

#define	ICW1_ICW4	0x01	/* ICW4 (not) needed */
#define	ICW1_SINGLE	0x02	/* Single (cascade) mode */
#define	ICW1_INTERVAL4	0x04	/* Call address interval 4 (8) */
#define	ICW1_LEVEL	0x08	/* Level triggered (edge) mode */
#define	ICW1_INIT	0x10	/* Initialization - required! */
/* Additional information about the environment */
#define	ICW4_8086	0x01	/* 8086/88 (MCS-80/85) mode */
#define	ICW4_AUTO	0x02	/* Auto (normal) EOI */
#define	ICW4_BUFSLAVE	0x08	/* Buffered mode/slave */
#define	ICW4_BUFMASTEER	0x0C	/* Buffered mode/master */
#define	ICW4_SFNM	0x10	/* Special fully nested (not) */

extern void pic_remap(uint8_t offset1,uint8_t offset2);
extern void pic_send_eoi(uint8_t irq);
#endif
