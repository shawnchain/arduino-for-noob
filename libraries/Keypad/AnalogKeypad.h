/*
 * AnalogKeypad.h
 *
 *  Created on: 2013-3-29
 *      Author: shawn
 */

#ifndef ANALOGKEYPAD_H_
#define ANALOGKEYPAD_H_

#include <stdint.h>

typedef uint8_t Keycode;
typedef bool (*KeypadCallback)(Keycode);

#define MAX_SUPPORTED_KEY_COUNT 12 // 12 keys max
#define DEBOUNCE_COUNT_EDGE 12

class AnalogKeypad {
public:
	AnalogKeypad(uint8_t inputPin);
	void init(KeypadCallback callback, uint8_t analogPin, uint16_t *keyADCValues, uint8_t keyCount);
	void init(KeypadCallback callback);
	void runloop(void);

private:
	uint8_t _analogInputPin;
	KeypadCallback _keyEventCallback;
	Keycode _lastKeyCode;
	Keycode _pressedKeycode;
	Keycode readKeyCode();
	uint8_t _debounceCount;
	uint8_t _keyCount;
	int16_t _keyADCValues[MAX_SUPPORTED_KEY_COUNT + 1];

public:
	void scan(void);
};

#endif /* ANALOGKEYPAD_H_ */
