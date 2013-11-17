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
#ifndef GeneratorLibrary_h
#define GeneratorLibrary_h

// Core library - IDE-based
#include "Arduino.h"

/*
 Pin Assignments

 Available PINS are: (0,1,)2,3,5,6,7,8,9
 0 and 1 cannot be used for digital i/o (digitalRead and digitalWrite) if i am also using serial communication (e.g. Serial.begin).

 some getter functions use PORTB, PORTC and PORTD, so they would need to get
 adjusted if adjustments are made here.

 */
//#define unusedPin          0 // free
#define usEchoPin          1
#define auxPin             2 //
#define starterPin         3 //s
#define onSolenoidPin      5 //n
#define offSolenoidPin     6 //f
#define fuelValveOpenPin   7 //
#define mains1RelayPin     8 //m1
#define ethernetLinkPin    9  //unused
#define starterCurrentPin  A0
#define usTrigPin          A1
#define batteryBankPin     A2
#define generatorOnPin     A3
#define oilSensorPin       A4
//#define relayCH7pin      A5 //unused

/*
 Engine State bits
 */
#define ENGINE          0
#define WARMINGUP       1
#define COOLINGDOWN     2
#define TIMEOUTS        3 // uses 2 bits

//#define MANU_ENABLE     5 // when set, isAUXON() returns false all the time.
//#define MANU_REQUEST    6
//#define REMOTE_ENABLE   7
//
//#define REMOTE_REQUEST  8
#define MODE            5 // when set, isAUXON() returns false all the time.
#define MANU_REQUEST    6
#define AUTO_REQUEST    7

#define AUX_STATE       8
#define OFF_LOCK        9 // when set, generator will keep load on even if auto has requested a shutdown.
//#define <FREE>        10
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
#define ES_NOTO_CTRL_MASK       0xFE07 //use this to filter out both timeouts and CTRL
#define ES_NO_CTRL_MASK         0xFE1F //use this to filter out CTRLs


// EEPROM. First 21 bytes (0-20) used for TFTP Ethernet settings.
#define EEPROMINDEX             21                  // where to start in EEPROM
#define EEPROM_TOTALRUNTIME     EEPROMINDEX+0       // offset from EEPROMINDEX 4 bytes (unsigned long)
#define EEPROM_VCONV            EEPROMINDEX+4       // offset from EEPROMINDEX 2 bytes (int)
#define EEPROM_GENON            EEPROMINDEX+6       // offset from EEPROMINDEX 2 bytes (int)
#define EEPROM_GENOFF           EEPROMINDEX+8       // offset from EEPROMINDEX 2 bytes (int)
#define EEPROM_COOLINGDOWN      EEPROMINDEX+10      // offset from EEPROMINDEX 2 bytes (int)
#define EEPROM_WARMINGUP        EEPROMINDEX+12      // offset from EEPROMINDEX 2 bytes (int)
#define EEPROM_MINIMUMRUNTIME   EEPROMINDEX+14      // offset from EEPROMINDEX 1 byte  (byte)


// Timeouts and thresholds
#define CURRENT_THRESHOLD       200
#define STARTER_TIMEOUT         7000    // in ms timeout for starter.
#define SLEEP_TIMEOUT           20000   // in ms time to wait between attempts.
#define WARM_COOL_INTERVAL      60000   // in ms interval to wait for warm up/cool down
#define VALVE_OPEN_DELAY        200     // how many ms to give current to solenoid
#define VALVE_CLOSE_DELAY       200     // how many ms to give current to solenoid
#define AUX_BUFFER_TIME         200     // in ms how long to press aux to count it as valid

unsigned int getState();

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

unsigned int getWarmUpSeconds();
void setWarmUpSeconds(unsigned int warmingUp);

unsigned int getCoolDownSeconds();
void setCoolDownSeconds(unsigned int coolingDown);

long getEngineStartTime();

unsigned long engineRunTimeMilliseconds();

unsigned long getTotalRunSecs();
void setTotalRunSecs(long secs);

/**
 * The minimum amount of time the generator should run when an AUTO request event has been triggered.
 * Value between 0-255
 */
int getMinimumRunMinutes();
void setMinimumRunMinutes(byte minutes);

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

void setMode(int mode);
int getMode();

float readVpin();

void setUpPinMode();

void Generator();
void updateStates();

int getCurrentRunningSeconds();

#endif
