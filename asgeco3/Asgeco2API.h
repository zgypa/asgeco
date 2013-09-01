///
/// @file	Asgeco2API.h 
/// @brief	Library header
/// @details	<#details#>
/// @n	
/// @n @b	Project asgeco3
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Antonio Magni
/// @author	Antonio Magni
/// @date	10/02/13 11:50
/// @version	2.1
/// 
/// @copyright	© Antonio Magni, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///
#ifndef Asgeco2API_h
#define Asgeco2API_h


// Core library - IDE-based
#include "Arduino.h"
#include "EthernetClient.h"

#define ASGECO "ASGECOv2"
#define BUFSIZ 100  //Buffer size for getting data


/*
 Writable states. See
 https://wiki.afm.co/display/CIAP/Asgeco+2.1+firmware+API+%28current%29
 */
#define API_STARTER         2
#define API_ONSOLENOID      3
#define API_OFFSOLENOID     4
#define API_MAINS           5
#define API_MAN_REQUEST     6
#define API_MODE            14
#define API_FATAL           15
#define API_TIMEOUTS        18
#define API_SECS_TOT        19
#define API_GENON           21
#define API_GENOFF          22
#define API_VBATT           25
#define API_WARMINGUP       27
#define API_COOLINGDOWN     28


void printState(EthernetClient ec);
void writeStates(char clientline[]);
void setUpAPI();
void HTTPserver();

#endif
