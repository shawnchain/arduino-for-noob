/*
 * Menu.h
 *
 *  Created on: 2013-4-1
 *      Author: shawn
 */

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

class LCD5110;

// Menus
typedef bool (*MENU_ITEM_CALLBACK)(void*);

struct MenuItem {
	const char* title;
	const char* description;
	const MENU_ITEM_CALLBACK callback;
	const MenuItem *children;
};

#define MAX_MENU_DEPTH 8
#define MAX_MENU_INDEX 16
class Menu {
public:
	Menu(const MenuItem* menuItem,LCD5110 *lcd);
	void prev();
	void next();
	void enter();
	void leave();
	void updateLCD(void);

private:
	uint8_t _menuLevel;
	uint8_t _menuIndex[MAX_MENU_DEPTH];
	const MenuItem* _menuStack[MAX_MENU_DEPTH]; // allow max
	LCD5110 *_lcd;
	MenuItem* currentItem(void);
	void updateLCD(const MenuItem* item);
};

#endif /* MENU_H_ */
