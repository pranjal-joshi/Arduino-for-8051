/*
	Author	:		Pranjal Joshi
	Date		:		20/5/2014
	Compiler:		Keil uV4
	This header file is used for Arduino similar programming for 8051/8052 microcontrollers that allow easy
	and arduino style programming, but it will increase code size & may affect efficiency. Not recommended
	for timing critical applications or applications that require extremely fast program execution.
	This is open source software.
*/
#include<reg52.h>

#ifndef __ARDUINO51_H__
#define __ARDUINO51_H__

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

/*unsigned char digitalRead(unsigned char pin)
{
	
}*/


#endif