///
/// @file	GeneratorLibrary.h 
/// @brief	Library header
/// @details	This is the 
/// @n	
/// @n @b	Project asgeco3
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Antonio Magni
/// @author	Antonio Magni
/// @date	10/02/13 11:07
/// @version	2.1
/// 
/// @copyright	© Antonio Magni, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


// Core library - IDE-based
#include "Arduino.h"



#ifndef GeneratorLibrary_h
#define GeneratorLibrary_h


/*
 Pin Assignments

 Available PINS are: (0,1,)2,3,5,6,7,8,9
 0 and 1 cannot be used for digital i/o (digitalRead and digitalWrite) if i am also using serial communication (e.g. Serial.begin).

 some getter functions use PORTB, PORTC and PORTD, so they would need to get
 adjusted if adjustments are made here.

 */
#define unusedPin          0 // free
#define usEchoPin          1
#define auxPin             2 //
#define starterPin         3 //s
#define onSolenoidPin      5 //n
#define offSolenoidPin     6 //f
#define fuelValveOpenPin   7 //
#define mains1RelayPin     8 //m1
#define mains2RelayPin     9  //unused
#define starterCurrentPin  A0
#define usTrigPin          A1
#define batteryBankPin     A2
#define generatorOnPin     A3
#define oilSensorPin       A4
#define relayCH2Pin        A5 //unused

/*
 Engine State bits
 */
#define ENGINE          0
#define WARMINGUP       1
#define COOLINGDOWN     2
#define TIMEOUTS        3 // uses 2 bits

#define MANU_ENABLE     5 // when set, isAUXON() returns false all the time.
#define MANU_REQUEST    6
#define REMOTE_ENABLE   7

#define REMOTE_REQUEST  8
#define AUTO_ENABLE     9
#define AUTO_REQUEST    10
#define STARTER         11

#define VALVE           12
#define MAINS           13
#define WAITING         14
#define FATAL           15

/* 
 Engine state names
 */

#define ES_ENG                  0x0001
#define ES_ENG_WAIT             0x4001
#define ES_VLV                  0x1000
#define ES_VLV_ENG              0x1001
#define ES_VLV_COOL             0x1005
#define ES_VLV_STR_WAIT         0x5800
#define ES_VLV_WAIT             0x5000
#define ES_VLV_WAIT_ENG         0x5001
#define ES_VLV_ENG_WARM         0x1003
#define ES_VLV_ENG_WARM_WAIT    0x5003
#define ES_VLV_ENG_COOL_WAIT    0x5005
#define ES_VLV_ENG_COOL         0x1005
#define ES_VLV_ENG_MAINS        0x3001
#define ES_WAIT_COOL            0x4004

#define ES_NOTIMEOUT_MASK       0xFFE7 //use this to filter out timeouts
#define ES_NOTO_CTRL_MASK       0xF807 //use this to filter out both timeouts and CTRL
#define ES_NO_CTRL_MASK         0xF81F //use this to filter out CTRLs



#define EEPROMINDEX             0
#define CURRENT_THRESHOLD       200
#define STARTER_TIMEOUT         7000    // in ms timeout for starter.
#define SLEEP_TIMEOUT           20000   // in ms time to wait between attempts.
#define WARM_COOL_INTERVAL      60000   // in ms interval to wait for warm up/cool down


#define ON      1
#define OPEN    1
#define CLOSE   0
#define OFF     0

boolean getState(byte b);
void setState(byte b, byte s);

byte getStarter();
void setStarter(byte i);

byte getEngine();
void setEngine(byte i);

byte getAttempts();
void setAttempts(byte i);

byte getValve();
void setValve(byte i);

byte getMains();
void setMains(byte i);

byte getOil();

int getVconv();
void setVconv(int vconv);

long getEngineStartTime();

unsigned long getTotalRunSecs();
void setTotalRunSecs(long secs);

unsigned int getBatt();
void setBatt(int vbatt);

long readVcc();

int getGENON();
void setGENON(int g);

int getGENOFF();
void setGENOFF(int g);

byte getInputPin(byte pin);

void openFuelValve();
void closeFuelValve();

float readVpin();

void setUpPinMode();

void Generator();

#endif
