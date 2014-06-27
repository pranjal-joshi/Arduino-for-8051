/*
	Author	:		Pranjal Joshi
	Date		:		27/6/2014
	Compiler:		Keil uV4
	This header file is used for Arduino similar programming for 8051/8052 microcontrollers that allow easy
	and arduino style programming, but it will increase code size & may affect efficiency. Not recommended
	for timing critical applications or applications that require extremely fast program execution.
	This is open source software.
*/

// SPECIAL HEADER FILE FOR ADC & DAC (Using I2C chip NXP PCF)

#include <ardu51_i2c.h>

unsigned char ADC_addr = 0x90;

void setADCAddr(unsigned char ADDR)
{
	ADC_addr = ADDR;
}

unsigned char analogRead(unsigned char channel)		// Function to use ADC
{
	unsigned char analogValue;
	if(channel > 3 || channel < 0)
		return;
	i2c_begin();
	i2c_write(ADC_addr);
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	i2c_write(channel);
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	i2c_end();
	i2c_begin();
	i2c_write(ADC_addr + 1);
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	analogValue = i2c_read();
	i2c_end();
	return analogValue;
}

void DAC_write(unsigned char value)		// Function to use DAC
{
	i2c_begin();
	i2c_write(ADC_addr);
	i2c_write(0x40);
	i2c_write(value);
	i2c_end();
}