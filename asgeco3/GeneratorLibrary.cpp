//
// GeneratorLibrary.cpp 
// Library C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project asgeco3
//
// Created by Antonio Magni, 10/02/13 11:07
// Antonio Magni
//	
//
// Copyright © Antonio Magni, 2013
// Licence CC = BY NC SA
//
// See GeneratorLibrary.h and ReadMe.txt for references
//


// Library header
#include "GeneratorLibrary.h"
#include "EEPROMAnything.h"
#include "LocalLibrary.h"

// Code
int engineState      = 0;
long engineStartTime = 0;



boolean getState(byte b){
//    if ((engineState >> b ) & 1)
//        return true;
//    else return false;
    return bitRead(engineState,b);
}

void setState(byte b, boolean s){
//    if (s)
//        engineState |= (1 << b);
//    else
//        engineState &= ~(1 << b);
    bitWrite(engineState, b, s);
}

byte getStarter(){
    return byte(bitRead(PORTD,starterPin));
};

void setStarter(byte i){
    digitalWrite(starterPin, i);
    logg("Starter " + String(i));
}

byte getEngine(){
    return byte(bitRead(PORTC,3));
}

void setEngine(byte i){
// not sure i need this.
}

byte getAttempts(){
    byte a = engineState >> TIMEOUTS; // extract the value of attempts from engineState
    a &= (1<<(2+1))-1;         // leaves alone the lowest 2 bits of x; all higher bits set to 0.
    return a;
}

void setAttempts(byte a){
    for (int i = 0; i<2; i++){
        if ((a >> i) & 1) setState(TIMEOUTS+i, true);
        else setState(TIMEOUTS+i, false);
    }
}

byte getValve(){
    return isInputOn(fuelValveOpenPin);
}
void setValve(byte i){
    if (i) openFuelValve();
    else closeFuelValve();
}

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

void openFuelValve(){
    logg("Opening valve");
    while(! getValve()) digitalWrite(onSolenoidPin, HIGH);
    digitalWrite(onSolenoidPin, LOW);
    logg("Valve 1");
}

void closeFuelValve(){
    logg("Closing valve");
    digitalWrite(offSolenoidPin, HIGH); //try to close regardless of microswitch, which might not be reliable.
    delay(100);
//    if (getValve()) digitalWrite(offSolenoidPin, HIGH);
    digitalWrite(offSolenoidPin, LOW);
    logg("Valve 0");
}

/*
 This functions checks the intput pin to see if it has been switched on. It takes
 a number of readings, counts the ONs and the OFFs. Returns the state in which it
 had most counts.
 
 This has been done to improve stability.
 */
boolean isInputOn(int pin){
    int ons  = 0;
    int offs = 0;
    while (ons == offs) { // keeping counting if we have tides.
        for (int i = 0; i < 10; i++) { //selecting an even number will give the opportunity of re-reading in case of tides.
            if (digitalRead(pin) == 1) offs++;
            else ons++;
        }
    }
    if (ons > offs) return true;
    else return false;
}

void updateStates(){
    
}
