// Do not remove the include below
#include "DemoApp.h"

#include <Keypad/AnalogKeypad.h>
#include "RFIDModule.h"

//---------------------------
// Modules
//---------------------------
RFIDModule rfidModule;

//---------------------------
// App Menu/LCD/Keypad setup
//---------------------------
// LCD setup (pins)
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

// Keypad setup
#define KEYPAD_INPUT A0
AnalogKeypad keypad(KEYPAD_INPUT);
bool keypad_callback(Keycode keycode);

// Menus
bool mHello(void* item);
bool mSettings_backlight(void* item);
bool mAbout(void* item);
bool mBack(void* item);

//MenuItem menu_a1[] = {
//		{"A-1-1","",mHello,0},
//		{"A-1-2","",mHello,0},
//		{"A-1-3","",mHello,0},
//		{"A-1-4","",mHello,0},
//		{"A-1-5","",mHello,0},
//		{NULL,NULL,NULL,NULL}
//};
//MenuItem menu_a2[] = {
//		{"A-2-1","",mHello,0},
//		{"A-2-2","",mHello,0},
//		{"A-2-3","",mHello,0},
//		{"A-2-4","",mHello,0},
//		{"A-2-5","",mHello,0},
//		{NULL,NULL,NULL,NULL}
//};
//
//MenuItem menu_a[] = {
//		{"A-1","",mHello,menu_a1},
//		{"A-2","",mHello,menu_a2},
//		{"A-3","",mHello,0},
//		{"A-4","",mHello,0},
//		{"A-5","",mHello,0},
//		{NULL,NULL,NULL,NULL}
//};
MenuItem menu_settings[] = {
		{"..","",mBack,0},
		{"Back light","",mSettings_backlight,0},
		{"Overclock","",0,0},
		{"Boot to windows","",0,0},
		{"...->","",0,0},
		{NULL,NULL,NULL,NULL}
} ;
MenuItem menu_root[] = {
		{"RPi Monitor","",mHello,0},
		{"RFID Reader","",mHello,0},
		{"Settings","",0,menu_settings},
		{"About","",mAbout,0},
		{NULL,NULL,NULL,NULL}
};
Menu menu(menu_root,&lcd);

//-------------------
// Arduino Entrance
//-------------------
void setup() {
	Serial.begin(9600); // RFID reader SOUT pin connected to Serial RX pin at 2400bps

	// LCD init
	lcd.init();
	lcd.setBacklight(ON);
	lcd.clear();
	lcd.setBacklight(OFF);

	// Keypad init
	keypad.init(keypad_callback,0,0,0);
	menu.updateLCD();

	// RFID module setup
	rfidModule.setup();
}

void loop(){
	keypad.runloop();
	rfidModule.loop();
}

//-----------------------------------------
// Keypad & menu callback implementations
//-----------------------------------------
bool keypad_callback(Keycode keycode){
	char buf[64];
	snprintf(buf,64,"Key pressed: %d", keycode);
	Serial.println(buf);
	lcd.drawString(0,5,buf);

	switch(keycode){
	case 5: // up
		menu.prev();
		return false; // allows key repeat
		break;
	case 3: // down
		menu.next();
		return false; // allow key repeat
		break;
	case 4: // center
		menu.enter();
		break;
	case 1: // left
		menu.leave();
		break;
	case 2: // right
		menu.enter();
		break;
	default:
		break;
	}
	return true; // done, do not repeat.
}

bool mHello(void* item){
	lcd.clear();
	lcd.drawString(25,0,"DEMO",true);
	char buf[128];
	snprintf(buf,128,"%s",((MenuItem*)item)->title);
	Serial.println(buf);
	lcd.drawString(0,2,buf);
	return true;
}

bool mSettings_backlight(void* item){
	lcd.setBacklight(!lcd.backlight());
	return true;
}

bool mAbout(void* item){
	lcd.clear();
	lcd.drawString(25,0,"About",true);
	lcd.drawString(0,2,"MagicBox 1.0");
	lcd.drawString(0,5,"S.C. 2013");
//	char buf[128];
//	snprintf(buf,128,"Greetings, this is %s",((MenuItem*)item)->title);
//	Serial.println(buf);
//	lcd.drawString(2,0,buf);
	return true;
}

bool mBack(void* item){
	menu.leave();
	return true;
}
