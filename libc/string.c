#include <string.h>

char *strrev(char *str)
{
	char *start=str;
	char *end=str;
	char tmp=0;
	while(*end++);
	end-=2;
	while(start<end){
		tmp=*start;
		*start++=*end;
		*end--=tmp;
	}
	return str;
}

char *strcpy(char *to,const char *from)
{
	char *s=to;
	while(*s++=*from++);
	return to;
}

int strcmp(const char *s1,const char *s2)
{
	int ret;
	while(!(ret=*s1 - *s2) && *s1)
	{
		s1++;
		s2++;
	}
	return ret;
}
