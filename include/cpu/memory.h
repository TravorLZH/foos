#ifndef	MEMORY_H
#define	MEMORY_H
#include <inttypes.h>
#define	TABLE_ADDR(pagtab)	(struct page*)(pagtab.page << 12)
#define	FRAME_ADDR(pag)		(pag.frame << 12)

struct page{
	uint8_t present:1;
	uint8_t writable:1;
	uint8_t user:1;
	uint8_t accessed:1;
	uint8_t dirty:1;
	uint8_t unused:7;
	uint32_t frame:20;
} __attribute__((packed));

struct page_table{
	uint8_t present:1;
	uint8_t writable:1;
	uint8_t user:1;
	uint8_t accessed:1;
	uint8_t unused:8;
	uint32_t page:20;
} __attribute__((packed));

extern int vmem_init(void *reserved);
#endif
