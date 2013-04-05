/*
 * LCD5110.cpp - Based on code from http://www.arduinodev.com/src/minicon/LCD4884.cpp.html
 *
 *  Created on: 2013-3-26
 *      Author: shawn
 */

#include "LCD5110.h"
#include "Arduino.h"
#include "LCDFonts.h"

#define sendCommand(x) sendByte(x,0)
#define sendData(x) sendByte(x,1)

//LCD5110::LCD5110() {
//	// SPI Interface --- (using Arduino Digital Pin 2,3,4,5,6)
//	SPI_SCK = 5;//2;     //Serial Clock(Master Output)
//	SPI_MOSI = 4;//3;   //Master Output,Slave Input
//	LCD_DC = 3;//4;   //Data/Command(command active low)
//	SPI_CS = 2;//5;  //Chip Select,Slave Transmit Enable(active low,Master Output)
//	LCD_RST = A3;//;6; //One Reset button
//	LCD_BL = 6;//7; //Backlit control (Arduino DIO Pin 7)
//
//	prev_char = 0;
//	NUM_COL = 14;
//	NUM_ROW = 5;
//	x = 0;
//	highlighted = false;
//	_backlight = false;
//}

LCD5110::LCD5110(int8_t RST = A3, int8_t CE = 2, int8_t DC = 3, int8_t Din = 4, int8_t Clk = 5, int8_t BL = 6){
	// SPI Interface --- (using Arduino Digital Pin 2,3,4,5,6 by default)
	SPI_SCK = Clk;//2;     //Serial Clock(Master Output)
	SPI_MOSI = Din;//3;   //Master Output,Slave Input
	LCD_DC = DC;//4;   //Data/Command(command active low)
	SPI_CS = CE;//5;  //Chip Select,Slave Transmit Enable(active low,Master Output)
	LCD_RST = RST;//;6; //One Reset button
	LCD_BL = BL;//7; //Backlit control (Arduino DIO Pin 7)

	prev_char = 0;
	NUM_COL = 14;
	NUM_ROW = 5;
	x = 0;
	highlighted = false;
	_backlight = false;
}

void LCD5110::setBacklight(bool on) {
	if(_backlight == on){
		return;
	}
	_backlight = on;
	if (on)
		digitalWrite(LCD_BL, HIGH);
	else
		digitalWrite(LCD_BL, LOW);
}

bool LCD5110::backlight(void){
	return _backlight;
}

void LCD5110::init(void) {
	// Set pins
	pinMode(SPI_SCK,OUTPUT);
	digitalWrite(SPI_SCK,LOW);
	pinMode(SPI_MOSI,OUTPUT);
	digitalWrite(SPI_MOSI,LOW);
	pinMode(LCD_DC,OUTPUT);
	digitalWrite(LCD_DC,LOW);
	pinMode(SPI_CS,OUTPUT);
	digitalWrite(SPI_CS,LOW);


	pinMode(LCD_BL,OUTPUT);
	digitalWrite(LCD_BL,LOW);

	// Reset
	if(LCD_RST>0){
		pinMode(LCD_RST,OUTPUT);
		digitalWrite(LCD_RST, LOW);
		delayMicroseconds(1);
		digitalWrite(LCD_RST, HIGH);
	}

	// Initialize
	digitalWrite(SPI_CS, LOW);
	delayMicroseconds(1);
	digitalWrite(SPI_CS, HIGH);
//	delayMicroseconds(1);
//	digitalWrite(LCD_BL, HIGH);

	sendCommand(0x21);
	sendCommand(0xc0);
	sendCommand(0x06);
	sendCommand(0x13);
	sendCommand(0x20);
	clear();
	sendCommand(0x0c);

	digitalWrite(SPI_CS, LOW);
}

void LCD5110::sendByte(uchar dat, uchar dat_type) {
	digitalWrite(SPI_CS, LOW);
	if (dat_type == 0)
		digitalWrite(LCD_DC, LOW);
	else
		digitalWrite(LCD_DC, HIGH);

	for (uchar i = 0; i < 8; i++) {
		if (dat & 0x80) {
			digitalWrite(SPI_MOSI, HIGH);
		} else {
			digitalWrite(SPI_MOSI, LOW);
		}
		digitalWrite(SPI_SCK, LOW);
		dat = dat << 1;
		digitalWrite(SPI_SCK, HIGH);
	}
	digitalWrite(SPI_CS, HIGH);
}

void LCD5110::drawBitmapPixel(uchar X, uchar Y,
		uchar *map, uchar Pix_x, uchar Pix_y) {
	unsigned int i, n;
	uchar row;

	if (Pix_y % 8 == 0)
		row = Pix_y / 8;
	else
		row = Pix_y / 8 + 1;

	for (n = 0; n < row; n++) {
		setPosition(X, Y);
		for (i = 0; i < Pix_x; i++) {
			sendData(map[i + n * Pix_x]);
		}
		Y++;
	}
}

void LCD5110::drawString(uchar X, uchar Y, const char *s,
		bool highlight) {
	setPosition(X, Y);
	while (*s) {
		drawChar(*s, highlight);
		s++;
	}
}

void LCD5110::LCD_write_chinese(uchar X, uchar Y,
		uchar *c, uchar ch_with, uchar num,
		uchar line, uchar row) {
	setPosition(X, Y);
	for (uchar i = 0; i < num;) {
		for (uchar n = 0; n < ch_with * 2; n++) {
			if (n == ch_with) {
				if (i == 0)
					setPosition(X, Y + 1);
				else
					setPosition((X + (ch_with + row) * i), Y + 1);
			}
			sendData(c[(i * ch_with * 2) + n]);
		}
		i++;
		setPosition((X + (ch_with + row) * i), Y);
	}
}

void LCD5110::drawStringLarge(uchar X, uchar Y,
		char *string, bool highlight) {
	while (*string) {
		drawCharLarge(X, Y, *string, highlight);
		if (*string++ == '.')
			X += 5;
		else
			X += 12;
	}
}

/* write char in big font */
void LCD5110::drawCharLarge(uchar X, uchar Y,
		uchar ch, bool highlight) {
	uchar i, j;
	uchar *pFont;
	uchar ch_dat;

	pFont = (uchar *) FONT_HUGE_NUMS;

	if (ch == '.')
		ch = 10;
	else if (ch == '+')
		ch = 11;
	else if (ch == '-')
		ch = 12;
	else
		ch = ch & 0x0f;

	for (i = 0; i < 3; i++) {
		setPosition(X, Y + i);

		for (j = 0; j < 16; j++) {
			ch_dat = pgm_read_byte(pFont+ch*48 + i*16 +j);
			sendData((!highlight) ? ch_dat : (ch_dat ^ 0xff));
		}
	}

}

void LCD5110::drawChar(uchar c, bool highlight = false) {
	uchar line;
	uchar *pFont;
	byte ch;

	pFont = (uchar *) FONT_6X8;
	c -= 32;

	for (line = 0; line < 6; line++) {
		ch = pgm_read_byte(pFont+c*6+line);
		sendData((!highlight) ? ch : (ch ^ 0xff));
	}
	x = (x + 1) % NUM_COL;
}

void LCD5110::setPosition(uchar X, uchar Y) {
	sendCommand(0x40 | Y);		// column
	sendCommand(0x80 | X);         // row
	x = X;
}

void LCD5110::clear(void) {
	unsigned int i;

	sendCommand(0x0c);
	sendCommand(0x80);

	for (i = 0; i < 504; i++)
		sendData(0);

	x = 0;
}

void LCD5110::drawTitle(const char* title) {
	for (char n = 0; n < NUM_COL; n++)
		drawChar(' ', true);
	drawString((NUM_COL - strlen(title)) * 3, 0, title, true);
}

void LCD5110::LCD_putchar(char c) {
	if (prev_char == 27) {
		switch (c) {
		case '0':
			clear();
			break;
		case '1':
			setPosition(0, 0);
			break;
		case '2':
			highlighted = false;
			break;
		case '3':
			highlighted = true;
			break;
		case '4':
			setBacklight(ON);
			break;
		case '5':
			setBacklight(OFF);
			break;
		}
	} else {
		switch (c) {
		case '\r':
			for (char n = x; n < NUM_COL; n++) {
				drawChar(' ', highlighted);
			}
			break;
		case '\n':
			break;
		default:
			drawChar(c, highlighted);
		}
	}
	prev_char = c;
}
