/*
	Author	:		Pranjal Joshi
	Date		:		18/6/2014
	Compiler:		Keil uV4
	This header file is used for Arduino similar programming for 8051/8052 microcontrollers that allow easy
	and arduino style programming, but it will increase code size & may affect efficiency. Not recommended
	for timing critical applications or applications that require extremely fast program execution.
	This is open source software.
*/

// SPECIAL HEADER FILE FOR I2C COMMUNICATION BUS. (BIT-BANGING METHOD)

/*
	 NOTE: Never use PIN21 (P2.0) & PIN22 (P2.1) as I/O pins when I2C bus is active.
	 DigitalWrite called for this pins will cause garbage data transfer between I2C device & CPU.
*/

#include <arduino51.h>
#include <intrins.h>

unsigned char I2C_COUNTER;

#define ACK 0
#define NAK 1

sbit SDA = P2^0;
sbit SCL = P2^1;

void i2c_init()
{
	SDA = 1;
	SCL = 1;
}

void i2c_begin()
{
	i2c_init();
	SCL = 1;
	_nop_();
	_nop_();
	SDA = 0;
	_nop_();
	_nop_();
	SCL = 0;
	_nop_();
	_nop_();
}

void i2c_restart()
{
	SDA = 1;
	_nop_();
	_nop_();
	SCL = 1;
	_nop_();
	_nop_();
	SDA = 0;
	_nop_();
	_nop_();
}

void i2c_end()
{
	SDA = 0;
	_nop_();
	_nop_();
	SCL = 1;
	_nop_();
	_nop_();
	SDA = 1;
	_nop_();
	_nop_();
}

void i2c_ack()
{
	SDA = 0;
	_nop_();
	_nop_();
	SCL = 1;
	_nop_();
	_nop_();
	SCL = 0;
	_nop_();
	_nop_();
}

void i2c_nak()
{
	SDA = 1;
	_nop_();
	_nop_();
	SCL = 1;
	_nop_();
	_nop_();
	SCL = 0;
	_nop_();
	_nop_();
}

unsigned char i2c_write(unsigned char val)
{
	bit ack_bit;
	for(I2C_COUNTER=0;I2C_COUNTER<8;I2C_COUNTER++)
	{
		SCL = 0;
		_nop_();
		if((val & 0x80) == 0)
			SDA = 0;
		else
			SDA = 1;
		SCL = 1;
		_nop_();
		val <<= 1;
	}
	SCL = 0;
	ack_bit = SDA;
	SCL = 1;
	_nop_();
	_nop_();
	SCL = 0;
	return !(ack_bit);
}

unsigned char i2c_read(bit ack_bit)
{
	unsigned char value = 0;
	for(I2C_COUNTER=0;I2C_COUNTER<8;I2C_COUNTER++)
	{
		SCL = 0;
		_nop_();
		SCL = 1;
		_nop_();
		if(SDA)
			value |= 0x01;
		else
			value &= 0xFE;
		if(I2C_COUNTER < 7)
			value <<= 1;
	}
	/*if(ack_bit == 1)
		SDA = 0;
	else
		SDA = 1;*/

	SCL = 0;
	_nop_();
	SDA = 1;
	_nop_();
	return value;
}