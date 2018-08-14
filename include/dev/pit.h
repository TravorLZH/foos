#ifndef	PIT_H
#define	PIT_H
#include <inttypes.h>
#define	PIT_DATA0	0x40	/* Channel 0 data port */
#define	PIT_DATA1	0x41	/* Channel 1 data port */
#define	PIT_DATA2	0x42	/* Channel 2 data port */
#define	PIT_CMD		0x43	/* Mode/Command register */
/* Select Channel (bit 6:7) */
#define	PIT_CHANNEL0	0x00
#define	PIT_CHANNEL1	0x40
#define	PIT_CHANNEL2	0x80
#define	PIT_READBACK	0xC0
/* Access Mode (bit 4:5) */
#define	PIT_COUNTERLATCH	0x00
#define	PIT_LOBYTEONLY		0x10
#define	PIT_HIBYTEONLY		0x20
#define	PIT_LOHIBYTE		0x30
/* Operating Mode (bit 1:3) */
#define	PIT_MODE0	0x00	/* Interrupt on terminal count */
#define	PIT_MODE1	0x02	/* Hardware re-triggerable one-shot */
#define	PIT_MODE2	0x04	/* Rate generator */
#define	PIT_MODE3	0x06	/* Square wave generator */
#define	PIT_MODE4	0x08	/* Software triggered strobe */
#define	PIT_MODE5	0x0A	/* Hardware triggered strobe */
#define	PIT_MODE2X	0x0C	/* Same as Mode 2 */
#define	PIT_MODE3X	0x0E	/* Same as Mode 3 */
/* BCD or BIN (bit 0) */
#define	PIT_BIN	0	/* Use Binary-Coded Decimal */
#define	PIT_BCD	1	/* Use Binary */
/* Default frequency of PIT (1193180 Hz) */
#define	PIT_FREQUENCY	1193180
/* Configuration of PIT */
struct pit_config{
	uint32_t frequency;
};

extern int pit_init(void *config);
#endif
