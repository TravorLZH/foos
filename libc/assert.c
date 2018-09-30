#include <assert.h>

extern int printf(const char *,...);

void __assert_func(const char *file,const char *func,int line,
		const char *expr)
{
	printf("Assertion `%s' failed at %s(%s:%d)\n",expr,func,file,line);
	while(1);
}
