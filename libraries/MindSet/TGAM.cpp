/*
 * TGAM.cpp
 *
 *  Created on: 2013-7-23
 *      Author: shawn
 */

#include "TGAM.h"
#include <Arduino.h>
#include "SoftwareSerial.h"
#include "HardwareSerial.h"
#include "../LCD/LCD5110.h"

TGAMData tgamData;

TGAM::TGAM(SoftwareSerial *serial, LCD5110 *lcd, TGAMCallback callback) {
	_swSerial = serial;
	_hwSerial = 0;
	_lcd = lcd;
	_callback = callback;
	reset();
}

TGAM::TGAM(HardwareSerial *serial, LCD5110 *lcd, TGAMCallback callback) {
	_swSerial = 0;
	_hwSerial = serial;
	_lcd = lcd;
	_callback = callback;
	reset();
}

void TGAM::setup(int baud) {
	if (_swSerial)
		_swSerial->begin(baud);
	else
		_hwSerial->begin(baud);
	delay(10);
}

#define SYNC_BYTE 0xAA
#define EXTC_BYTE 0x55

/**
 * Parse payload
 *
 * returns 1|true for success, 0|false for failure
 */
int TGAM::parsePayload() {
	if (_state == 0) {
		return 0;
	}

	int i = 0, result = 0;
	while (i < _payloadLength) {
		int code = _payload[i++];
		switch (code) {
		case 0x02:
			tgamData.signal = _payload[i++];
			i = i + 2;
			result = 1;
			break;
		case 0x04:
			tgamData.attention = _payload[i++];
			i = i + 2;
			result = 1;
			break;
		case 0x05:
			tgamData.meditation = _payload[i++];
			result = 1;
			break;
		case 0x16:
			tgamData.blink = _payload[i++];
			result = 1;
			break;
		default:
			// unsupported
			i++;
		}
	}

	// dump out
	char buf[100];
	if (result) {
		snprintf(buf, 100, "sig:%d, att:%d, med:%d, blk:%d", tgamData.signal,
				tgamData.attention, tgamData.meditation, tgamData.blink);
	} else {
		snprintf(buf, 100, "Unsupported packet, %d bytes", _payloadLength);
	}
	Serial.println(buf);

	reset();
	return result;
}

#define STATE_SYNC0 0
#define STATE_SYNC1 1
#define STATE_PAYLOAD_LEN   2
#define STATE_PAYLOAD 3
#define STATE_CHECKSUM 4

int TGAM::handleByte(char byte) {
	int returnValue = 0;
	switch (_state) {
	/* Waiting for SyncByte */
	case (STATE_SYNC0):
		if (byte == SYNC_BYTE) {
			_state = STATE_SYNC1;
		}
		break;

		/* Waiting for second SyncByte */
	case (STATE_SYNC1):
		if (byte == SYNC_BYTE) {
			_state = STATE_PAYLOAD_LEN;
		} else {
			_state = STATE_SYNC0;
		}
		break;

		/* Waiting for Data[] length */
	case (STATE_PAYLOAD_LEN):
		_payloadLength = byte;
		if (_payloadLength > 170) {

			char buf[100];
			snprintf(buf, 100, "ERROR: Invalid length: %d", _payloadLength);
			Serial.print(buf);

			_state = STATE_SYNC0;
		} else if (_payloadLength == 170) {
			// do nothing, but should continue?
			returnValue = 1;
		} else {
			_payloadReceived = 0;
			_payloadSum = 0;
			_state = STATE_PAYLOAD;
		}
		break;

		/* Waiting for Payload[] bytes */
	case (STATE_PAYLOAD):
		_payload[_payloadReceived++] = byte;
		_payloadSum = (unsigned char) (_payloadSum + byte);
		if (_payloadReceived >= _payloadLength) {
			_state = STATE_CHECKSUM;
		}
		break;

		/* Waiting for CKSUM byte */
	case (STATE_CHECKSUM): {
		_state = STATE_SYNC0;
		unsigned char expectedSum = byte;
		unsigned char actuallSum = ((~_payloadSum) & 0xFF);
		if (expectedSum != actuallSum) {
			// checksum error!
			char buf[100];
			snprintf(buf, 100, "ERROR: Checksum invalid, expect %d but got %d",
					expectedSum, actuallSum);
			Serial.println(buf);
//			returnValue = -1;
		} else {
			// read success!
			returnValue = 1;
		}
		break;
	}

		/* unrecognized state */
	default:
		_state = STATE_SYNC0;
		break;
	}
	return returnValue;
}

/**
 * Read a block of record
 */
void TGAM::loop() {
	while (available()) {
		char abyte = readByte();
		if (abyte >= 0) {
			if (handleByte(abyte) == 1) {
				parsePayload();
			}
		}
	}
}

void TGAM::reset() {
	_state = 0;
	_payloadLength = 0;
	_payloadReceived = 0;
	_payloadSum = 0;
}

bool TGAM::available() {
	if (_swSerial) {
		return _swSerial->available();
	} else {
		return _hwSerial->available();
	}
}

char TGAM::readByte() {
	if (_swSerial) {
		return (char) (_swSerial->read());
	} else {
		return (char) (_hwSerial->read());
	}
}
size_t TGAM::readBytes(char* buf, size_t size) {
	if (_swSerial) {
		return (_swSerial->readBytes(buf, size));
	} else {
		return (_hwSerial->readBytes(buf, size));
	}
}
