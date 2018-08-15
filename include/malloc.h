#ifndef	MALLOC_H
#define	MALLOC_H
#include <inttypes.h>
extern void *malloc(size_t sz);
extern void *calloc(size_t sz,size_t n);
extern void *realloc(void *ptr,size_t sz);
extern void free(void *ptr);
#endif
