/*
 * TGAM.h
 *
 *  Created on: 2013-7-23
 *      Author: shawn
 */

#ifndef TGAM_H_
#define TGAM_H_
#include "SoftwareSerial.h"
#include "HardwareSerial.h"


class LCD5110;

struct TGAMData{
	char signal;
	char attention;
	char meditation;
	char blink;

	uint16_t raw;

	// more details
	uint16_t delta;
	uint16_t theta;
	uint16_t alphaLow;
	uint16_t alphaHigh;
	uint16_t betaLow;
	uint16_t betaHigh;
	uint16_t gammaHigh;
	uint16_t gammaLow;
};

typedef void (*TGAMCallback)(const TGAMData*);

class TGAM {
public:
	TGAM(SoftwareSerial *serial, LCD5110 *lcd, TGAMCallback callback);
	TGAM(HardwareSerial *serial, LCD5110 *lcd, TGAMCallback callback);

	void setup(int baud = 57600);
	void loop(void);
	void reset(void);

private:
	TGAMCallback _callback;
	SoftwareSerial *_swSerial;
	HardwareSerial *_hwSerial;

	LCD5110 *_lcd;
	TGAMData _data;

	int parsePayload();
	int handleByte(char byte);
	int _state;
	char _payload[256];
	int _payloadLength;
	int _payloadSum;
	unsigned char _payloadReceived;

	bool available();
	char readByte();
	size_t readBytes(char* buf, size_t size);
};

#endif /* TGAM_H_ */
