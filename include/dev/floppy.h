#ifndef	FLOPPY_H
#define	FLOPPY_H
#include <inttypes.h>
/* Which FDC are we using ? */
#define	FDC1	0x3F0
#define	FDC2	0x370
/* Offset from base */
#define	FDC_STATUSA	(floppy_fdc()|0x00)
#define	FDC_STATUSB	(floppy_fdc()|0x01)
#define	FDC_DOR		(floppy_fdc()|0x02)	/**< Digital Output Reg */
#define	FDC_MSR		(floppy_fdc()|0x04)	/**< Main Status Reg */
#define	FDC_DSR		(floppy_fdc()|0x04)	/**< Datarate Select Reg */
#define	FDC_FIFO	(floppy_fdc()|0x05)
#define	FDC_DIR		(floppy_fdc()|0x07)	/* Read (AT) */
#define	FDC_CCR		(floppy_fdc()|0x07)	/* Write (AT) */
/* Bits in DOR */
#define	DOR_CONTROLLER	0x04	/**< 1= Controller Enabled */
#define	DOR_DMA		0x08	/**< DMA and IRQ channel (1 for enabled) */
/* Motor Control of A, B, C and D (1 for start, 0 for stop) */
#define	DOR_MOTA	0x10
#define	DOR_MOTB	0x20
#define	DOR_MOTC	0x40
#define	DOR_MOTD	0x80
/* Bits in MSR */
#define	MSR_ACTA	0x01
#define	MSR_ACTB	0x02
#define	MSR_ACTC	0x04
#define	MSR_ACTD	0x08
#define	MSR_BUSY	0x10	/**< Instruction (device busy) */
#define	MSR_NODMA	0x20	/**< Not using DMA? */
#define	MSR_DATAIO	0x40	/**< 1=Contoller ? CPU 0=CPU ? Controller */
#define	MSR_MRQ		0x80	/**< Data register ready or not */
/* Commands */
#define	FLP_SPECIFY	0x03	/**< Set drive parameters */
#define	FLP_WRITE	0x05
#define	FLP_READ	0x06
#define	FLP_RECALIBRATE	0x07
#define	FLP_INTSTATUS	0x08
#define	FLP_FORMAT	0x0C
#define	FLP_SEEK	0x0F
#define	FLP_VERSION	0x10
#define	FLP_PERPEN_MODE	0x12
#define	FLP_CONFIGURE	0x13	/**< Set controller parameters */
#define	FLP_LOCK	0x14	/**< Protect controller params from a reset */
/* Option Command Bits */
#define	FLP_MULTITRACK	0x80
#define	FLP_MFM		0x40	/**< Always set this */

struct floppy_conf{
	uint8_t fdc;	/**<
};

/** Get the current FDC no.
 * @return Either of FDC1_BASE or FDC2_BASE
 */
extern uint8_t floppy_fdc(void);
/** Reset the controller */
extern int floppy_reset(void);
/** Wait until the floppy is ready for reading or writing */
extern void floppy_ready(void);
/** Write command to FDC */
extern void floppy_send_command(uint8_t cmd);
/** Read data from FDC */
extern uint8_t floppy_read_data(void);
/** Check status */
extern void floppy_check_interrupt(uint8_t *st0,uint8_t *cyl);
/** Recalibrate floppy */
extern int floppy_calibrate(void);
/** Initialization of FDC */
extern int floppy_init(void *reserved);
#endif
