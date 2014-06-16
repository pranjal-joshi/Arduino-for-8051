#include <Arduino51.h>

void serial_begin(unsigned long baudrate)
{
	unsigned long register_value;
	register_value = (65536 - (F_OSC/(baudrate * 32)));
	SCON = 0x50;
	T2CON &= 0xF0;
	T2CON |= 0x30;
	RCAP2L = (register_value & 0xFF);
	RCAP2H = (register_value >> 8) & 0xFF;
	TH2 = RCAP2H;
	TL2 = RCAP2L;
	ES = 1;
	EA = 1;
	TR2 = 1;
}

void serial_read() interrupt 4
{
	if(RI == 1)
	{
		RI = 0;
		uart_read = SBUF;
		uart_read_count++;
	}
	else
		TI = 0;
}

void serialSend(unsigned char info)
{
	SBUF = info;
	delay(1);
}

void serial_print(unsigned char *str)
{
	unsigned int x;
	for(x=0;str[x]!=0;x++)
		serialSend(str[x]);
}

void serial_println(unsigned char *str)
{
	unsigned int x;
	for(x=0;str[x]!=0;x++)
		serialSend(str[x]);
	serialSend('\n');
	serialSend('\r');
}

void serial_printlnInt(long num)
{
	char buf[8] = {0,0,0,0,
								 0,0,0,0};
	unsigned char cnt = 0;
	if(num != 0)
	{
		while(num > 0)
		{
			buf[cnt] = (num % 10) + 48;
			num /= 10;
			cnt++;
		}
	}
	serial_println(buf);
}

void serial_printInt(long num)
{
	char buf[8] = {0,0,0,0,
								 0,0,0,0};
	unsigned char cnt = 0;
	if(num != 0)
	{
		while(num > 0)
		{
			buf[cnt] = (num % 10) + 48;
			num /= 10;
			cnt++;
		}
	}
	serial_print(buf);
}