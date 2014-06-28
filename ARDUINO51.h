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
		*** CPU frequency must be 29.4912MHz or 11.0592MHz to obtain correct delay results.
		*** ANY CHANGES IN OSC. FREQ WILL CAUSE GARBAGE DATA ON SERIAL.
		*** ANY CHANGES IN OSC. FREQ WILL DISTURB DELAY ROUTINES.
		*** MINIMUM BAUDRATE FOR 29.4912MHz IS 19200. (FOR 9600 USE 11.0592MHz OSC.).
		*** MAXIMUM BAUDRATE WITH 29.4912MHz IS 115200.
*/
#include <reg52.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __ARDUINO51_H__
#define __ARDUINO51_H__

//#define F_OSC 29491200 //11059200 //29491200	// oscillator freq. DEFINE IN CODE <<<---

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
/* --- PWM Pins (Port 0) --- */
sbit PWM39 = P0^0;
sbit PWM38 = P0^1;
sbit PWM37 = P0^2;
sbit PWM36 = P0^3;
sbit PWM35 = P0^4;
sbit PWM34 = P0^5;

unsigned long TIMER_VALUE, ONE_MS_VALUE;
double T_MACHINE, F_MACHINE;
unsigned int TIMER_VALUE_LOW, TIMER_VALUE_HIGH;

unsigned char PWM_VAL_39,PWM_VAL_38,PWM_VAL_37,PWM_VAL_36,PWM_VAL_35,PWM_VAL_34;
bit PWM_FLAG_39,PWM_FLAG_38,PWM_FLAG_37,PWM_FLAG_36,PWM_FLAG_35,PWM_FLAG_34;
bit isAnalogWriteActive = 0;
volatile unsigned char ovf_cnt = 0;

void setup(void);		// executes only once
void loop(void);		// executes infinitely

/* This char array helps to convert pin map to port*/
code const unsigned char pinToPort[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
code const unsigned char pinToPortInverted[] = { 128, 64, 32, 16, 8, 4, 2, 1 };


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
		p = pinToPortInverted[pin - 32];
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
	if(pin > 9 && pin < 18 )
	{
		digitalWrite(pin,LOW);
		readlow = P3;
		digitalWrite(pin,HIGH);
		readhigh = P3;
		if(readlow != readhigh)
			return 1;
		else
			return 0;
	}
	if(pin > 20 && pin < 29 )
	{
		digitalWrite(pin,LOW);
		readlow = P2;
		digitalWrite(pin,HIGH);
		readhigh = P2;
		if(readlow != readhigh)
			return 1;
		else
			return 0;
	}
	if(pin > 31 && pin < 40 )
	{
		digitalWrite(pin,LOW);
		readlow = P0;
		digitalWrite(pin,HIGH);
		readhigh = P0;
		if(readlow != readhigh)
			return 1;
		else
			return 0;
	}
}

void delayTimerValueCalculation()
{
	F_MACHINE = (F_OSC/12);
	T_MACHINE = (1/F_MACHINE);
	ONE_MS_VALUE = (0.001/T_MACHINE);
	TIMER_VALUE = (0xFFFF - ONE_MS_VALUE);
	TIMER_VALUE_LOW = (0x00FF & TIMER_VALUE);
	TIMER_VALUE_HIGH = (0xFF00 & TIMER_VALUE);
	//TIMER_VALUE_HIGH = (TIMER_VALUE_HIGH >> 8);
}

void delay(unsigned long millisec)
{
	unsigned char tlow,thigh;
	bit isInterrupt;
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
	// following loop increment added by trial-&-error method.
	// You may change the multiplier to obtian more accurate
	// result depending on your application.
	if(F_OSC == 29491200)
		millisec *= 10;
	else if(F_OSC == 11059200)
		millisec *= 4;
	for(cnt=0;cnt<=millisec;cnt++)
	{
		TH0 = TIMER_VALUE_HIGH;
		TL0 = TIMER_VALUE_LOW;
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

void start_analogWrite()
{
	if(!(isAnalogWriteActive))
	{
		TMOD |= 0x20;
		TH1 = 250;
		EA = 1;
		ET1 = 1;
		TR1 = 1;
		isAnalogWriteActive = 1;
	}
}

void analogWrite39(unsigned char pwm)
{
	PWM_VAL_39 = pwm;
	start_analogWrite();
}

void analogWrite38(unsigned char pwm)
{
	PWM_VAL_38 = pwm;
	start_analogWrite();
}

void analogWrite37(unsigned char pwm)
{
	PWM_VAL_37 = pwm;
	start_analogWrite();
}

void analogWrite36(unsigned char pwm)
{
	PWM_VAL_36 = pwm;
	start_analogWrite();
}

void analogWrite35(unsigned char pwm)
{
	PWM_VAL_35 = pwm;
	start_analogWrite();
}

void analogWrite34(unsigned char pwm)
{
	PWM_VAL_34 = pwm;
	start_analogWrite();
}

void timer1_ovf() interrupt 3
{
	if(ovf_cnt == 255)
		ovf_cnt = 0;
	
	ovf_cnt++;
	
	if(ovf_cnt > PWM_VAL_39)
		PWM39 = 0;
	else
		PWM39 = 1;
	
	if(ovf_cnt > PWM_VAL_38)
		PWM38 = 0;
	else
		PWM38 = 1;
	
	if(ovf_cnt > PWM_VAL_37)
		PWM37 = 0;
	else
		PWM37 = 1;
	
	if(ovf_cnt > PWM_VAL_36)
		PWM36 = 0;
	else
		PWM36 = 1;
	
	if(ovf_cnt > PWM_VAL_35)
		PWM35 = 0;
	else
		PWM35 = 1;
	
	if(ovf_cnt > PWM_VAL_34)
		PWM34 = 0;
	else
		PWM34 = 1;
}

void clear_analogWrite()
{
	ET1 = 0;
	TR1 = 0;
	isAnalogWriteActive = 0;
}

void main()
{
	delayTimerValueCalculation();
	setup();
	while(1)
	{
		loop();
	}
}
#endif