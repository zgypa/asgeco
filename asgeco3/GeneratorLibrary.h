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
#define inputPin           2 //
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
 Engine States
 */
#define ENGINE          0
#define WARMINGUP       1
#define COOLINGDOWN     2
#define TIMEOUTS        3 // uses 2 bits

#define MANU_ENABLE     5 // when set, isAUXON() returns false all the time.
#define MANU_CONTROL    6
#define REMOTE_ENABLE   7

#define REMOTE_CONTROL  8
#define AUTO_ENABLE     9
#define AUTO_CONTROL    10
#define STARTER         11

#define VALVE           12
#define MAINS           13
#define WAITING         14
#define FATAL           15

#define EEPROMINDEX     0



boolean getState(byte b);
void setState(byte b, boolean s);

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

int readBatt();
void setBatt(int vbatt);

long readVcc();

int getGENON();
void setGENON(int g);

int getGENOFF();
void setGENOFF(int g);

void updateStates();

#endif
