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
#define DHCP 1

// IP address of Arduino, separated in octets
#define IPADDR1 10
#define IPADDR2 1
#define IPADDR3 16
#define IPADDR4 81

#define IPMASK1 255
#define IPMASK2 255
#define IPMASK3 255
#define IPMASK4 0

#define GW1 10
#define GW2 1
#define GW3 16
#define GW4 1

#define DNS1 10
#define DNS2 1
#define DNS3 16
#define DNS4 6

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

#define EEPROM_SIG_1_VALUE 0x55
#define EEPROM_SIG_2_VALUE 0xAA

#define EEPROM_SIG_1_OFFSET 0
#define EEPROM_SIG_2_OFFSET 1

#define EEPROM_GATEWAY_OFFSET 3
#define EEPROM_MASK_OFFSET 7
#define EEPROM_MAC_OFFSET 11
#define EEPROM_IP_OFFSET 17


// Port of http server
#define HTTPPORT  80

//extern byte mac_addr[6];

#define ON      1
#define OPEN    1
#define AUTO    1
#define DOWN    1
#define OFF     0
#define CLOSE   0
#define MANUAL  0
#define UP      0

#define LOG     0

#define ERROR_NONE                              0

// 10-19 starter errors
#define ERROR_ENGINE_NOT_STARTING               11

// 20-29 open valve solenoid errors
#define ERROR_VALVE_NOT_OPENING                 21

// 30-39 close valve solenoid errors

// 40-49 engine errors (oil, temp)
#define ERROR_LOW_OIL                           41
#define ERROR_ENGINE_OVERHEAT                   42

// 50-59 mains errors

// 60-69

// 70-79

// 80-89

// 90-99 network errors
#define ERROR_NETWORK_LINK_DOWN                 91

// 110-119 fuel tank errors
#define ERROR_LOW_FUEL                          111
#define ERROR_TOO_MUCH_FUEL                     112

// 200-255 software errors
#define ERROR_UNKOWN_STATE                      201

/// @brief	Log a string
/// @details	The passed string is logged
/// @n		Total cycle duration = ms
/// @param	pin pin to which the LED is attached
/// @param	times number of times
/// @param	ms cycle duration in ms
///
extern void logg(String string);


typedef struct {
    byte lastEthernetLinkState      : 1;
    byte ethernet                   : 1;
    byte ethernetSetup              : 1;
} Globals;

extern Globals globals;
extern char logstring[18];

byte getErrorCode();
void setErrorCode(byte e);

#endif
