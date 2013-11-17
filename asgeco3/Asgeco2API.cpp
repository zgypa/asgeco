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
#include <EEPROM.h>
#include <avr/wdt.h>
#include "LocalLibrary.h"
#include "Asgeco2API.h"
#include "GeneratorLibrary.h"


EthernetServer server(HTTPPORT);
byte mac_addr[6]={ MAC1, MAC2, MAC3, MAC4, MAC5, MAC6 };
byte ip_addr[4] ={ IPADDR1,IPADDR2,IPADDR3,IPADDR4 };
byte ip_mask[4] ={ IPMASK1,IPMASK2,IPMASK3,IPMASK4 };
byte ip_gw[4]   ={ GW1, GW2, GW3, GW4 };
byte ip_dns[4]  ={ DNS1, DNS2, DNS3, DNS4 };

// Code
void configureNetwork() {
    // Reads IP address from EEPROM as stored by `WriteNetworkSettings` sketch.
    
    if ((EEPROM.read(EEPROM_SIG_1_OFFSET) == EEPROM_SIG_1_VALUE)
        && (EEPROM.read(EEPROM_SIG_2_OFFSET) == EEPROM_SIG_2_VALUE)) {
        for (int i=0; i<4; i++) {
            ip_addr[i] = EEPROM.read(EEPROM_IP_OFFSET+i);
            ip_mask[i]    = EEPROM.read(EEPROM_MASK_OFFSET+i);
            ip_gw[i]   = EEPROM.read(EEPROM_GATEWAY_OFFSET+i);
        }
        for (int i=0; i<6; i++) {
            mac_addr[i] = EEPROM.read(EEPROM_MAC_OFFSET+i);
        }
    }
}

void setUpAPI(){
    configureNetwork();
    if(DHCP == 0)
        Ethernet.begin(
                       mac_addr,
                       ip_addr,
                       ip_dns,
                       ip_gw,
                       ip_mask);
    else if (DHCP == 1)
        Ethernet.begin(mac_addr);
    globals.ethernetSetup = true;
}


/*
 Please refer to
 https://wiki.afm.co/display/CIAP/Asgeco+2.1+firmware+API+%28current%29
 */
void printState(EthernetClient ec) {
    char outstring[120] = "";
    snprintf(outstring, sizeof(outstring),
             "%s %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %lu %i %i %i %i %i %i %i %i %i %i %i %i %i %i ",
             ASGECO,                            //1
             bitRead(PORTD,starterPin),         //2
             bitRead(PORTD,onSolenoidPin),      //3
             bitRead(PORTD,offSolenoidPin),     //4
             bitRead(PORTB,mains1RelayPin-8),   //5
             getState(MANU_REQUEST),            //6
             getBatt(),                         //7
             (int) readVcc(),                   //8
             analogRead(starterCurrentPin),     //9
             getState(ENGINE),                  //10
             getState(WARMINGUP),               //11
             getState(COOLINGDOWN),             //12
             getState(WAITING),                 //13
             getMode(),                         //14
             getState(FATAL),                   //15
             getState(AUTO_REQUEST),            //16
             digitalRead(auxPin),               //17
             getAttempts(),                     //18
             getTotalRunSecs(),                 //19
             getCurrentRunningSeconds(),        //20
             getGENON(),                        //21
             getGENOFF(),                       //22
             getState(VALVE),                   //23
             0,                                 //24
             getVconv(),                        //25
             getOil(),                          //26
             getWarmUpSeconds(),                //27
             getCoolDownSeconds(),              //28
             getMinimumRunMinutes(),            //29
             getState(OFF_LOCK),                //30
             0,                                 //31
             (int) getErrorCode(),              //32
             getState()
             );
    ec.print(outstring);
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
//        logg("WEB:STOP");
    }
    
}

void writeStates(char clientline[]){
    char * pch;
    char FS[] = "=&";
    char web[] = "web: ";
    
    pch = strtok(clientline, FS);
    while (pch != NULL)
    {
        switch (atoi(pch)){
            case API_STARTER:
                pch = strtok (NULL, FS);
                setStarter(atoi(pch));
//                logg("web:STARTER=" + String(atoi(pch)));
                break;
            case API_ONSOLENOID:
                pch = strtok (NULL, FS);
//                logg("web:ONSOLENOID=" + String(atoi(pch)));
                digitalWrite(onSolenoidPin,atoi(pch));
                break;
            case API_OFFSOLENOID:
                pch = strtok (NULL, FS);
//                logg("web:OFFSOLENOID=" + String(atoi(pch)));
                digitalWrite(offSolenoidPin,atoi(pch));
                logg("WEB:OK");
              break;
            case API_MAINS:
                pch = strtok (NULL, FS);
//                logg("web:MAINS=" + String(atoi(pch)));
                setMains(atoi(pch));
                break;
            case API_MODE:
                pch = strtok (NULL, FS);
//                logg("web:MODE=" + String(atoi(pch)));
                setMode(atoi(pch));
                break;
            case API_FATAL:
                pch = strtok (NULL, FS);
//                logg("web:FATAL=" + String(atoi(pch)));
                setState(FATAL, atoi(pch));
                break;
            case API_MAN_REQUEST:
                pch = strtok (NULL, FS);
//                logg("web:MANU_REQ=" + String(atoi(pch)));
                setState(MANU_REQUEST, atoi(pch));
                break;
            case API_TIMEOUTS:
                pch = strtok (NULL, FS);
                // TODO
                // logg("web:TIMEOUTS=" + String(atoi(pch)));
                break;
            case API_SECS_TOT:
                pch = strtok (NULL, FS);
//                logg("web:SECS_TOT=" + String(atoi(pch)));
                setTotalRunSecs(atol(pch));
//                logg("web:SECS_TOT=" + String(getTotalRunSecs()) + " set");
                break;
            case API_GENON:
                pch = strtok (NULL, FS);
//                logg("web:GENON=" + String(atoi(pch)));
                setGENON(atoi(pch));
//                logg("web:GENOFF=" + String(getGENON()) + " set");
                break;
            case API_GENOFF:
                pch = strtok (NULL, FS);
//                logg("web:GENOFF=" + String(atoi(pch)));
                setGENOFF(atoi(pch));
//                logg("web:GENOFF=" + String(getGENOFF()) + " set");
                break;
            case API_VBATT:
                pch = strtok (NULL, FS);
//                logg("web:VBATT=" + String(atoi(pch)));
                setBatt(atoi(pch));
//                logg("web:VBATT=" + String(getVconv()) + " set");
                break;
            case API_WARMINGUP:
                pch = strtok (NULL, FS);
//                logg("web:WARMINGUP=" + String(atoi(pch)));
                setWarmUpSeconds(atoi(pch));
//                logg("web:WARMINGUP=" + String(getWarmUpSeconds()) + " set");
                break;
            case API_COOLINGDOWN:
                pch = strtok (NULL, FS);
//                logg("web:COOLINGDOWN=" + String(atoi(pch)));
                setCoolDownSeconds(atoi(pch));
//                logg("web:COOLINGDOWN=" + String(getCoolDownSeconds()) + " set");
                break;
            case API_MINIMUMRUNTIME:
                pch = strtok (NULL, FS);
                //                logg("web:MINRUNTIME=" + String(atoi(pch)));
                setMinimumRunMinutes(atoi(pch));
                //                logg("web:MINRUNTIME=" + String(getMinimumRunMinutes()) + " set");
                break;
            case API_REBOOT:
                pch = strtok (NULL, FS);
                wdt_disable();
                wdt_enable(WDTO_2S);
                while (1);
                break;
        }
        pch = strtok (NULL, FS);
    }
    
}


