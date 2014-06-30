/*
	Arduino51 analogWrite (PWM) example.
	Author	: Pranjal Joshi
	Date		: 17-6-2014
	This example uses arduino style programming for PWM generation.
	Timer 1 interrupt is used for PWM generation.
	To stop PWM, call function "clear_analogWrite()".
	
	PWM pin map:
	
	analogWrite39(PWM_VALUE [0 - 255])		-			PWM on PIN39
	analogWrite38(PWM_VALUE [0 - 255])		-			PWM on PIN38
	analogWrite37(PWM_VALUE [0 - 255])		-			PWM on PIN37
	analogWrite36(PWM_VALUE [0 - 255])		-			PWM on PIN36
	analogWrite35(PWM_VALUE [0 - 255])		-			PWM on PIN35
	analogWrite34(PWM_VALUE [0 - 255])		-			PWM on PIN34
	
	Copyright (C) 2014.		Pranjal P. Joshi. <joshi.pranjal5@gmail.com>

*/

#define F_OSC 11059200				// CPU freq in Hz
#include <arduino51.h>				// basic file for Arduino51.


void setup()
{
}

void loop()
{
	unsigned char x;
	for(x=0;x<256;x++)		// This will create varing PWM on PIN34 to PIN39.
	{
		analogWrite39(x);		// PWM on respective pin
		analogWrite38(x);
		analogWrite37(x);
		analogWrite36(x);
		analogWrite35(x);
		analogWrite34(x);
	}
	clear_analogWrite();	// STOP PWM on all pins & free Timer 1 for user
	delay(1500);
}