//
// EthernetLibrary.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project asgeco3
//
// Created by Antonio Magni, 10/02/13 11:08
// Antonio Magni
//
//
// Copyright © Antonio Magni, 2013
// Licence CC = BY NC SA
//
// See EthernetLibrary.h and ReadMe.txt for references
//


// Library header
#include <Ethernet.h>
#include <SPI.h>
#include <Syslog.h>
#include "Asgeco2API.h"
#include "GeneratorLibrary.h"
#include "LocalLibrary.h"

// Wiznet prefix is 00:08:DC
byte mac[] = { 0x00, 0x08, 0xDC, 0xB7, 0xD6, 0xDA }; //generated with http://www.miniwebtool.com/mac-address-generator/
IPAddress ip(10,1,11,80);
EthernetServer server(80);

// Code
void setUpAPI(){
    Ethernet.begin(mac, ip);
}

void printState(EthernetClient ec) {
    char c = ' ';
    ec.print(ASGECO);//1
    ec.print(c);
    ec.print(bitRead(PORTD,starterPin));//2
    ec.print(c);
    ec.print(bitRead(PORTD,onSolenoidPin), DEC) ;//3
    ec.print(c);
    ec.print(bitRead(PORTD,offSolenoidPin), DEC) ;//4
    ec.print(c);
    ec.print(bitRead(PORTB,mains1RelayPin-8), DEC);//5
    ec.print(c);
    ec.print(digitalRead(inputPin), DEC);//6
    ec.print(c);
//    ec.print(readBatt());//7
    ec.print(c);
//    ec.print(readVcc());//8
    ec.print(c);
    ec.print(analogRead(starterCurrentPin)) ;//9
    ec.print(c);
//    ec.print(isON(ENGINE));//10
    ec.print(c);
//    ec.print(isON(WARMINGUP));//11
    ec.print(c);
//    ec.print(isON(COOLINGDOWN));//12
    ec.print(c);
//    ec.print(isON(SLEEP));//13
    ec.print(c);
//    ec.print(isON(AUXDISABLE));//14
    ec.print(c);
//    ec.print(isON(FATAL));//15
    ec.print(c);
//    ec.print(isON(LASTAUXSTATE));//16
    ec.print(c);
//    ec.print(isON(AUTO));//17
    ec.print(c);
//    ec.print(getAttempts());//18
    ec.print(c);
//    ec.print(getTotalRunSecs(),DEC);//19
    ec.print(c);
//    if(isEngineRunning())
//        ec.print((millis() - getEngineStartTime())/1000);//20
//    else
//        ec.print(0);
    ec.print(c);
//    ec.print(getGENON());//21
    ec.print(c);
//    ec.print(getGENOFF());//22
    ec.print(c);
//    ec.print(isON(REMOTE_ENABLE));//23
    ec.print(c);
//    ec.print(isON(REMOTE_CONTROL));//24
    ec.print(c);
//    ec.print(getVconv());//25
    ec.print(c);
}

void HTTPserver() {
    server.begin();
    // listen for incoming clients
    int index=0;  //reset the clientline index
    char clientline[BUFSIZ];  //string that will contain command data

    EthernetClient client = server.available();
    if (client) {
        // logg("new client");
        
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                //        Serial.write(c);
                // if you've gotten to the end of the line (received a newline
                // character) and the line is blank, the http request has ended,
                // so you can send a reply
                if(index<BUFSIZ)  //Only add data if the buffer isn't full.
                {
                    clientline[index]=c;
                    index++;
                }
                
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connnection: close");
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    
                    printState(client);
                    
                    client.println("</html>");
                    break;
                }
                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                }
                else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
                
            }
        }
        
        
        writeStates(clientline);
        
        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
        //logg("client disonnected");
    }
    
}

void writeStates(char clientline[]){
    char * pch;
    char FS[] = "=;";
    char web[] = "web: ";
    
    pch = strtok(clientline, FS);
    while (pch != NULL)
    {
        switch (atoi(pch)){
            case 2:
                pch = strtok (NULL, FS);
                logg("web:STARTER=" + String(atoi(pch)));
//                setStarter(atoi(pch));
                break;
            case 3:
                pch = strtok (NULL, FS);
                logg("web:ONSOLENOID=" + String(atoi(pch)));
//                digitalWrite(onSolenoidPin,atoi(pch));
                break;
            case 4:
                pch = strtok (NULL, FS);
                logg("web:OFFSOLENOID=" + String(atoi(pch)));
//                digitalWrite(offSolenoidPin,atoi(pch));
                break;
            case 5:
                pch = strtok (NULL, FS);
                logg("web:MAINS=" + String(atoi(pch)));
//                setMains(atoi(pch));
                break;
            case 14:
                pch = strtok (NULL, FS);
                logg("web:AUXDIS=" + String(atoi(pch)));
//                setState(AUXDISABLE, atoi(pch));
                break;
            case 15:
                pch = strtok (NULL, FS);
                logg("web:FATAL=" + String(atoi(pch)));
//                setState(FATAL, atoi(pch));
                break;
            case 16:
                pch = strtok (NULL, FS);
                logg("web:LASTAUX=" + String(atoi(pch)));
//                setState(LASTAUXSTATE, atoi(pch));
                break;
            case 17:
                pch = strtok (NULL, FS);
                logg("web:AUTO=" + String(atoi(pch)));
//                setState(AUTO, atoi(pch));
                break;
            case 18:
                pch = strtok (NULL, FS);
                // TODO
                // logg("web:TIMEOUTS=" + String(atoi(pch)));
                break;
            case 19:
                pch = strtok (NULL, FS);
                logg("web:SECS_TOT=" + String(atoi(pch)));
//                setTotalRunSecs(long(atoi(pch)));
                break;
            case 21:
                pch = strtok (NULL, FS);
                logg("web:GENON=" + String(atoi(pch)));
//                setGENON(atoi(pch));
                //        GENON = atoi(pch);
                break;
            case 22:
                pch = strtok (NULL, FS);
                logg("web:GENOFF=" + String(atoi(pch)));
//                setGENOFF(atoi(pch));
                //        GENOFF = atoi(pch);
                break;
            case 23:
                pch = strtok (NULL, FS);
                logg("web:REM_EN=" + String(atoi(pch)));
//                setState(REMOTE_ENABLE, atoi(pch));
                break;
            case 24:
                pch = strtok (NULL, FS);
                logg("web:REM_CTRL=" + String(atoi(pch)));
//                setState(REMOTE_CONTROL, atoi(pch));
                break;
            case 25:
                pch = strtok (NULL, FS);
                logg("web:VBATT=" + String(atoi(pch)));
//                setBatt(atoi(pch));
                break;
        }
        pch = strtok (NULL, FS);
    }
    
}


