/*
	Arduino51 Serial (UART) example.
	Author	: Pranjal Joshi
	Date		: 16/6/2014
	This example uses arduino style programming for UART or Serial interfacing with 8051.
	
	NOTES:
		*** MINIMUM BAUDRATE FOR 29.4912MHz IS 19200. (FOR 9600 USE 11.0592MHz OSC.).
		*** MAXIMUM BAUDRATE WITH 29.4912MHz IS 115200.
		
	///////////////////////// IMPORTANT ///////////////////////////////////////////////////
	// Here, 8052 uses Timer 2 with capture units so Timer 1 is free for other services. //
	// Timer 2 can provide high baudrate than timer 1.                                   //
	// This is the biggest advantage.                                                    //
	// THIS SERIAL COMM. WILL NOT WORK ON 8051 MCU.                                      //
	// All devices having Timer 2 are supported even they are named as 8051 series.      //
	// For eg. The code will work on AT89S52 & NXP P89V51Rxx series controllers.         //
	///////////////////////////////////////////////////////////////////////////////////////
*/

#define F_OSC 11059200	// CPU Freq. in Hz..
												//MUST BE DEFINED BEFROE INCLUDING HEADER FILES
												
#include <arduino51.h>			// basic header file
#include <ardu51_serial.h>	// header file for serial communication

unsigned char demo = 48;
unsigned int x = 1024;

void setup()
{
	serial_begin(9600);		// Must be called once in setup
}

void loop()
{
	serial_print("Arduino style programming on 8052.\n");
	serial_println("println means print & jump to new line. no need to add '\n' at the end.");
	
	
	serial_write(demo);			// sending single char value
												// this will print '0' on terminal
	
	serial_printInt(x);		// This will print Int on terminal.. BUG SOLVED! UPDATED!!
	
	delay(2000);					// wait for 2 seconds n loop back.
}