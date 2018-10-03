void uart_sendbyte(char c)
{
	__asm__("ldr r0,=#0x3F201000\n"
		"str %[val],[r0]"::[val]"r"((long)c));
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
