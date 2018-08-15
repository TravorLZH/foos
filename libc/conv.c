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

void utoa(uint32_t n,char *s)
{
	char *x=s;
	do{
		*x=n%10+'0';
		x++;
	}while(n/=10);
	*x='\0';
	strrev(s);
}

/* Hex conversions */
size_t num_hex_digits(size_t n)
{
	int ret=0;
	if(n==0)return 1;
	while(n!=0){
		n >>= 4;
		ret++;
	}
	return ret;
}

void itoh(uint32_t n,char* s)
{
	int len=num_hex_digits(n);
	const char hex_lookup[]="0123456789ABCDEF";

	if(len & 1){
		*s++ = '0';
	}
	s[len]='\0';
	for(--len; len>=0; n>>= 4, --len){
		s[len]=hex_lookup[n & 0xF];
	}
}

