///
/// @file	GeneratorLibrary.h 
/// @brief	Library header
/// @details	<#details#>
/// @n	
/// @n @b	Project asgeco3
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Antonio Magni
/// @author	Antonio Magni
/// @date	10/02/13 11:07
/// @version	<#version#>
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

#define REST_ONLY               0x0000
#define REST_MANU_ONLY          0x0020
#define REST_AUTO_ONLY          0x0200
#define REST_REMT_ONLY          0x0080
#define REST_MANU_AUTO          0x0220
#define REST_REMT_MANU          0x00A0
#define REST_REMT_AUTO          0x0280
#define ES_REST_REMT_MANU_AUTO  0x02A0
#define ES_REQ_MANU             0x0060 // manu enabled + manu request
#define ES_REQ_NOT_MANU         0x0780 // the complement of ES_REQ_MANU only of enable/request bits
#define ES_REQ_REMT             0x0180 // remote enabled + remote request
#define ES_REQ_NOT_REMT         0x0660 // remote enabled + remote request
#define ES_REQ_AUTO             0x0600 // auto enabled + auto request
#define ES_REQ_NOT_AUTO         0x01E0 // auto enabled + auto request
#define ES_ALL_REQ_ENABLED      0x07E0 // all req + enable set to 1
#define ES_WAITING_ONLY         0x4000 //WAITING
#define ES_ENG_RUN_COLD         0x1001 //ENG, VALVE

#define ES_ENG                  0x0001
#define ES_VLV                  0x1000
#define ES_VLV_ENG              0x1001
#define ES_VLV_COOL             0x1005
#define ES_VLV_STR_WAIT         0x5800
#define ES_VLV_WAIT             0x5000
#define ES_VLV_WAIT_ENG         0x5001
#define ES_VLV_ENG_WARM         0x1003
#define ES_VLV_ENG_WARM_WAIT    0x5003
#define ES_VLV_ENG_COOL_WAIT    0x5005
#define ES_VLV_ENG_MAINS        0x3001

#define ES_NOTIMEOUT_MASK       0xFFE7 //use this to filter out timeouts
#define ES_NOTO_CTRL_MASK       0xF807 //use this to filter out both timeouts and CTRL
#define ES_NO_CTRL_MASK         0xF81F //use this to filter out CTRLs



#define EEPROMINDEX             0
#define CURRENT_THRESHOLD       200
#define STARTER_TIMEOUT         7000    // in ms timeout for starter.
#define SLEEP_TIMEOUT           20000   // in ms time to wait between attempts.
#define WARM_COOL_INTERVAL      60000   // in ms interval to wait for warm up/cool down


#define ON  1
#define OPEN 1
#define CLOSE 0
#define OFF 0

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

int getVconv();
void setVconv(int vconv);

long getEngineStartTime();

unsigned long getTotalRunSecs();
void setTotalRunSecs(long secs);

int getBatt();
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

void updateStates();

void Generator();

#endif
