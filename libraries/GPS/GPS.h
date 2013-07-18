/*
 * GPS.h
 *
 *  Created on: 2013-7-16
 *      Author: shawn
 */

#ifndef GPS_H_
#define GPS_H_
#include <stdint.h>
#include "SoftwareSerial.h"
#include "HardwareSerial.h"


class LCD5110;
class NMEA;

class GPSInfo{
	char* lat;
	char* lon;
	char* alt;
};

typedef void (*GPSCallback)(GPSInfo&);

class GPS {
public:
	GPS(SoftwareSerial *serial, LCD5110 *lcd, GPSCallback callback);
	GPS(HardwareSerial *serial, LCD5110 *lcd, GPSCallback callback);

	void setup(int baud = 4800);
	void loop(void);
	void reset(void);
	void sendCommand(char* cmd);

private:
	GPSCallback _callback;
	SoftwareSerial *gpsSwSerial;
	HardwareSerial *gpsHwSerial;

	LCD5110 *_lcd;
	GPSInfo _gpsInfo;

	NMEA *_pNMEA;

	int handleByte(int byteGPS);
};
#endif /* GPS_H_ */
