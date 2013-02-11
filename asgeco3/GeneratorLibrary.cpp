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

void setState(byte b, byte s){
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
    return digitalRead(generatorOnPin);
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
    return getInputPin(fuelValveOpenPin);
}
void setValve(byte i){
    if (i) openFuelValve();
    else closeFuelValve();
}

byte getMains(){
    return byte(bitRead(PORTB,mains1RelayPin-8));
}
void setMains(byte i){
    digitalWrite(mains1RelayPin, i);
    logg("MAINS  " + String(i));
}

/*
 Gets vConv from EEPROM
 */
int getVconv(){
    int vConv;
    EEPROM_readAnything(EEPROMINDEX+4, vConv);
    return vConv;
}

void setVconv(int vConv){
    EEPROM_writeAnything(EEPROMINDEX+4, vConv);
}


long getEngineStartTime(){
    return engineStartTime;
}

unsigned long getTotalRunSecs(){
    unsigned long total;
    EEPROM_readAnything(EEPROMINDEX, total);
    return total;
}

void setTotalRunSecs(long secs){
    EEPROM_writeAnything(EEPROMINDEX, secs);
}

/*
 
 Return the value of the sensor (voltage divider) in mV, rounded down to 10th of mV.
 
 Vin = Vout * (R1 + R2)/R1
 R1 = 11.3kOhm
 R2 = 96.2kOhm
 => Vin = Vout * 9.51
 
 */
int readBatt(){
    int vb = readVpin() * getVconv() / 1000;
    vb = round(vb / 100.0);
    return (vb*100);
}

/*
 First calculates vConv from vBatt, then stores it into EEPROM.
 */
void setBatt(int vBatt){
    setVconv(vBatt / readVpin() * 1000);// perform *1000 at last to avoid overflow
}

/*
 Code from http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
 
 Many AVR chips including the ATmega series and many ATtiny series provide a means to measure
 the internal voltage reference. Why would anyone want to do so? The reason is simple – by
 measuring the internal reference, we can determine the value of Vcc. Here’s how:
 
 First set the voltage reference to Vcc.
 Measure the value of the internal reference.
 Calculate the value of Vcc.
 Our measured voltage is:
 
 Vcc * (ADC-measurement) / 1023
 
 which as we know is 1.1 volts. Solving for Vcc, we get:
 
 Vcc = 1.1 * 1023 / ADC-measurement
 
 Putting it altogether, here’s the code:
 */
long readVcc() {
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
#else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
    
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Start conversion
    while (bit_is_set(ADCSRA,ADSC)); // measuring
    
    uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both
    
    long result = (high<<8) | low;
    
    result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
    return result; // Vcc in millivolts
}


int getGENON(){
    int G;
    EEPROM_readAnything(EEPROMINDEX+8, G);
    return G;
}
void setGENON(int g){
    EEPROM_writeAnything(EEPROMINDEX+8, g);
}

int getGENOFF(){
    int G;
    EEPROM_readAnything(EEPROMINDEX+12, G);
    return G;
}

void setGENOFF(int g){
    EEPROM_writeAnything(EEPROMINDEX+12, g);
}

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

byte getAux(){
    return getInputPin(auxPin);
}


/*
 This functions checks the input pin to see if it has been switched on. It takes
 a number of readings, counts the ONs and the OFFs. Returns the state in which it
 had most counts.
 
 This has been done to improve stability.
 */
byte getInputPin(byte pin){
    int ons  = 0;
    int offs = 0;
    while (ons == offs) { // keeping counting if we have tides.
        for (int i = 0; i < 10; i++) { //selecting an even number will give the opportunity of re-reading in case of tides.
            if (digitalRead(pin) == 1) offs++;
            else ons++;
        }
    }
    if (ons > offs) return 1;
    else return 0;
}

/*
 
 Return the value of the input pin batteryBankPin in mV.
 
 */
float readVpin(){
    const int samples=60; // num of samples to average out
    long total, totalVcc;
        
    // compute average
    total = 0;
    totalVcc = 0;
    for (int i=0;i<samples;i++)
    {
        total+=analogRead(batteryBankPin);
        totalVcc+=readVcc();
    }
    float v = (total/samples) / 1023.0 * (totalVcc/samples);
    return (v); // see calculations in comments above
}

void setUpPinMode(){
    /*
     Note that because the W5100 and SD card share the SPI bus, only one can be active at a time. If you are using both peripherals in
     your program, this should be taken care of by the corresponding libraries. If you're not using one of the peripherals in your
     program, however, you'll need to explicitly deselect it. To do this with the SD card, set pin 4 as an output and write a high to it.
     For the W5100, set digital pin 10 as a high output.
     */
    pinMode(4, OUTPUT); digitalWrite(4, HIGH);
    
    pinMode(auxPin, INPUT); digitalWrite (auxPin, HIGH); // enable pullup resistore
    pinMode(starterPin, OUTPUT);
    pinMode(onSolenoidPin, OUTPUT);
    pinMode(offSolenoidPin, OUTPUT);
    pinMode(mains1RelayPin, OUTPUT);
    //  pinMode(mains2RelayPin, OUTPUT);
    pinMode(fuelValveOpenPin, INPUT); digitalWrite (fuelValveOpenPin, HIGH); // enable pullup resistor
    pinMode(starterCurrentPin, INPUT);
    pinMode(oilSensorPin, INPUT); digitalWrite (oilSensorPin, HIGH); // enable pullup resistor
    pinMode(generatorOnPin, INPUT); digitalWrite (generatorOnPin, HIGH); // enable pullup resistor
    pinMode(batteryBankPin, INPUT);
    pinMode(oilSensorPin, INPUT); digitalWrite (oilSensorPin, HIGH); // enable pullup resistor

}

void updateStates(){
    int past = engineState;
    setState(ENGINE,getEngine());
    setState(VALVE,getValve());
    setState(MAINS,getMains());
    setState(STARTER,getStarter());
    setState(MANU_CONTROL,getAux());
    if (past != engineState)
        logg(String("New: ") + String(engineState));
}


void Generator(){
    updateStates();
}