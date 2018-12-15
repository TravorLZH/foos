#ifndef	KMALLOC_H
#define	KMALLOC_H
#include <inttypes.h>
#define	HEAP_BASE	0x400000
#define	HEAP_SIZE	0x400000

#define	PAGE_ALIGN	12	// 4 KB Aligned

extern void *kmalloc(size_t sz);
extern void *kmalloca(size_t sz,size_t align);
extern void kfree(void *ptr);
#endif
