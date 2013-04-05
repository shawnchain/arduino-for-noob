// Do not remove the include below
#include "RFID_Demo.h"

#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

// the sensor communicates using SPI, so include the library:
#include <SPI.h>

#include <RFID/MFRC522.h>
#include <LCD/LCD5110.h>
#include <Keypad/AnalogKeypad.h>
#include "Menu.h"

// Choose different Arduino boards
#define MEGA_2560 1
#define PROMINI_328 0

extern "C"{
	bool onKeyPressed(Keycode keycode);
}

// LCD Pin setup
#if MEGA_2560
#define LCD_RST 6
#define LCD_CE 5
#define LCD_DC 4
#define LCD_DIN 3
#define LCD_CLK 2
#define LCD_BL 7
#endif

#if PROMINI_320
#define LCD_RST A3
#define LCD_CE 2
#define LCD_DC 3
#define LCD_DIN 4
#define LCD_CLK 5
#define LCD_BL 6
#endif

LCD5110 lcd(LCD_RST, LCD_CE, LCD_DC, LCD_DIN, LCD_CLK, LCD_BL);


// RFID Pin setup
// 2560 mega
#if MEGA_2560
#define RFID_SS 53  // SPI device select
#define RFID_RST A5 // device reset
// The beep pin
#define PinBEEP A9
#endif
// 328 pro mini
#if PROMINI_328
#define RFID_SS 10  // SPI device select
#define RFID_RST 9 // device reset
// The beep pin
#define PinBEEP A2
#endif
MFRC522 rfid(RFID_SS,RFID_RST);


// Keypad setup
#define KEYPAD_INPUT A0
AnalogKeypad keypad(KEYPAD_INPUT);

extern "C"{
bool mHello(void* item){
	char buf[128];
	snprintf(buf,128,"Greetings from menu %s",((MenuItem*)item)->title);
	Serial.println(buf);
	return true;
}
}

MenuItem menu_a1[] = {
		{"A-1-1","",mHello,0},
		{"A-1-2","",mHello,0},
		{"A-1-3","",mHello,0},
		{"A-1-4","",mHello,0},
		{"A-1-5","",mHello,0},
		{NULL,NULL,NULL,NULL}
};

MenuItem menu_a2[] = {
		{"A-2-1","",mHello,0},
		{"A-2-2","",mHello,0},
		{"A-2-3","",mHello,0},
		{"A-2-4","",mHello,0},
		{"A-2-5","",mHello,0},
		{NULL,NULL,NULL,NULL}
};

MenuItem menu_a[] = {
		{"A-1","",mHello,menu_a1},
		{"A-2","",mHello,menu_a2},
		{"A-3","",mHello,0},
		{"A-4","",mHello,0},
		{"A-5","",mHello,0},
		{NULL,NULL,NULL,NULL}
};

MenuItem menu_b[] = {
		{"B-1","",0,0},
		{"B-2","",0,0},
		{"B-3","",0,0},
		{"B-4","",0,0},
		{"B-5","",0,0},
		{NULL,NULL,NULL,NULL}
} ;

MenuItem menu_root[] = {
		{"A","",0,menu_a},
		{"B","",0,menu_b},
		{"C","",mHello,0},
		{"D","",mHello,0},
		{NULL,NULL,NULL,NULL}
};

Menu menu(menu_root,&lcd);


//PROGMEM MENU_ITEM menus[] = {
//		{"a1","",0,{
//				{"a1.1","",0,0},
//				{"a1.2","",0,0},
//			}
//		},
//		{"b1","",0,0
////				{"b1.1","",0,0},
////				{"b1.2","",0,0},
//		},
//};


//-----------------------------------------------
//data array maxium length
#define MAX_LEN 16

//4 bytes Serial number of card, the 5 bytes is verify bytes
uchar cardId[5];
uchar moneyConsume = 18;  //Deduct 18 USD
uchar moneyAdd = 10;  //Charge up 10 USD

int state = 0;

void beep_init(void){
	pinMode(PinBEEP,OUTPUT);
}

void beep(unsigned char delayms){
  analogWrite(PinBEEP, 250);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(PinBEEP, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms
}

/////////////////////////////////////////////////////////////////////
//set the pin
/////////////////////////////////////////////////////////////////////
//const int chipSelectPin = 53;  // SDA
//const int NRSTPD = A5;          // RST

void setup() {
	beep_init();
	beep(200);

	Serial.begin(9600); // RFID reader SOUT pin connected to Serial RX pin at 2400bps

	// Initialize the LCD
	lcd.init();
	lcd.setBacklight(ON);
	lcd.clear();

	// start the SPI library:
	SPI.begin();
	rfid.Init();
	Serial.println(F("MFRC522 initialized"));
#if 0
	lcd.drawString(0, 0, "Initialized");
#endif
	state = 1;

	keypad.init(onKeyPressed,0,0,0);
	lcd.setBacklight(OFF);
	lcd.clear();
	menu.updateLCD();
}

void loop(){
//	uchar i, tmp;
	uchar status;
	uchar buf[MAX_LEN];
//	uchar RC_size;
//	uchar blockAddr;	//Select operation buck address  0 - 63
//	String mynum = "";

	keypad.runloop();

	//Search card, return card types
	status = rfid.Request(PICC_REQIDL, buf);

	if (status == MI_OK){
		Serial.println("Card detected");
		Serial.print(buf[0], BIN);
		Serial.print(" , ");
		Serial.print(buf[1], BIN);
		Serial.println(" ");
	}

	//Prevent conflict, return the 4 bytes Serial number of the card
	status = rfid.Anticoll(buf);

	memcpy(cardId, buf, 5);
	if (status == MI_OK){
		lcd.setBacklight(ON);

		// Should really check all pairs, but for now we'll just use the first
		if (cardId[0] == 130) {
			Serial.println("Hello Shawn");
			lcd.drawString(0,2,"Hello Shawn!");
		}else if (cardId[0] == 141) {
			Serial.println("Hello Qian");
			lcd.drawString(0,2,"Hello Qian!");
		}
		char buf[15];
		snprintf(buf,15,"ID:%02X%02X%02X%02X",cardId[0], cardId[1] , cardId[2] , cardId[3]);
		Serial.println(buf);
		lcd.drawString(0,4,buf);
		beep(200);
		state = 1;

		delay(2000);
	}

	if(state !=0){
//		lcd.clear();
//		lcd.setBacklight(OFF);
//		lcd.drawString(0,0,"Waiting...");
		Serial.println("Waiting...");
		state = 0;
	}
	rfid.Halt();			//command the card into sleep mode
}


bool onKeyPressed(Keycode keycode){
	char buf[64];
	snprintf(buf,64,"Key pressed: %d", keycode);
	Serial.println(buf);
	lcd.drawString(0,5,buf);

	switch(keycode){
	case 5:
		menu.prev();
		break;
	case 3:
		menu.next();
		break;
	case 4:
		menu.enter();
		break;
	case 1:
		menu.leave();
		break;
	case 2:
		lcd.setBacklight(!lcd.backlight());
		break;
	default:
		break;
	}

	return true; // done, do not repeat.
}
