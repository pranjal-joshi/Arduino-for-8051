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

#ifndef __ARDU51_SERIAL_H__
#define __ARDU51_SERIAL_H__

#include <Arduino51.h>
#include <math.h>
#include <intrins.h>

char serbuf[8] = {0,0,0,0,0,0,0,0};
char serbufInv[8] = {0,0,0,0,0,0,0,0};
unsigned char sercnt = 0;
unsigned char sercntInv = 0;

volatile unsigned char uart_read;
volatile bit isRxDataAvailable = 0;

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

void serialInterrupt() interrupt 4
{
	if(RI == 1)
	{
		RI = 0;
		uart_read = SBUF;
		isRxDataAvailable = 1;
	}
	else
		TI = 0;
}

void serial_write(unsigned char info)
{
	SBUF = info;
	_nop_();
	_nop_();
}

void serial_print(unsigned char *str)
{
	unsigned int x;
	for(x=0;str[x]!=0;x++)
		serial_write(str[x]);
}

void serial_println(unsigned char *str)
{
	unsigned int x;
	for(x=0;str[x]!=0;x++)
		serial_write(str[x]);
	serial_write('\n');
	serial_write('\r');
}

void serial_printlnInt(unsigned long num)
{
	sercnt = 0;
	sercntInv = 0;
	if(num != 0)
	{
		while(num > 0)
		{
			serbuf[sercnt] = (num % 10) + 48;
			num /= 10;
			sercnt++;
		}
		while(sercnt > 0)
		{
			sercnt--;
			serbufInv[sercntInv] = serbuf[sercnt];
			sercntInv++;
		}
		serial_println(serbufInv);
	}
	else
	{
		serial_println("0");
	}
}

void serial_printInt(unsigned long num)
{
	sercnt = 0;
	sercntInv = 0;
	if(num != 0)
	{
		while(num > 0)
		{
			serbuf[sercnt] = (num % 10) + 48;
			num /= 10;
			sercnt++;
		}
		while(sercnt > 0)
		{
			sercnt--;
			serbufInv[sercntInv] = serbuf[sercnt];
			sercntInv++;
		}
		serial_print(serbufInv);
	}
	else
	{
		serial_print("0");
	}
}

void serial_end()
{
	TR2 &= 0;
}

unsigned char serial_read()
{
	while(!isRxDataAvailable);
	isRxDataAvailable = 0;
	return uart_read;
}

#endif