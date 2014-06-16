
#include <arduino51.h>

// Function definations :

void lcd_init(void);

void lcd_begin(unsigned char rs, unsigned char en, unsigned char d4, unsigned char d5, unsigned char d6, unsigned char d7)
{
	rs_pin = rs;
	en_pin = en;
	d4_pin = d4;
	d5_pin = d5;
	d6_pin = d6;
	d7_pin = d7;
	lcd_init();
}

void lcdWrite(unsigned char val)
{
	unsigned char temp;
	temp = val;
	val = ((val >> 4) & 0x0F);
	d4_pin_val = (val >> 0) & 0x01;
	d5_pin_val = (val >> 1) & 0x01;
	d6_pin_val = (val >> 2) & 0x01;
	d7_pin_val = (val >> 3) & 0x01;
	digitalWrite(d4_pin,d4_pin_val);
	digitalWrite(d5_pin,d5_pin_val);
	digitalWrite(d6_pin,d6_pin_val);
	digitalWrite(d7_pin,d7_pin_val);
	digitalWrite(en_pin,HIGH);
	delay(2);
	digitalWrite(en_pin,LOW);
	val = temp;
	val &= 0x0F;
	d4_pin_val = (val >> 0) & 0x01;
	d5_pin_val = (val >> 1) & 0x01;
	d6_pin_val = (val >> 2) & 0x01;
	d7_pin_val = (val >> 3) & 0x01;	
	digitalWrite(d4_pin,d4_pin_val);
	digitalWrite(d5_pin,d5_pin_val);
	digitalWrite(d6_pin,d6_pin_val);
	digitalWrite(d7_pin,d7_pin_val);
	digitalWrite(en_pin,HIGH);
	delay(2);
	digitalWrite(en_pin,LOW);
}

void lcdcmd(unsigned char cmd)
{
	digitalWrite(rs_pin,LOW);
	lcdWrite(cmd);
	delay(1);
}

void lcddata(unsigned char LCDdata)
{
	digitalWrite(rs_pin,HIGH);
	lcdWrite(LCDdata);
	delay(1);
}

void lcd_print(unsigned char *str)
{
	unsigned int x;
	for(x=0;str[x]!=0;x++)
		lcddata(str[x]);
}

void lcd_init()
{
	lcdcmd(0x28);
	lcdcmd(0x0C);
	lcdcmd(0x06);
	lcdcmd(0x80);
}