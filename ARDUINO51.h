/*
	Author	:		Pranjal Joshi
	Date		:		20/5/2014
	Compiler:		Keil uV4
	This header file is used for Arduino similar programming for 8051/8052 microcontrollers that allow easy
	and arduino style programming, but it will increase code size & may affect efficiency. Not recommended
	for timing critical applications or applications that require extremely fast program execution.
	This is open source software.
*/

/*
	NOTES:
		CPU frequency must be 29.4912MHz crystal to obtain correct results.
		ANY CHANGES IN OSC. FREQ WILL CAUSE GARBAGE DATA ON SERIAL.
		ANY CHANGES IN OSC. FREQ WILL DISTURB DELAY ROUTINES.
*/
#include <reg52.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __ARDUINO51_H__
#define __ARDUINO51_H__

#define F_OSC 29491200	// oscillator freq.

#define HIGH 1 			// high means 1
#define LOW 0				// low means 0

/* --- PORT 1 --- */
#define PIN0 0
#define PIN1 1
#define PIN2 2
#define PIN3 3
#define PIN4 4
#define PIN5 5
#define PIN6 6
#define PIN7 7
/* --- PORT 3 --- */
#define PIN10 10
#define PIN11 11
#define PIN12 12
#define PIN13 13
#define PIN14 14
#define PIN15 15
#define PIN16 16
#define PIN17 17
/* --- PORT 2 --- */
#define PIN21 21
#define PIN22 22
#define PIN23 23
#define PIN24 24
#define PIN25 25
#define PIN26 26
#define PIN27 27
#define PIN28 28
/* --- PORT 0 --- */
#define PIN32 32
#define PIN33 33
#define PIN34 34
#define PIN35 35
#define PIN36 36
#define PIN37 37
#define PIN38 38
#define PIN39 39

volatile unsigned char uart_read[32];
volatile unsigned char uart_read_count = 0;

void setup(void);		// executes only once
void loop(void);		// executes infinitely

/* This char array helps to convert pin map to port*/
code const unsigned char pinToPort[] = { 1, 2, 4, 8, 16, 32, 64, 128 };

/*
This function write 1 or 0 to particular pin.
Physical pin numbers of chip & software pin mapping is kept same to avoid any confilicts.
*** NOTE : Alternately, pins can be accessed by numbers. i.e. digitalWrite(PIN0,HIGH) is same
		as of digitalWrite(0,HIGH)
*/
void digitalWrite(unsigned char pin, unsigned char val)
{
	unsigned char p;
	if(pin > -1 && pin < 8 )
	{
		p = pinToPort[pin];
		if(val == HIGH)
		{
			P1 |= p;
		}
		else if(val == LOW)
		{
			p = 0xFF - p;
			P1 &= p;
		}
	}
	else if(pin > 9 && pin < 18)
	{
		p = pinToPort[pin - 10];
		if(val == HIGH)
		{
			P3 |= p;
		}
		else if(val == LOW)
		{
			p = 0xFF - p;
			P3 &= p;
		}
	}
	else if(pin > 20 && pin < 29)
	{
		p = pinToPort[pin - 21];
		if(val == HIGH)
		{
			P2 |= p;
		}
		else if(val == LOW)
		{
			p = 0xFF - p;
			P2 &= p;
		}
	}
	else if(pin > 31 && pin < 40)
	{
		p = pinToPort[pin - 32];
		if(val == HIGH)
		{
			P0 |= p;
		}
		else if(val == LOW)
		{
			p = 0xFF - p;
			P0 &= p;
		}
	}
}

unsigned char digitalRead(unsigned char pin)
{
	unsigned char readlow,readhigh;
	if(pin > -1 && pin < 8 )
	{
		digitalWrite(pin,LOW);
		readlow = P1;
		digitalWrite(pin,HIGH);
		readhigh = P1;
		if(readlow != readhigh)
			return 1;
		else
			return 0;
	}
}

void delay(unsigned long millisec)
{
	unsigned char tlow,thigh,isInterrupt;
	unsigned int cnt;
	isInterrupt = 0;
	TMOD |= 0x02;
	tlow = TL0;
	thigh = TH0;
	if(ET0 == 1)
	{
		ET0 = 0;
		isInterrupt = 1;
	}
	millisec *= 12;
	for(cnt=0;cnt<=millisec;cnt++)
	{
		TH0 = 0xF6;
		TL0 = 0x60;
		TR0 = 1;
		while(TF0 == 0);
		TR0 = 0;
		TF0 = 0;
	}
	TL0 = tlow;
	TH0 = thigh;
	if(isInterrupt == 1)
		ET0 = 1;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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
		uart_read[uart_read_count] = SBUF;
		uart_read_count++;
		SBUF = uart_read[uart_read_count];
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
	unsigned char temp,i,cnt = 0;
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

void main()
{
	setup();
	while(1)
	{
		loop();
	}
}
#endif