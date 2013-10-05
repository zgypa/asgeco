/// 
/// @mainpage	asgeco3 
/// @details	<#details#>
/// @n 
/// @n 
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Antonio Magni
/// @author	Antonio Magni
/// @date	09/02/13 18:40
/// @version	<#version#>
/// 
/// @copyright	© Antonio Magni, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


///
/// @file	asgeco3.pde 
/// @brief	Main sketch
/// @details	<#details#>
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Antonio Magni
/// @author	Antonio Magni
/// @date	09/02/13 18:40
/// @version	<#version#>
/// 
/// @copyright	© Antonio Magni, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
/// @n
///
#define ARDUINO 103

// Core library for code-sense
#include "Arduino.h"

// Include application, user and local libraries
#include "LocalLibrary.h"
#include "Asgeco2API.h"
#include "Syslog.h"
#include "GeneratorLibrary.h"

byte loghost[] = { SYSLOGIP1,SYSLOGIP2,SYSLOGIP3,SYSLOGIP4 };



// Define variables and constants
///
/// @brief	Name of the LED
/// @details	Each board has a LED but connected to a different pin
///


///
/// @brief	Setup
/// @details	Define the pin the LED is connected to
///
// Add setup code 
void setup() {
//    Serial.begin(9600);
//    Serial.print("ON:");
    setUpAPI();
    Syslog.setLoghost(loghost);
    setUpPinMode();
    logg(ASGECO);
//    setState(MANU_ENABLE, ON);
}

///
/// @brief	Loop
/// @details	Call blink
///
// Add loop code 
void loop() {
    HTTPserver();
    updateStates();
//    if ((getState(MODE) == AUTO) ||
//        ((getState(MODE) == MANUAL) && (getState(MANU_REQUEST) == ON)))
        // @TODO
        // This might not be a good idea, because if i switch on, all good, but if i
        // switch off, the state machine will shut down immediately, and not turn
        // engine off. State machine must be disabled from state machine itself, once it's
        // done doing it's thing.
        Generator();
}
