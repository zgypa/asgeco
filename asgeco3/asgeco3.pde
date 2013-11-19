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
#define ARDUINO 105

// Core library for code-sense
#include "Arduino.h"

// Include application, user and local libraries
#include "LocalLibrary.h"
#include "Asgeco2API.h"
#include "Syslog.h"
#include "GeneratorLibrary.h"
#include <avr/wdt.h>

byte loghost[] = { SYSLOGIP1,SYSLOGIP2,SYSLOGIP3,SYSLOGIP4 };
Globals globals = { DOWN, DOWN, false };
char logstring[18];

void setUpEthernet(){
    logg("LKUP");
    if (globals.ethernetSetup == false){
        setUpAPI();
        Syslog.setLoghost(loghost);
    }
    globals.ethernet = UP;
    globals.lastEthernetLinkState = UP;
    logg("NETUP");
}


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
    delay(1000);
    wdt_enable(WDTO_8S);  // reset after 8 second, if no "pat the dog" received
    Serial.begin(9600);
    if (digitalRead(ethernetLinkPin) == UP){
        setUpEthernet();
    }
    setUpPinMode();
    setErrorCode(ERROR_NONE);
    logg(ASGECO);
//    setState(MANU_ENABLE, ON);
}

///
/// @brief	Loop
/// @details	Call blink
///
// Add loop code 
void loop() {
    wdt_reset();
//    snprintf(logstring, sizeof(logstring), "%i %i %i",globals.ethernet,globals.lastEthernetLinkState,digitalRead(ethernetLinkPin));
//    logg(logstring);
    if ((digitalRead(ethernetLinkPin) == UP) && (globals.lastEthernetLinkState == DOWN)){
        setUpEthernet();
    }
    
    if (digitalRead(ethernetLinkPin) == DOWN){
        globals.ethernet = DOWN;
//        snprintf(logstring, sizeof(logstring), "%s %i","LKDN:",globals.ethernet);
//        logg(logstring);
        globals.lastEthernetLinkState = DOWN;
    }
    
    if (globals.ethernet == UP) {
        HTTPserver();
    }

    updateStates();
    Generator();
    
    
}

