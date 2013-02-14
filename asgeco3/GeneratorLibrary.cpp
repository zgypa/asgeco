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
unsigned int engineState    = 0;
long engineStartTime        = 0;
long waitStartTime          = 0;
int initialCurrent        = 0;


boolean getState(byte b){
//    if ((engineState >> b ) & 1)
//        return true;
//    else return false;
    return bitRead(engineState,b);
}

void setState(byte b, byte s){
    if (getState(b) != s) {
        bitWrite(engineState, b, s);
//        logg(String("Set: ") + String(b));
        logg(String("New: 0x") + String(engineState, HEX));
    }
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
//    byte a = engineState >> TIMEOUTS; // extract the value of attempts from engineState
//    a &= (1<<(2+1))-1;         // leaves alone the lowest 2 bits of x; all higher bits set to 0.
    byte a = 0;
    bitWrite(a, 0, bitRead(engineState, TIMEOUTS+0));
    bitWrite(a, 1, bitRead(engineState, TIMEOUTS+1));
    
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
unsigned int getBatt(){
    // something wrong here: value seems to overflow.
    unsigned int vb = readVpin() * getVconv() / 1000;
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


/*
 open and close fuelvalve functions are not responsible for checking the state of the microswitch.
 This should be done making use of states.
 */
void openFuelValve(){
    logg("Opening valve");
    digitalWrite(onSolenoidPin, HIGH);
    delay(200);
    digitalWrite(onSolenoidPin, LOW);
}

void closeFuelValve(){
    logg("Closing valve");
    digitalWrite(offSolenoidPin, HIGH);
    delay(300);
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

    setState(MANU_ENABLE, 1);
    
//    Serial.println(ES_REQ_MANU, HEX);
//    Serial.println(ES_REST_REMT_MANU_AUTO, HEX);


}

/*
 Stores amount of runtime in EEPROM.
 
 Returns amount logged this time.
 */
unsigned long logOnTime(){
    unsigned long logthis = (getTotalRunSecs() + (millis() - engineStartTime)/1000);
    EEPROM_writeAnything(EEPROMINDEX, logthis);
    unsigned long ee;
    EEPROM_readAnything(EEPROMINDEX, ee);
    if (logthis != ee)
        logg("EEPROM CHK FAIL");
    else
        logg(String("Logged ") + String(logthis));
    return logthis;
}

/*
 
 A Valid Request is a request which is accompanied with an enabled pin. So MANU_REQUEST set to high,
 is not valid unless MANU_ENABLE is also set.
 
 Returns true only if there is an enabled AND requested pin
 */
boolean isValidRequest(){
    if (((getState(MANU_ENABLE)&getState(MANU_REQUEST)) == 0) &&
        ((getState(REMOTE_ENABLE)&getState(REMOTE_REQUEST)) == 0) &&
        ((getState(AUTO_ENABLE)&getState(AUTO_REQUEST)) == 0))
        return false;
    else return true;
}

void setWaiting(byte b){
    if (b == 1) {
        waitStartTime = millis();
        setState(WAITING,1);
    } else {
        waitStartTime = 0;
        setState(WAITING,0);
    }
}


int readCurrent(){
    const byte buf = 10;
    unsigned int sensorValue = 0;
    for(int i=0;i<buf;i++) sensorValue += analogRead(starterCurrentPin);
    return sensorValue/buf; // take an average of buf readings.
    
}

/*
 increases attempts. Every time the engine fails to start, an attempt is made, and this counter is increased.
 
 Timeout is encoded in 2 bits: TIMEOUT and TIMEOUT +1, for total of 4 attempts.
 */
void increaseAttempts(){
    byte a = getAttempts();
    a++;  // now the value is in a, and we can increment it.
    setAttempts(a);
    if (a == 3) {
        logg("FATAL after 4 attempts");
        setState(FATAL, true);
    }
}



void updateStates(){
    if(getState(ENGINE) != getEngine()){
        setState(ENGINE,getEngine());
        if (getState(ENGINE) == ON) {   // engine just started
            engineStartTime = millis(); // start counter
            setAttempts(0);             // if running, attempts have to be reset
        }
        else {
            logOnTime();
            engineStartTime = 0;
        }
    }
    setState(VALVE,getValve());
    setState(MAINS,getMains());
    setState(STARTER,getStarter());
    setState(MANU_REQUEST,getAux());
    if(getBatt() < getGENON())  setState(AUTO_REQUEST, ON);
    if(getBatt() > getGENOFF()) setState(AUTO_REQUEST, OFF);
}

/*
 Engine not running, no request to start. No Timeouts.
 
 This state also takes into account state 10a in state diagram.
 */
byte isState10(){
    return ((engineState & ES_NOTO_CTRL_MASK) == 0) && ! isValidRequest();
}

/*
 Engine not running, but request to start rcvd.
 */
byte isState11(){
    return ((engineState & ES_NOTO_CTRL_MASK) == 0) && isValidRequest();
}

/*
 Engine not running, request, valve open, timeouts accepted (i.e. ignored)
 */
byte isState12(){
    return (isValidRequest() && ((engineState & ES_NOTO_CTRL_MASK) == ES_VLV));
}

/*
 Engine not running, request lost, valve open.
 */
byte isState120(){
    return (!isValidRequest() && ((engineState & ES_NOTO_CTRL_MASK) == ES_VLV));
}

/*
 Engine not running, request, valve, starter, waiting.
 */
byte isState13(){
    return (isValidRequest() && ((engineState & ES_NOTO_CTRL_MASK) == ES_VLV_STR_WAIT));
}

/*
 Engine not running, request lost, valve, starter, waiting.
 */
byte isState130(){
    return (!isValidRequest() && ((engineState & ES_NOTO_CTRL_MASK) == ES_VLV_STR_WAIT));
}

/*
 This is a transition state in which starter was just shut down.
 
 TRIGGER: STATE13 if current dropped
 
 Engine not running, request, valve, waiting.
 */
byte isState14(){
    return (isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_WAIT));
}

/*
 This is a transition state in which starter was just shut down.
 
 TRIGGER: request drop
 
 Engine not running, request lost, valve, waiting.
 */
byte isState140(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_WAIT));
}

/*
 This is a transition state arrived by the engine starting up.
 
 TRIGGER: engine alone, starting up via generatorOnPin
 
 Engine, request, valve, waiting.
 */
byte isState15(){
    return (isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_WAIT_ENG));
}

/*
 This is a transition state arrived by the engine starting up.
 
 TRIGGER: request drop
 
 Engine, request lost, valve, waiting.
 */
byte isState150(){
    return (! isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_WAIT_ENG));
}

/*
 This is a transition state in which the engine just started up.
 We can start engine time counter here and prepare for warm up.
 
 TRIGGER: STATE15
 
 Engine, request, valve, warming.
 */
byte isState16(){
    return (isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_WARM));
}

/*
 This is a transition state in which the engine just started up and request was lost.
 
 TRIGGER: request drop
 
 Engine, request lost, valve, warming.
 */
byte isState160(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_WARM));
}

/*
 This is a resting state, in which the engine warms up.
 
 TRIGGER: STATE16
 
 Engine, request, valve, warming, waiting.
 */
byte isState17(){
    return (isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_WARM_WAIT));
}

/*
 This is a transition state, in which the request was dropped while engine warms up.
 
 TRIGGER: request drop
 
 Engine, request lost, valve, warming, waiting.
 */
byte isState170(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_WARM_WAIT));
}

/*
 This is a resting state, in which the generator is fully operational.
 
 TRIGGER: STATE17
 
 Engine, request, valve, mains.
 */
byte isState20(){
    return (isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_MAINS));
}


/*
 This is a transition state, in which is still fully operational.
 
 TRIGGER: lost request to run
 
 Engine, valve, mains.
 */
byte isState21(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_MAINS));
}

/*
 This is a transition state, in which mains just got turned off.
 
 TRIGGER: STATE21
 
 Engine, valve.
 */
byte isState22(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG));
}

/*
 This is a transition state, req appeared again just when mains got turned off.
 
 TRIGGER: STATE22
 
 Engine, valve, req.
 */
byte isState220(){
    return (isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG));
}

/*
 This is a transition state, in preparation for cooling.
 
 TRIGGER: STATE22
 
 Engine, valve, cooling.
 */
byte isState23(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_COOL));
}

/*
 This is a transition state, req appeared again just when mains got turned off.
 
 TRIGGER: STATE23
 
 Engine, valve, cooling.
 */
byte isState230(){
    return (isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_COOL));
}

/*
 This is a resting state, in which the engine cools down.
 
 TRIGGER: STATE23
 
 Engine, valve, cooling, waiting.
 */
byte isState24(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_COOL_WAIT));
}

/*
 This is a transition state, req appeared again during cooling.
 
 TRIGGER: STATE24
 
 Engine, valve, cooling.
 */
byte isState240(){
    return (isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_VLV_ENG_COOL_WAIT));
}

/*
 This is a transition state, in which the engine prepares to shut down.
 
 TRIGGER: STATE24
 
 Engine, waiting.
 */
byte isState241(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_ENG_WAIT));
}

/*
 This is a transition state, funny state. Probably engine shut off while cooling.
 
 TRIGGER: external
 
 cooling, waiting.
 */
byte isState242(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_WAIT_COOL));
}

/*
 This is a transition state, in which the engine has been shut off, but is still running.
 
 TRIGGER: STATE24
 
 Engine.
 */
byte isState25(){
    return (!isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_ENG));
}

/*
 This is a transition state, req appeared while engine shutting down.
 Here, just delay 3000 secs. Eventually eng will stop and we will be in state 11.
 
 TRIGGER: STATE24
 
 Engine, request.
 */
byte isState250(){
    return (isValidRequest() && ((engineState & ES_NO_CTRL_MASK) == ES_ENG));
}

/*
 This is a resting state, waiting for resting period before trying again.
 
 TRIGGER: STATE51
 
 Request, valve, timeout, wait.
 */
byte isState52(){
    return (isValidRequest() && (getAttempts()>0) &&
            ((engineState & ES_NOTO_CTRL_MASK) == ES_VLV_WAIT));
}

/*
 This is a resting state, we are dead.
 
 TRIGGER: STATES 13
 
 FATAL and anything else.
 */
byte isState90(){
    return (getState(FATAL));
}


void Generator(){
    updateStates();

    if (isState10()) {
        // do nothing, just wait for state to change

    } else if (isState11()){
        // There has been a valid manual request to start
        logg("S11");
        setValve(OPEN);

    } else if (isState12()){
        logg("S12");
        setStarter(ON);
        setWaiting(ON);
        delay(300); // give time for the system to draw current.
        initialCurrent = readCurrent();

    } else if (isState120()){
        logg("S120");
        setValve(CLOSE);

    } else if (isState13()){
        logg("S13:Starter running.");
        logg(String(initialCurrent));
        logg(String(readCurrent()));
        logg(String(initialCurrent - readCurrent()));
        if (initialCurrent - readCurrent() > CURRENT_THRESHOLD) {
            logg("Dropped");
            setStarter(OFF);
            setAttempts(0);
        } else if (millis() - waitStartTime > STARTER_TIMEOUT ) {
            logg("Timeout");
            setStarter(OFF);
            setWaiting(OFF);
            setWaiting(ON); // reset waiting timer for S52
            increaseAttempts();
            logg(String("TO: ") + String(getAttempts()));
            if (getAttempts() >= 3) setState(FATAL, ON);
        }
    } else if (isState130()){
        logg("S130");
        setStarter(OFF);
        setValve(OFF);
        setWaiting(OFF);

    } else if (isState14()){
        logg("S14");

    } else if (isState140()){
        logg("S140");
        setValve(CLOSE);
        setWaiting(OFF);

    } else if (isState15()){
        logg("S15");
        // Engine is running, need to warm up
        // engineStartTime is set automatically by updateStatus.
        setWaiting(OFF);
        setState(WARMINGUP, ON);

    } else if (isState150()){
        logg("S150");
        setValve(OFF);
        setWaiting(OFF);

    } else if (isState16()){
        logg("S16");
        setWaiting(ON);

    } else if (isState160()){
        logg("S160");
        setValve(OFF);
        setState(WARMINGUP, OFF);

    } else if (isState17()){
        // Now we are warming up
        if (millis() - waitStartTime > WARM_COOL_INTERVAL) {
            logg("S17");
            setWaiting(OFF);
            setState(WARMINGUP, OFF);
            setMains(ON);
        }
        
    } else if (isState170()){
        logg("S170");
        setValve(OFF);
        setState(WARMINGUP, OFF);
        setWaiting(OFF);

    } else if (isState20()){
        // Generator full ON. Wait for request to drop.

    } else if (isState21()){
        logg("S21");
        // Received request to stop
        setMains(OFF);
        setState(COOLINGDOWN, ON);

    } else if (isState22()){
        logg("S22");
        // Do nothing. Wait for valve to close.

    } else if (isState220()){
        logg("S220");
        setMains(ON);

    } else if (isState23()){
        logg("S23");
        setWaiting(ON);

    } else if (isState230()){
        logg("S230");
        setState(COOLINGDOWN, OFF);
        setMains(ON);
    
    } else if (isState24()){
        // Now we are cooling off
        if (millis() - waitStartTime > WARM_COOL_INTERVAL) {
            logg("S24");
            setState(COOLINGDOWN, OFF);
            setValve(CLOSE);
            setWaiting(OFF);
        }
 
    } else if (isState240()){
        // go back to State 20
        setState(COOLINGDOWN, OFF);
        setWaiting(OFF);
        setMains(ON);
    
    } else if (isState241()){
        // dangling state. Fix:
        setWaiting(OFF);
        
    } else if (isState242()){
        // dangling state. Fix:
        setWaiting(OFF);
        setState(COOLINGDOWN, OFF);
        
    } else if (isState25()){
        logg("S25");
        // do nothing, just wait for engine to turn off
    
    } else if (isState250()){
        logg("S250");
        // freeze system for a couple of secends, engine is shutting down.
        delay(2000);

    } else if (isState52()){
        logg("S52");
        // starting timed out. Waiting some time before trying again
        if (millis() - waitStartTime > SLEEP_TIMEOUT) {
            setWaiting(OFF); //this should throw us back to state 12.
        }

    } else if (isState90()){
        logg("FATAL");
    } else {
        logg("ERR:0x" + String(engineState, HEX));
    }
}