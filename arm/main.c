#define	MWR(addr,v) \
	__asm__("str %[val],[%[at]]"::[val]"r"((v)), \
			[at]"r"((addr)));

void uart_sendbyte(char c)
{
	MWR(0x3F201000,c);
	if(c=='\n'){
		uart_sendbyte('\r');
	}
}

int uart_sendstring(const char *s)
{
	do{
		uart_sendbyte(*s);
	}while(*++s);
}

int entry(void)
{
	uart_sendstring("Hello world!\n");
	return 0;
}
