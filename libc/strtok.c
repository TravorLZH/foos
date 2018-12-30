#include <string.h>

static char *old=NULL;

char *strtok(char *str,const char *delim)
{
	const char *delim_start=delim;
	char *ptr=NULL;
	int f=1,i=0;
	str= str==NULL ? old : str;
	/* Skip the starting delimiters */
	str+=strspn(str,delim);
	/* Now entering a loop of looking up delimiters */
	for(;*str;str++){
		for(delim=delim_start;*delim;delim++){
			if(*str==*delim){
				*str='\0';
				i=1;
				break;
			}
		}
		if(*str!='\0' && f==1){
			ptr=str;
			f=0;
		}
		if(*str!='\0' && f==0 && i==1){
			old=str;
			return ptr;
		}
	}
	old=str;
	return ptr;
}
