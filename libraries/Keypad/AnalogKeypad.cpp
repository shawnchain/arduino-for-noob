/*
 * AnalogKeypad.cpp
 *
 *  Created on: 2013-3-29
 *      Author: shawn
 */

#include "AnalogKeypad.h"
#include <Arduino.h>
#include <inttypes.h>
#include <avr/io.h>

// Timer2 interrupt routine for keycode scan
// 1/(160000000/256/(256-6)) = 4ms interval
AnalogKeypad *_keypadInstance = 0; // we assume only one keypad instance available
ISR(TIMER2_OVF_vect) {
	TCNT2 = 6;
	if(_keypadInstance){
		_keypadInstance->scan();
	}
}

AnalogKeypad::AnalogKeypad(uint8_t inputPin) {
	_analogInputPin = inputPin;
	_keyEventCallback = 0;
	_lastKeyCode = 0;
	_debounceCount = 0;
	_keyCount = 0;
	_pressedKeycode = 0;
}

void AnalogKeypad::init(KeypadCallback callback){
	this->init(callback, 0,0,0);
}

void AnalogKeypad::init(KeypadCallback callback, uint8_t analogPin = 0, uint16_t *keyADCValues = 0, uint8_t keyCount = 0){
	_keyEventCallback = callback;
	_analogInputPin = analogPin;

	// store key adc values
	_keyADCValues[0] = 0;
	if(keyADCValues && keyCount){
		_keyCount = keyCount;
		for(int i = 0;i<keyCount || i < MAX_SUPPORTED_KEY_COUNT;i++){
			_keyADCValues[i + 1] = *(keyADCValues + i);
		}
	}else{
		// default key ADC values
		_keyCount =  5;
		_keyADCValues[1] = 50;
		_keyADCValues[2] = 200;
		_keyADCValues[3] = 400;
		_keyADCValues[4] = 600;
		_keyADCValues[5] = 800;
	}

	_keypadInstance = this;

	// Setup timer2 -- Prescaler/256
	TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
	TCCR2B &= ~(1<<WGM22);
	TCCR2B = (1<<CS22)|(1<<CS21);
	ASSR |=(0<<AS2);
	// Use normal mode
	TCCR2A =0;
	//Timer2 Overflow Interrupt Enable
	TIMSK2 |= (0<<OCIE2A);
	TCNT2=0x6;  // counting starts from 6;
	TIMSK2 = (1<<TOIE2);
	SREG|=1<<SREG_I;
}

void AnalogKeypad::runloop(void){
	if(_pressedKeycode == 0){
		return;
	}
	int code = _pressedKeycode;
	_pressedKeycode = 0;
	// call the callbacks
	bool repeat = false;
	if(_keyEventCallback){
		repeat = !_keyEventCallback(code);
	}
	// if callback returns false, then it will be called repeatly
	if(repeat){
		_debounceCount--;
	}
}

/**
 * Just detect the key pressed event
 */
void AnalogKeypad::scan(void){
	Keycode code = readKeyCode();
	// released
	if(code == 0){
		if(_lastKeyCode == 0){
			return;
		}
		if(_debounceCount > 0){
			_debounceCount --;
		}else{
			_lastKeyCode = 0;
		}
		return ;
	}

	// pressed, we assume bounce only occurs between 0-to-keycode, but not keycode-to-keycode
	if(_lastKeyCode != code){
		// key changed, enable restart debounce counting.
		_lastKeyCode = code;
		_debounceCount = 0;
		return;
	}else{
		if(_debounceCount < DEBOUNCE_COUNT_EDGE){
			_debounceCount ++;
		}else if(_debounceCount == DEBOUNCE_COUNT_EDGE){
			_pressedKeycode = code;
			_debounceCount++;
		}
	}
}

inline Keycode AnalogKeypad::readKeyCode(){
	int inputLevel = analogRead(_analogInputPin);
	for (Keycode k = 1; k < 6; k++) {
		if (inputLevel < _keyADCValues[k]) {
			return k;
		}
	}
	return 0;
}
