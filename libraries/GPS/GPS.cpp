/*
 * GPS.cpp
 *
 *  Created on: 2013-7-16
 *      Author: shawn
 */

#include "GPS/GPS.h"
#include <Arduino.h>
#include "Stream.h"
#include "SoftwareSerial.h"
#include "HardwareSerial.h"
#include "../LCD/LCD5110.h"

GPS::GPS(SoftwareSerial *gpsPort, LCD5110 *lcd, GPSCallback callback){
	_counter1 = 0; // counts how many bytes were received (max 300)
	_counter2 = 0; // counts how many commas were seen
	_callback = callback;

	_gpsPort = gpsPort;
	_lcd = lcd;
}

void GPS::reset(){
	_counter1 = 0; // counts how many bytes were received (max 300)
	_counter2 = 0; // counts how many commas were seen
}

void GPS::init() {
	_offsets[0] = 0;
	_buf[0] = '\0';
	reset();
	_gpsPort->begin(4800);
}

int GPS::getSize(uint8_t offset){
	return _offsets[offset+1] - _offsets[offset] - 1;
}

int GPS::handleByte(int byteGPS){
	// GPS returns with '\r\n' ending, we just trigger by '\r' here
	// so always Skip the '\n';
	if(byteGPS == '\n'){
		return 1;
	}

	_buf[_counter1] = byteGPS;
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
	}else if(byteGPS == '*') {
		// directly store the checksum
		_offsets[12] = _counter1;
	}

	if(byteGPS != '\r')
		return 1; // continue reading.

	// else we got a <LF>, which indicates the end of a line
	// $GPRMC,001101.053,V,,,,,,,291006,,,N*46
	// Check that we got 12 pieces, and that the first piece is 6 characters
	if (_counter2 != 12 || (getSize(0) != 6)) {
		return 0;
	}

	// Check that we received $GPRMC
	if(strncmp(_buf,"$GPRMC",6) != 0){
		return 0;
	}

	// Check that time is well formed
	if (getSize(1) != 10) {
		return 0;
	}

	// Check that date is well formed
	if (getSize(9) != 6) {
		return 0;
	}

	// dump to serial console
	_buf[_counter1] = 0;
	Serial.println(_buf);


	// TODO: compute and validate checksum
	// TODO: handle timezone offset
	// TODO: extract the GPS info

	// print time
	char buf[15];
	const char* time = _buf + _offsets[1];
	snprintf(buf,14,"%c%c:%c%c:%c%c UTC",time[0],time[1],time[2],time[3],time[4],time[5]);
	_lcd->drawString(0,1,buf);

	// print date
	const char* date = _buf + _offsets[9];
	snprintf(buf,10,"%c%c/%c%c/%c%c",date[0],date[1],date[2],date[3],date[4],date[5]);
	_lcd->drawString(0,0,buf);

	return 0;
}

void GPS::loop() {
	if(_gpsPort->available() > 0){
		int abyte = _gpsPort->read();      // Read a byte of the serial port
		if(abyte == -1){
			delay(1);
		}else{
			if (handleByte(abyte) == 0) {
				reset();
			}
		}
	}
}
