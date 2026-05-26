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
	while(!(ret=(unsigned char)*s1 - (unsigned char)*s2) && *s1)
	{
		s1++;
		s2++;
	}
	return ret;
}

int strncmp(const char *s1,const char *s2,size_t num)
{
	int ret;
	while(num && !(ret=(unsigned char)*s1 - (unsigned char)*s2) && *s1)
	{
		s1++;
		s2++;
		num--;
	}
	return num ? ret : 0;
}

char *strstr(const char *haystack,const char *needle)
{
	size_t needle_len=strlen(needle);
	if (needle_len==0)
		return (char *)haystack;

	while (*haystack) {
		if (*haystack==*needle &&
			strncmp(haystack,needle,needle_len)==0)
			return (char *)haystack;

		haystack++;
	}
	return NULL;
}

char *strnstr(const char *haystack,const char *needle,size_t len)
{
	size_t needle_len=strlen(needle);
	if (needle_len==0)
		return (char *)haystack;

	while (*haystack && len>=needle_len) {
		if (*haystack==*needle &&
			strncmp(haystack,needle,needle_len)==0)
			return (char *)haystack;

		haystack++;
		len--;
	}
	return NULL;
}

size_t strspn(const char *str,const char *accept)
{
	const char *a=NULL,*b=NULL;
	size_t count=0;
	for(a=str;*a;a++){
		for(b=accept;*b;b++){
			if(*a==*b)
				break;
		}
		if(*b=='\0')
			return count;
		count++;
	}
	return count;
}

void *memmem(const void *haystack,size_t haystack_len,
	const void *needle,size_t needle_len)
{
	size_t i;
	const unsigned char *h=haystack;
	const unsigned char *n=needle;

	if (needle_len==0)
		return (void *)h;

	if (needle_len>haystack_len)
		return NULL;

	for (i=0;i<=haystack_len-needle_len;i++) {
		if (h[i]==n[0] &&
			memcmp(h+i,n,needle_len)==0)
			return (void *)(h+i);
	}

	return NULL;
}