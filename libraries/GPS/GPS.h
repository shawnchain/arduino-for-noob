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

class GPSInfo{
	char* lat;
	char* lon;
	char* alt;
};

typedef void (*GPSCallback)(GPSInfo&);

class GPS {
public:
	GPS(SoftwareSerial *gpsPort, LCD5110 *lcd, GPSCallback callback);
	void init(void);
	void loop(void);
	void reset(void);

private:

	int _counter1; // counts how many bytes were received (max 300)
	int _counter2; // counts how many commas were seen
	int _offsets[13];
	char _buf[300];
	GPSCallback _callback;

	int getSize(uint8_t offset);
	int handleByte(int byteGPS);

	SoftwareSerial *_gpsPort;

	LCD5110 *_lcd;

	GPSInfo _gpsInfo;
};

#endif /* GPS_H_ */
