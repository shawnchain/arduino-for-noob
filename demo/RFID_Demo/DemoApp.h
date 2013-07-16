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
#include "Menu.h"

// Eliminate the GCC warning
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

// Shared components
extern Menu menu;
extern LCD5110 lcd;

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
