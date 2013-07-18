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
#include "NMEA/nmea.h"

NMEA sNMEA(GPRMC);

GPS::GPS(SoftwareSerial *serial, LCD5110 *lcd, GPSCallback callback) {
	gpsSwSerial = serial;
	gpsHwSerial = 0;
	_lcd = lcd;
	_callback = callback;
	_pNMEA = &sNMEA;
}

GPS::GPS(HardwareSerial *serial, LCD5110 *lcd, GPSCallback callback) {
	gpsSwSerial = 0;
	gpsHwSerial = serial;
	_lcd = lcd;
	_callback = callback;
	_pNMEA = &sNMEA;
}

void GPS::setup(int baud) {
	if (gpsSwSerial)
		gpsSwSerial->begin(baud);
	else
		gpsHwSerial->begin(baud);

	delay(10);
}

int GPS::handleByte(int byteGPS) {
	_pNMEA->decode(byteGPS);
	return 0;
}

void GPS::loop() {
	int abyte = -1;
	if(gpsSwSerial){
		abyte = gpsSwSerial->read();
	}else{
		abyte = gpsHwSerial->read();
	}

	if (abyte < 0) {
//		_lcd->drawString(0, 3, "Not connected!");
		delay(1);
		return;
	}

	if (_pNMEA->decode(abyte)) {
		char buf[32], buf1[10],buf2[10],buf3[10];

		// display the time/date
		snprintf(buf1,7,"%s",_pNMEA->term(1));
		snprintf(buf2,7,"%s",_pNMEA->term(9));
		snprintf(buf,30,"%s/%s",buf1,buf2);
		_lcd->drawString(0,0,buf);

		if (_pNMEA->gprmc_status() == 'A') {
			// Display lat/lon
			dtostrf(_pNMEA->gprmc_latitude(),3,5,buf1);
			dtostrf(_pNMEA->gprmc_longitude(),3,5,buf2);
			dtostrf(_pNMEA->gprmc_speed(KMPH),3,2,buf3);
			_lcd->drawString(0, 1, buf1);
			_lcd->drawString(0, 2, buf2);
			_lcd->drawString(0, 3, buf3);
			_lcd->drawString(0, 5, "Recving...    ");
		} else {
			// void
			// Not receiving any thing
			static int flag = 0;
			if (flag == 0) {
				_lcd->drawString(0, 5, "Searching   ");
				flag = 1;
			} else if (flag == 1) {
				_lcd->drawString(0, 5, "Searching.  ");
				flag = 2;
			} else if (flag == 2) {
				_lcd->drawString(0, 5, "Searching.. ");
				flag = 3;
			} else if (flag == 3) {
				_lcd->drawString(0, 5, "Searching...");
				flag = 0;
			}
		}
	}
}

void GPS::sendCommand(char* cmd) {
	if (gpsSwSerial)
		gpsSwSerial->println(cmd);
	else
		gpsHwSerial->println(cmd);
}
