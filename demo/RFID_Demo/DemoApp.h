// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef RFID_Demo_H_
#define RFID_Demo_H_

// Choose different Arduino boards
//#define MEGA_2560 1
//#define PROMINI_328 0
//#define BOARD arduino_mega_2560
//#define BOARD arduino_promini_328


#include <Arduino.h>
#include <LCD/LCD5110.h>
#include <Keypad/AnalogKeypad.h>
#include "Menu.h"

// Eliminate the GCC warning
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

// DRIVERS
extern Menu menu;
extern LCD5110 lcd;

// COMPONENTS
//class Module{
//	LCD5110 &_lcd;
//	Menu &_menu;
//	AnalogKeypad &_keypad;
//	Module(LCD5110 &lcd,Menu &menu,AnalogKeypad &kp):_lcd(lcd),_menu(menu),_keypad(kp){};
//};

#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//Do not add code below this line
#endif /* RFID_Demo_H_ */
