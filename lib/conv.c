#include <string.h>
#include <stdlib.h>

int atoi(const char *nptr)
{
	char *x=(char*)nptr;
	int res=0;
	int sign=1;
	switch(x[0]){
	case '-':
		sign=-1;
	case '+':
		x++;
		break;
	}
	char *s=x+strlen(x);
	while(x<s){
		res=res*10+*x-'0';
		x++;
	}
	return sign*res;
}

void itoa(int n,char *s)
{
	char neg=0;
	char *x=s;
	if(n<0){
		neg=1;
		n=-n;
	}
	do{
		*x=n%10+'0';
		x++;
	}while(n/=10);
	if(neg){
		*x='-';
		x++;
	}
	*x='\0';
	strrev(s);
}
