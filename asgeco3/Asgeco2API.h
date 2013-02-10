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
/// @version	<#version#>
/// 
/// @copyright	© Antonio Magni, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


// Core library - IDE-based
#include "Arduino.h"
#include "EthernetClient.h"

#define ASGECO "ASGECOv2"
#define BUFSIZ 100  //Buffer size for getting data


#ifndef Asgeco2API_h
#define Asgeco2API_h

void printState(EthernetClient ec);
void writeStates(char clientline[]);
void setUpAPI();
void HTTPserver();

#endif
