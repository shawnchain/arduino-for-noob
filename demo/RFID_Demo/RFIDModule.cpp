/*
 * RFIDModule.cpp
 *
 *  Created on: 2013-4-6
 *      Author: shawn
 */
#include "RFIDModule.h"
#include "DemoApp.h"

#include <Arduino.h>

// the sensor communicates using SPI, so include the library:
#include <SPI.h>
#include <RFID/MFRC522.h>

// RFID Pin setup
#ifndef BOARD
	#warning Board type not defined, using -DBOARD=xxx to specify one. Supported board types: arduion_mega_2560, arduino_promini_328
	#define BOARD arduion_mega_2560
#endif

#ifdef BOARD
	#if BOARD == arduino_mega_2560
		#define RFID_SS 53  // SPI device select
		#define RFID_RST A5 // device reset
		// The beep pin
		#define PinBEEP A9
	#elif BOARD == arduino_promini_328
		#define RFID_SS 10  // SPI device select
		#define RFID_RST 9 // device reset
		// The beep pin
		#define PinBEEP A2
	#endif
#endif

/////////////////////////////////////////////////////////////////////
//set the pin
/////////////////////////////////////////////////////////////////////
//const int chipSelectPin = 53;  // SDA
//const int NRSTPD = A5;          // RST
MFRC522 rfid(RFID_SS,RFID_RST);

//-----------------------------------------------
//data array maxium length
#define MAX_LEN 16

//4 bytes Serial number of card, the 5 bytes is verify bytes
uchar cardId[5];
uchar moneyConsume = 18;  //Deduct 18 USD
uchar moneyAdd = 10;  //Charge up 10 USD

int state = 0;

// Beep
static void beep_setup(void){
	pinMode(PinBEEP,OUTPUT);
}
static void beep(unsigned char delayms){
  analogWrite(PinBEEP, 250);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(PinBEEP, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms
}

/**
 * RFID Module
 */
RFIDModule::RFIDModule() {
}

void RFIDModule::setup(){
	// beep setup
	beep_setup();

	// RFID controller setup
	// start the SPI library:
	SPI.begin();
	rfid.Init();
	Serial.println(F("MFRC522 initialized"));
#if 0
	lcd.drawString(0, 0, "Initialized");
#endif
	state = 1;
}

void RFIDModule::loop(){
	//	uchar i, tmp;
		uchar status;
		uchar buf[MAX_LEN];
	//	uchar RC_size;
	//	uchar blockAddr;	//Select operation buck address  0 - 63
	//	String mynum = "";
	//Search card, return card types
	status = rfid.Request(PICC_REQIDL, buf);

	if (status == MI_OK){
		Serial.println("Card detected");
		Serial.print(buf[0], BIN);
		Serial.print(" , ");
		Serial.print(buf[1], BIN);
		Serial.println(" ");
	}

	//Prevent conflict, return the 4 bytes Serial number of the card
	status = rfid.Anticoll(buf);

	memcpy(cardId, buf, 5);
	if (status == MI_OK){
		lcd.setBacklight(ON);

		// Should really check all pairs, but for now we'll just use the first
		if (cardId[0] == 130) {
			Serial.println("Hello Shawn");
			lcd.drawString(0,2,"Hello Shawn!");
		}else if (cardId[0] == 141) {
			Serial.println("Hello Qian");
			lcd.drawString(0,2,"Hello Qian!");
		}
		char buf[15];
		snprintf(buf,15,"ID:%02X%02X%02X%02X",cardId[0], cardId[1] , cardId[2] , cardId[3]);
		Serial.println(buf);
		lcd.drawString(0,4,buf);
		beep(200);
		state = 1;

		delay(2000);
	}

	if(state !=0){
//		lcd.clear();
//		lcd.setBacklight(OFF);
//		lcd.drawString(0,0,"Waiting...");
		Serial.println("Waiting...");
		state = 0;
	}
	rfid.Halt();			//command the card into sleep mode
}

