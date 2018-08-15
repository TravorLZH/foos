#ifndef	MEMORY_H
#define	MEMORY_H
#include <inttypes.h>
#define	TABLE_ADDR(pagtab)	(struct page*)(pagtab.page << 12)
#define	FRAME_ADDR(pag)		(pag.frame << 12)

#define	MAPPED_MEMORY	0x800000
#define	PAGE_SIZE	0x1000
#define	NPAGES		(MAPPED_MEMORY/PAGE_SIZE)

#define	P_WRITABLE	0x1
#define	P_USER		0x2

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
extern struct page *vmem_get(void *addr,struct page_table *dir);
extern void pmem_set(void *addr);
extern void pmem_clear(void *addr);
extern int pmem_test(void *addr);
extern int pmem_map(struct page *pg,void *phys,uint8_t flags);
extern int pmem_mapaddr(void *addr,void *phys,struct page_table*,uint8_t);
extern int pmem_init(void *reserved);
#endif
