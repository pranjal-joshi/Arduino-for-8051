
/*
	Arduino51 LCD example.
	Author	: Pranjal Joshi
	Date		: 16-6-2014
	This example uses arduino style programming for interfacing LCD with 8051.
	LCD is interfaced in 4-bit mode.
	Just call lcd_begin() function at setup, software will take
	care of complex signal generation.
*/

// CPU freq in Hz.. Required for delay routine
#define F_OSC 11059200

//*******************************
			#define lcd_port P1			/****** <<<<-------<< IMPORTANT!
//*******************************
// MUST SPECIFY THE PORT TO WHICH LCD IS CONNECTED
// FIRST 6 PINS OF GIVEN PORT WILL BE USED FOR LCD.
// MUST DEFINE THIS BEFORE INCLUDING HEADER FILES.....
/*
		Hardware mapping:
		
		Px.0	:		D4
		Px.1	:		D5
		Px.2	:		D6
		Px.3	:		D7
		Px.4	:		RS
		Px.5	:		EN
		
		Px is any port which is defined in program.
		(Connect R/W pin to ground.)
		
		NOTE:		You can still use remaining pins of Px.
		That is Px.6 & Px.7 can be used for other applications.
		LCD data won't interfere with remaining pins!
*/

#include <arduino51.h>				// basic file for Arduino51.
#include <ardu_lcd51.h>				// LCD header for Arduino51.


void setup()
{
	lcd_begin();			// must be called once in setup.
}

void loop()
{
	/*
	There is lot of similarity in the code written below
	with respect to Arduino.
	Please read header file for more functions.
	*/
	
	lcd_print(" Arduino51  LCD");
	lcd_setCursor(2,6);
	lcd_print("Demo");
	delay(1500);
	lcd_shiftRight(13,100);		// shiftRight( HowMany times to shift? , Delay between each shifting)
	lcd_shiftLeft(13,100);		// shiftLeft( HowMany times to shift? , Delay between each shifting)
	delay(500);
	lcd_displayOff();
	delay(500);
	lcd_displayOn();
	delay(500);
	lcd_displayOff();
	delay(500);
	lcd_displayOn();
	delay(500);
	lcd_clear();
	delay(1500);
}