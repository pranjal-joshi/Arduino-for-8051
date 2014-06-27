/*
	Arduino51 Serial (UART) example.
	Author	: Pranjal Joshi
	Date		: 18/6/2014
	This example uses arduino style programming for interfacing external
	I2C EEPROM with 8051.
	This allow to store & retrive data on EEPROM chip.
	
	Supported models: All ATMEL I2C EEPROM chips. (AT24Cxxx)
	E.g. AT24C04 or AT24C512B or AT24C1024
	
	DON'T FORGET TO SET I2C Addr. OF EEPROM IN CODE, IF U CHANGED IT THROUGH HARDWARE.
	
	HARDWARE PIN MAPPING:
			
			SDA --- PIN 21 (P2.0)
			SCL --- PIN 22 (P2.1)
			
			WARNING ::: Never digitalWrite on PIN 21 & 22 while I2C bus is active!
*/

#define F_OSC 11059200			// Define CPU freq.
#include <arduino51.h>			// basic header file
#include <ardu51_eeprom.h>	// header file to access i2c eeprom
#include <ardu51_serial.h>	// header file for serial communication

void setup()
{
	int x;
	char d='A';
	serial_begin(9600);		// start UART @ 9600bps (use 19200 or more for 29.4912MHz crystal)
	eeprom_setAddr(0xA0);	// EEPROM address on i2c bus. if not called then 0xA0 will be default.
	serial_println("I2C EEPROM Test: Supported models - AT24Cxxx");
	serial_println("Now erasing 26 bytes of EEPROM. (writing 0xFF)");
	eeprom_erase(0,26);		// write 0xFF to first 26 bytes of EEPROM memory.
	serial_println("Now writing A-Z on EEPROM...");
	for(x=0;x<26;x++)			// write A-Z alphabets on EEPROM.
	{
		eeprom_write(x,d);
		d++;
	}
	serial_println("Now reading 26 bytes from EEPROM...");
	for(x=0;x<26;x++)			// read 26 location i.e. 26 alphabets from eeprom
	{
		serialSend(eeprom_read(x));		// use serialSend function to print single char on serial terminal.
	}
	serial_println("\r\nTest completed!");
	// If you got A-Z alphabets on serial terminal then the test complited succesfully
	// If not then recheck the connections and re-compile code without errors.
}

void loop()
{
	// Do nothing!
}
