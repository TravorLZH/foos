/* Assert function for FOOS kernel */
#ifndef	ASSERT_H
#define	ASSERT_H

#ifndef	NDEBUG
#define	assert(expr)	(expr ? (void)0 : __assert_func( \
				__FILE__,__func__,__LINE__,#expr))
#else
#define	assert(expr)	((void)0)
#endif

extern void __assert_func(const char *,const char *,int,const char*);

#endif
