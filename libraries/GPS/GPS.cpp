/*
 * GPS.cpp
 *
 *  Created on: 2013-7-16
 *      Author: shawn
 */

#include "GPS/GPS.h"
#include <Arduino.h>

GPS::GPS(uint8_t txPin, uint8_t rxPin){
	_rxPin = rxPin;
	_txPin = txPin;
	_byteGPS=-1;
	_cmd = "$GPRMC";
	_counter1 = 0; // counts how many bytes were received (max 300)
	_counter2 = 0; // counts how many commas were seen

	//FIXME - Use different serial according to boards
#ifndef BOARD
#warning Board type not defined, using -DBOARD=xxx to specify one. Supported board types: arduion_mega_2560, arduino_promini_328
#define BOARD arduino_mega_2560
#endif

#ifdef BOARD
	#if BOARD == arduino_mega_2560
		if(rxPin == 19){
			_pSerial = &Serial1;
		}else if(rxPin == 17){
			_pSerial = &Serial2;
		}else if(rxPin == 15){
			_pSerial = &Serial3;
		}else{
			_pSerial = &Serial;
		}
	#else
		#warning Unknow board type, useing Serial as GPS tx/rx port
		_pSerial = &Serial;
	#endif
#endif


}

void GPS::reset(){
	_counter1 = 0; // counts how many bytes were received (max 300)
	_counter2 = 0; // counts how many commas were seen
}

void GPS::init() {
	pinMode(_rxPin, INPUT);
	pinMode(_txPin, OUTPUT);
	_pSerial->begin(4800);
	_offsets[0] = 0;
	_buf[0] = '\0';
	reset();
}

int GPS::getSize(uint8_t offset){
	return _offsets[offset+1] - _offsets[offset] - 1;
}

int GPS::handleByte(int byteGPS){
	_buf[_counter1] = byteGPS;
	Serial.print((char) byteGPS);
	_counter1++;
	if (_counter1 == 300) {
		return 0;
	}
	if (byteGPS == ',') {
		_counter2++;
		_offsets[_counter2] = _counter1;
		if (_counter2 == 13) {
			return 0;
		}
	}
	if (byteGPS == '*') {
		_offsets[12] = _counter1;
	}

	// Check if we got a <LF>, which indicates the end of line
	if (byteGPS == 10) {
		// Check that we got 12 pieces, and that the first piece is 6 characters
		if (_counter2 != 12 || (getSize(0) != 6)) {
			return 0;
		}

		// Check that we received $GPRMC
		for (int j = 0; j < 6; j++) {
			if (_buf[j] != _cmd[j]) {
				return 0;
			}
		}

		// Check that time is well formed
		if (getSize(1) != 10) {
			return 0;
		}

		// Check that date is well formed
		if (getSize(9) != 6) {
			return 0;
		}

		// TODO: compute and validate checksum

		// TODO: handle timezone offset

		// print time
		//lcd.clear();
		for (int j = 0; j < 6; j++) {
//			lcd.print(buf[offsets[1] + j]);
			Serial.print(_buf[_offsets[1] + j]);
			if (j == 1) {
				Serial.print("h");
			} else if (j == 3) {
				Serial.print("m");
			} else if (j == 5) {
				Serial.print("s UTC");
			}
		}

		// print date
//		lcd.setCursor(0, 1);
		for (int j = 0; j < 6; j++) {
			Serial.print(_buf[_offsets[9] + j]);
			if (j == 1 || j == 3) {
				Serial.print(".");
			}
		}
		return 0;
	}
	return 1;
}

void GPS::loop() {
	_byteGPS = _pSerial->read();      // Read a byte of the serial port
	if (_byteGPS == -1) {           // See if the port is empty yet
		delay(100);
	} else {
		if (!handleByte(_byteGPS)) {
			reset();
		}
	}
}
