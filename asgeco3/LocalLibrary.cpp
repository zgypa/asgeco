//
// LocalLibrary.cpp 
// Library C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project asgeco3
//
// Created by Antonio Magni, 09/02/13 18:40
// Antonio Magni
//	
//
// Copyright © Antonio Magni, 2013
// Licence CC = BY NC SA
//
// See LocalLibrary.cpp.h and ReadMe.txt for references
//

#include "LocalLibrary.h"
#include "Asgeco2API.h"
#include "Syslog.h"

byte errorCode;


// Wiznet prefix is 00:08:DC
// generated with http://www.miniwebtool.com/mac-address-generator/
//byte mac_addr[6] = { MAC1, MAC2, MAC3, MAC4, MAC5, MAC6 };

String lastLog;
//char logstring[18];
//char[] lastLog;

// this function will return the number of bytes currently free in RAM
// written by David A. Mellis
// based on code by Rob Faludi http://www.faludi.com
//int availableMemory() {
//    int size = 2048; // Use 2048 with ATmega328
//    byte *buf;
//    
//    while ((buf = (byte *) malloc(--size)) == NULL)
//        ;
//    
//    free(buf);
//    
//    return size;
//}
//

void logg(String string) {
    if (LOG == 1){
        if (string != lastLog) {
            if (globals.ethernet == UP)
                Syslog.logger(1,6,ASGECO, string);
            Serial.println(string);
            lastLog = string;
        }
    }
}

byte getErrorCode(){
    return errorCode;
}

void setErrorCode(byte e){
    errorCode = e;
}