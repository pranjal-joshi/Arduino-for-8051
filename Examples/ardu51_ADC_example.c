/*
	Arduino51 analogRead (ADC) example.
	Author	: Pranjal Joshi
	Date		: 27/6/2014
	This example uses arduino style programming for PWM generation.
	This example uses PCF8591 (8-bit I2C ADC/DAC chip) for Analog to Digital conversion.

	NOTE:
				This example is only valid for NXP PCF8591 chip!
				You can add multiple chips with diffrent addresses for nore ADC/DAC channels.
				DON'T FORGET TO ADDRESS CHIP ON I2C BUS. (Default : 0x90)
				Both ADC & DAC have 8 bit resolution i.e. data is in the range of 0-255 (0x00 - 0xFF)[1 byte].
				
	Hardware Pin Mapping:
	
				analogRead(0) :		AIN0		(PIN 1 of the chip)
				analogRead(1) :		AIN1		(PIN 2 of the chip)
				analogRead(2) :		AIN2		(PIN 3 of the chip)
				analogRead(3) :		AIN3		(PIN 4 of the chip)
				
				DAC_write(value)	:		AOUT		(PIN 15 of the chip)

	Its recommended to Add 1uF capacitor between DAC output & GND for noise elimination.
*/

#define F_OSC 11059200				// define CPU freq.
#include <arduino51.h>				// basic header file
#include <ardu51_adc_dac.h>				// PCF8591 header file
#include <ardu51_serial.h>		// Serial comm. header file

unsigned char x=0;

void setup()
{
	serial_begin(9600);		//start serial @ 9600bps
	setADCAddr(0x90);
	/*
		set I2C addr of chip to 0x90.
		No need to call if using only 1 PCF8591 chip.
		Default Address is 0x90.
		Call only when using more than one chips.
		Calling this function with hardware defined address will switch
		the communication with particular chip.
		For e.g.
			setADCAddr(0x90);		--> use 1st PCF8591 as ADC
			unsigned char a = analogRead(2);
			setADCAddr(0x92);		--> use 2nd PCF8591 as DAC
			DAC_write(a);
	*/
}

void loop()
{
	serial_write(analogRead(3));		// read ADC channel & send data to serial port
	delay(1);
	x++;
	if(x < 255)
		DAC_write(x);				// write 8-bit data to DAC.
	else
		x = 0;
	delay(1);
	/*
		This will create a Sawtooth waveform on DAC output & data read
		from ADC will be displayed on serial.
	*/
}