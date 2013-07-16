/*
 * GPS.h
 *
 *  Created on: 2013-7-16
 *      Author: shawn
 */

#ifndef GPS_H_
#define GPS_H_
#include <stdint.h>

class HardwareSerial;

class GPS {
public:
	GPS(uint8_t txPin, uint8_t rxPin);
	void init(void);
	void loop(void);
	void reset(void);

private:
	uint8_t _rxPin;
	uint8_t _txPin;
	int _byteGPS;
	const char* _cmd;
	int _counter1; // counts how many bytes were received (max 300)
	int _counter2; // counts how many commas were seen
	int _offsets[13];
	char _buf[300];

	int getSize(uint8_t offset);
	int handleByte(int byteGPS);
	HardwareSerial *_pSerial;
};

#endif /* GPS_H_ */
