/*
	Author	:		Pranjal Joshi
	Date		:		30/6/2014
	Compiler:		Keil uV4
	This header file is used for Arduino similar programming for 8051/8052 microcontrollers that allow easy
	and arduino style programming, but it will increase code size & may affect efficiency. Not recommended
	for timing critical applications or applications that require extremely fast program execution.
	This is open source software.
	
	Copyright (C) 2014.		Pranjal P. Joshi. <joshi.pranjal5@gmail.com>
*/

// SPECIAL HEADER FILE FOR SERVO MOTOR INTERFACING

/*
		NOTE / WARNING:
		
		This library uses Timer 1 interrupt. It will automatically disable PWM
		or analogWrite. DONT INCLUDE THIS FILE IF YOU ARE USING PWM. (analogWrite())
		
		This library uses SOFTWARE PWM, so error of +/- 10 degress with respected
		to given angle is expected as the interrupt occurs at every 50th uS.
		Accuracy can be increased by faster interrupt, but it will adversly affect
		the speed of main subroutine.
		Works with any crystal freq. defined in program as F_OSC.
		More higher frequency, more accuracy.
		
		THE BASIC 8052 ARCHITECTURE IS NOT WELL SUITED FOR MULTIPLE SERVO CONTROL.
		STILL THIS LIBRARY ALLOWS YOU TO CONNECT 3 SERVOS AT SAME TIME.
		PLEASE OBTAIN REQUIRED ANGLES BY TRIAL-AND-ERROR METHOD!!!
*/

#ifndef __ARDU51_SERVO_H__
#define __ARDU51_SERVO_H__

#include <arduino51.h>

sbit servo39 = P0^0;
sbit servo38 = P0^1;
sbit servo37 = P0^2;

unsigned int FIFTY_US_VALUE;
unsigned char servo_timer_ovf_val;
bit isLowPeriod = 0;
unsigned int lowPeriod_cnt = 0;
unsigned char servoLowPort = 0xFF;
unsigned int ANGLE39,ANGLE38,ANGLE37;
volatile unsigned char servo_ovf_cnt = 20;

void servo_attach()
{
	if(!(isAnalogWriteActive) && !(isServoActive))
	{
		FIFTY_US_VALUE = (0.00005/T_MACHINE);
		servo_timer_ovf_val = 0xFF - FIFTY_US_VALUE;
		TMOD |= 0x20;
		TH1 = servo_timer_ovf_val;
		TL1 = servo_timer_ovf_val;
		EA = 1;
		ET1 = 1;
		TR1 = 1;
		isAnalogWriteActive = 0;
		isServoActive = 1;
	}
}

void servo_detach()
{
	ET1 = 0;
	TR1 = 0;
	ET1 = 0;
	isServoActive = 0;
}

void servo39_write(unsigned char angle)
{
	ANGLE39 = map(angle,0,180,1000,2000);
	ANGLE39 /= 50;
	servoLowPort |= 0x01;
	servo_attach();
}

void servo38_write(unsigned char angle)
{
	ANGLE38 = map(angle,0,180,1000,2000);
	ANGLE38 /= 50;
	servoLowPort |= 0x02;
	servo_attach();
}

void servo37_write(unsigned char angle)
{
	ANGLE37 = map(angle,0,180,1000,2000);
	ANGLE37 /= 50;
	servoLowPort |= 0x04;
	servo_attach();
}



void timer1_servo_ISR() interrupt 3
{
	if(isServoActive)
	{
		if(servo_ovf_cnt == 51)
		{
			servo_ovf_cnt = 20;
			isLowPeriod = 1;
		}
		
		servo_ovf_cnt++;
		
		if(ANGLE39 < servo_ovf_cnt)
			servo39 = 1;
		else
			servo39 = 0;

		if(ANGLE38 < servo_ovf_cnt)
			servo38 = 1;
		else
			servo38 = 0;
		
		if(ANGLE37 < servo_ovf_cnt)
			servo37 = 1;
		else
			servo37 = 0;
		
	}
	if(isLowPeriod)
	{
		isServoActive = 0;
		P1 = servoLowPort;
		lowPeriod_cnt++;
		if(lowPeriod_cnt == 360)
		{
			isServoActive = 1;
			isLowPeriod = 0;
			lowPeriod_cnt = 0;
		}
	}
}
#endif