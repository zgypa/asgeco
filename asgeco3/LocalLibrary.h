///
/// @file	LocalLibrary.h 
/// @brief	Library header
/// @details	<#details#>
/// @n	
/// @n @b	Project asgeco3
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
#ifndef LocalLibrary_h
#define LocalLibrary_h

// Core library - IDE-based
#include "Arduino.h"

// Set to 0 for manual IP, 1 for DHCP.
#define DHCP 0

// IP address of Arduino, separated in octets
#define IPADDR1 10
#define IPADDR2 1
#define IPADDR3 16
#define IPADDR4 181

// IP address of syslog server, seprated in octets
#define SYSLOGIP1 10
#define SYSLOGIP2 1
#define SYSLOGIP3 16
#define SYSLOGIP4 1

#define MAC1 0x00
#define MAC2 0x08
#define MAC3 0xDC
#define MAC4 0xB7
#define MAC5 0xD6
#define MAC6 0xDA

// Port of http server
#define HTTPPORT  80

//extern byte mac_addr[6];


/// @brief	Log a string
/// @details	The passed string is logged
/// @n		Total cycle duration = ms
/// @param	pin pin to which the LED is attached
/// @param	times number of times
/// @param	ms cycle duration in ms
///
extern void logg(String string);


#endif
