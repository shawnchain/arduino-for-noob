/*
 * LCD5110.h
 *
 *  Created on: 2013-3-26
 *      Author: shawn
 */

#ifndef LCD5110_H_
#define LCD5110_H_

//display mode -- normal / highlight
//#define TEXT_NORMAL 0
//#define TEXT_HIGHLIGHT 1
#define OFF false
#define ON true

#define uchar unsigned char

#include <stdint.h>

class LCD5110 {
public:
	LCD5110(int8_t RST, int8_t CE, int8_t DC, int8_t Din, int8_t Clk, int8_t BL);

	void init(void);
	void setBacklight(bool on);
	bool backlight(void);

	void drawBitmapPixel(uchar X,uchar Y,uchar *map, uchar Pix_x,uchar Pix_y);

	void drawString(uchar X,uchar Y,const char *s, bool highlight = false);
	void drawStringLarge ( uchar X,uchar Y, char *string, bool highlight = false);
	void drawChar(uchar c, bool highlight);
	void drawCharLarge (uchar X,uchar Y, uchar ch, bool highlight = false);
	void drawTitle(const char* title);

	void clear(void);

private:
	void setPosition(uchar X, uchar  Y);
	void sendByte(uchar dat, uchar dat_type);
//	void LCD_prop_write_string(uchar X,uchar Y,char *s, char mode); // new function1
//	uchar LCD_prop_write_char(uchar c, char mode);//new function2
	void LCD_write_chinese(uchar X, uchar Y,uchar *c,uchar ch_with,uchar num,uchar line,uchar row);
	void LCD_putchar(char c);

private:
	char SPI_SCK;	//Serial Clock(Master Output)
	char SPI_MOSI;	//Master Output,Slave Input
	char LCD_DC;		//Data/Command(command active low)
	char SPI_CS;		//Chip Select,Slave Transmit Enable(active low,Master Output)
	char LCD_RST;	//One Reset button
	char LCD_BL;		//Backlit control

	uchar NUM_COL;
	uchar NUM_ROW;
	uchar prev_char;
	bool highlighted;
	uchar x;

	bool _backlight;
};

// FONTS
//extern uchar* FONT_6X8;
//extern uchar* FONT_HUGE;
#endif /* LCD5110_H_ */
