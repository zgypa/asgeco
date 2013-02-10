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

#include "Asgeco2API.h"
#include "LocalLibrary.h"
#include "Syslog.h"

void logg(String string) {
    Syslog.logger(1,6,ASGECO,string);
}
