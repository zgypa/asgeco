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

// Wiznet prefix is 00:08:DC
// generated with http://www.miniwebtool.com/mac-address-generator/
//byte mac_addr[6] = { MAC1, MAC2, MAC3, MAC4, MAC5, MAC6 };

String lastLog;

void logg(String string) {
    if (string != lastLog) {
        Syslog.logger(1,6,ASGECO,string);
        lastLog = string;
    }
}
