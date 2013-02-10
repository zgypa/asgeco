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
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"   
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries
#include "LocalLibrary.h"
#include "Asgeco2API.h"

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
   setUpAPI();
}

///
/// @brief	Loop
/// @details	Call blink
///
// Add loop code 
void loop() {
    HTTPserver();
}
