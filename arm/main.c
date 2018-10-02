void serial_sendbyte(char c)
{
	__asm__("ldr r0,=#0x101f1000\n"
		"str %[val],[r0]"::[val]"r"((long)c));
}

int serial_sendstring(const char *s)
{
	do{
		serial_sendbyte(*s);
	}while(*++s);
}

int entry(void)
{
	serial_sendstring("Hello world!\n");
	return 0;
}
