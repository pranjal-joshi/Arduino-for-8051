/*
	Author	:		Pranjal Joshi
	Date		:		15/6/2014
	Compiler:		Keil uV4
	This header file is used for Arduino similar programming for 8051/8052 microcontrollers that allow easy
	and arduino style programming, but it will increase code size & may affect efficiency. Not recommended
	for timing critical applications or applications that require extremely fast program execution.
	This is open source software.
*/

// SPECIAL HEADER FILE FOR SERIAL COMMUNICATION

/*
	NOTES:
		*** MINIMUM BAUDRATE FOR 29.4912MHz IS 19200. (FOR 9600 USE 11.0592MHz OSC.).
		*** MAXIMUM BAUDRATE WITH 29.4912MHz IS 115200.
*/

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

void serial_end()
{
	TR2 &= 0;
}