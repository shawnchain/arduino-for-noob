/*
 * Menu.cpp
 *
 *  Created on: 2013-4-1
 *      Author: shawn
 */

#include "Menu.h"
#include <stdio.h>
#include <LCD/LCD5110.h>
#include "Arduino.h"

#define DEBUG 1

Menu::Menu(const MenuItem *menuItem, LCD5110 *lcd){
	// TODO - read from PROGMEM
	_menuLevel = 0;
	_menuStack[0] = menuItem;
	_menuIndex[0] = 0;
	_lcd = lcd;
}

void Menu::prev(){
	if(_menuIndex[_menuLevel] > 0){
		_menuIndex[_menuLevel] = _menuIndex[_menuLevel] - 1;
		updateLCD(0);
	}else{
#if DEBUG
		char buf[128];
		snprintf(buf,128,"reaches the first menu item, level:%d, index:%d",_menuLevel,_menuIndex[_menuLevel]);
		Serial.println(buf);
#endif
	}
}

void Menu::next(){
	if((_menuStack[_menuLevel] + _menuIndex[_menuLevel] + 1)->title != 0){
		_menuIndex[_menuLevel] = _menuIndex[_menuLevel] + 1;
		updateLCD(0);
	}else{
#if DEBUG
		char buf[128];
		snprintf(buf,128,"reaches the last menu item, level:%d, index:%d",_menuLevel,_menuIndex[_menuLevel]);
		Serial.println(buf);
#endif
	}
}

void Menu::enter(){
	MenuItem *current = currentItem();
	if(current->children != 0){
		// has children, can enter
		_menuLevel++;
		_menuStack[_menuLevel] = current->children;
		updateLCD(0);
#if DEBUG
		char buf[128];
		snprintf(buf,128,"Enter Menu:%s",current->title);
		Serial.println(buf);
#endif
	}else{
#if DEBUG
		char buf[128];
		snprintf(buf,128,"Choosed Menu %s, level:%d, index:%d",current->title, _menuLevel,_menuIndex[_menuLevel]);
		Serial.println(buf);
#endif
		if(current->callback){
			// call the menu callbacks.
			current->callback((void*)current);
		}
	}
}

void Menu::leave(){
	if(_menuLevel > 0){
#if DEBUG
		char buf[128];
		snprintf(buf,128,"Leave Menu:%s", currentItem()->title);
		Serial.println(buf);
#endif
		_menuLevel--;
	}
	updateLCD(0);
}

MenuItem*  Menu::currentItem(void){
	return (MenuItem*)(_menuStack[_menuLevel] + _menuIndex[_menuLevel]);
}


void Menu::updateLCD(void){
	updateLCD(NULL);
}

/**
 * Update item on LCD
 */
void Menu::updateLCD(const MenuItem* item){
	#define LCD_LINE_MAX 6
	//Update all menus on LCD
	_lcd->clear();
	for(int i = 0;i<6;i++){
		const MenuItem* c = (_menuStack[_menuLevel] + i);
		if( c->title == 0){
			// reaches the last one
			break;
		}
		//FIXME - support progmem
//		char buf[64];
//		snprintf(buf,32,"  draw: %s",c->title);
//		Serial.println(buf);
		_lcd->drawString(0,i,c->title, _menuIndex[_menuLevel] == i);
	}
}
