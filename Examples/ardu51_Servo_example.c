/*
	Arduino51 Servo Control example.
	Author	: Pranjal Joshi
	Date		: 30/6/2014
	This example uses arduino style programming for PWM generation.
	Timer 1 interrupt is used for Servo Signal generation.
	
	PWM pin map:
	
	servo39_write(ANGLE_VALUE [0 - 180])		-			Servo on PIN39
	servo38_write(ANGLE_VALUE [0 - 180])		-			Servo on PIN38
	servo37_write(ANGLE_VALUE [0 - 180])		-			Servo on PIN37  --> less accuracy due to latency period on this pin!
	
	Copyright (C) 2014.		Pranjal P. Joshi. <joshi.pranjal5@gmail.com>

*/
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

#define F_OSC 11059200
#include <arduino51.h>
#include <ardu51_servo.h>			// header file for servo control

void setup()
{
	servo39_write(90);			// can be also called inside loop() or any other function.
	servo38_write(120);			// can be also called inside loop() or any other function.
	servo37_write(50);			// can be also called inside loop() or any other function.
}

void loop()
{	
	////////////////////////////////////////////////////////////////
	// If you want to access the PORT 1 (PINS 32 - 39), you can
	// call servo_detach();
	// This will stop servo signal on port & port can be used for IO.
	// To re-enable the servo signal, just call servoXX_write(angle);
	/////////////////////////////////////////////////////////////////
	
}