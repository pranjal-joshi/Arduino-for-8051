/*
	Author	:		Pranjal Joshi
	Date		:		15/6/2014
	Compiler:		Keil uV4
	This header file is used for Arduino similar programming for 8051/8052 microcontrollers that allow easy
	and arduino style programming, but it will increase code size & may affect efficiency. Not recommended
	for timing critical applications or applications that require extremely fast program execution.
	This is open source software.
*/

// SPECIAL HEADER FILE FOR I2C EEPROM COMMUNICATION (SUPPORTS Atmel AT24CXXX models).

/*
	 NOTE: Never use PIN21 (P2.0) & PIN22 (P2.1) as I/O pins when I2C bus is active.
	 DigitalWrite called for this pins will cause garbage data transfer between I2C device & CPU.
*/

#include <arduino51.h>
#include <ardu51_i2c.h>

#define ACK 0
#define NAK 1

unsigned char eeprom_addr = 0xA0;			//default for AT24Cxx i2c eeprom.
unsigned char EEPROM_COUNTER;

void eeprom_setAddr(unsigned char addr)
{
	eeprom_addr = addr;
}

void eepromSetMemoryAddr(unsigned int addr)
{
	i2c_begin();
	i2c_write(eeprom_addr);
	i2c_write((unsigned char)(addr >> 8) & 0xFF);
	i2c_write((unsigned char)addr & 0xFF);
}

void eeprom_write(unsigned int addr, unsigned char val)
{
	eepromSetMemoryAddr(addr);
	_nop_();
	_nop_();
	i2c_write(val);
	_nop_();
	_nop_();
	i2c_end();
	delay(25);
}

void eeprom_writeBytes(unsigned int addr, unsigned char *val)
{
	eepromSetMemoryAddr(addr);
	for(EEPROM_COUNTER=0;val[EEPROM_COUNTER]!=0;EEPROM_COUNTER++)
	{
		i2c_write(val[EEPROM_COUNTER]);
	}
	i2c_end();
}

unsigned char eeprom_read(unsigned int addr)
{
	unsigned int receivedData;
	i2c_begin();
	i2c_write(eeprom_addr);
	eepromSetMemoryAddr(addr);
	i2c_begin();
	i2c_write(eeprom_addr + 1);
	receivedData = i2c_read();
	return receivedData;
}

void eeprom_erase(unsigned int startAddr, unsigned int lastAddr)
{
	for(startAddr=startAddr;startAddr<lastAddr+1;startAddr++)
		eeprom_write(startAddr,0xFF);
}
