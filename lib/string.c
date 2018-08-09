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
