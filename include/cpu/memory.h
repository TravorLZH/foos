#ifndef	MEMORY_H
#define	MEMORY_H
#include <foos/system.h>

#define	MAPPED_MEMORY	0x1000000
#define	PAGE_SIZE	0x1000
#define	NPAGES		(MAPPED_MEMORY/PAGE_SIZE)

#define	P_PRESENT	0x1
#define	P_WRITABLE	0x2
#define	P_USER		0x4
#define	P_ACCESSED	0x8
#define	P_DIRTY		0x10	/* Only used by page table entry */

extern int vmem_init(void *reserved);
extern uint32_t *vmem_get(void *addr,void *dir);

extern int pmem_init(void *reserved);
extern void pmem_set(void *addr);
extern void pmem_clear(void *addr);
extern int pmem_test(void *addr);
extern int pmem_map(uint32_t *pg,void *phys,uint32_t flags);
extern int pmem_mapaddr(void *addr,void *phys,uint32_t flags,uint32_t *dir);
extern void *pmem_get_free(void);
#endif
