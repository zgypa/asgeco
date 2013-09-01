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
#include "LocalLibrary.h"
#include "Asgeco2API.h"
#include "GeneratorLibrary.h"

IPAddress ip(IPADDR1,IPADDR2,IPADDR3,IPADDR4);
EthernetServer server(HTTPPORT);

// Code
void setUpAPI(){
    byte mac_addr[6] = { MAC1, MAC2, MAC3, MAC4, MAC5, MAC6 };

    if(DHCP == 0)
        Ethernet.begin(mac_addr, ip);
    else if (DHCP == 1)
        Ethernet.begin(mac_addr);
}


/*
 Please refer to 
 https://wiki.afm.co/display/CIAP/Asgeco+2.1+firmware+API+%28current%29
 */
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
    ec.print(getState(MANU_REQUEST));//6
    ec.print(c);
    ec.print(getBatt());//7
    ec.print(c);
    ec.print(readVcc());//8
    ec.print(c);
    ec.print(analogRead(starterCurrentPin)) ;//9
    ec.print(c);
    ec.print(getState(ENGINE));//10
    ec.print(c);
    ec.print(getState(WARMINGUP));//11
    ec.print(c);
    ec.print(getState(COOLINGDOWN));//12
    ec.print(c);
    ec.print(getState(WAITING));//13
    ec.print(c);
    ec.print(getState(MODE));//14
    ec.print(c);
    ec.print(getState(FATAL));//15
    ec.print(c);
    ec.print(getState(AUTO_REQUEST));//16
    ec.print(c);
    ec.print(digitalRead(auxPin), DEC);//17
    ec.print(c);
    ec.print(getAttempts());//18
    ec.print(c);
    ec.print(getTotalRunSecs(),DEC);//19
    ec.print(c);
    if(getState(ENGINE))
        ec.print((millis() - getEngineStartTime())/1000);//20
    else
        ec.print(0);
    ec.print(c);
    ec.print(getGENON());//21
    ec.print(c);
    ec.print(getGENOFF());//22
    ec.print(c);
    ec.print(getState(VALVE));//23
    ec.print(c);
    ec.print(0);//24
    ec.print(c);
    ec.print(getVconv());//25
    ec.print(c);
    ec.print(getOil());//26
    ec.print(c);
    ec.print(getWarmingUp());//27
    ec.print(c);
    ec.print(getCoolingDown());//28
    ec.print(c); // this last space is necessary.
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
    char FS[] = "=&";
    char web[] = "web: ";
    
    pch = strtok(clientline, FS);
    while (pch != NULL)
    {
        switch (atoi(pch)){
            case API_STARTER:
                pch = strtok (NULL, FS);
                logg("web:STARTER=" + String(atoi(pch)));
                setStarter(atoi(pch));
                break;
            case API_ONSOLENOID:
                pch = strtok (NULL, FS);
                logg("web:ONSOLENOID=" + String(atoi(pch)));
                digitalWrite(onSolenoidPin,atoi(pch));
                break;
            case API_OFFSOLENOID:
                pch = strtok (NULL, FS);
                logg("web:OFFSOLENOID=" + String(atoi(pch)));
                digitalWrite(offSolenoidPin,atoi(pch));
                break;
            case API_MAINS:
                pch = strtok (NULL, FS);
                logg("web:MAINS=" + String(atoi(pch)));
                setMains(atoi(pch));
                break;
            case API_MODE:
                pch = strtok (NULL, FS);
                logg("web:MODE=" + String(atoi(pch)));
                setState(MODE, atoi(pch));
                break;
            case API_FATAL:
                pch = strtok (NULL, FS);
                logg("web:FATAL=" + String(atoi(pch)));
                setState(FATAL, atoi(pch));
                break;
            case API_MAN_REQUEST:
                pch = strtok (NULL, FS);
                logg("web:MANU_REQ=" + String(atoi(pch)));
                setState(MANU_REQUEST, atoi(pch));
                break;
            case API_TIMEOUTS:
                pch = strtok (NULL, FS);
                // TODO
                // logg("web:TIMEOUTS=" + String(atoi(pch)));
                break;
            case API_SECS_TOT:
                pch = strtok (NULL, FS);
                logg("web:SECS_TOT=" + String(atoi(pch)));
                setTotalRunSecs(long(atoi(pch)));
                break;
            case API_GENON:
                pch = strtok (NULL, FS);
                logg("web:GENON=" + String(atoi(pch)));
                setGENON(atoi(pch));
                break;
            case API_GENOFF:
                pch = strtok (NULL, FS);
                logg("web:GENOFF=" + String(atoi(pch)));
                setGENOFF(atoi(pch));
                break;
            case API_VBATT:
                pch = strtok (NULL, FS);
                logg("web:VBATT=" + String(atoi(pch)));
                setBatt(atoi(pch));
                break;
            case API_WARMINGUP:
                pch = strtok (NULL, FS);
                logg("web:WARMINGUP=" + String(atoi(pch)));
                setWarmingUp(atoi(pch));
                break;
            case API_COOLINGDOWN:
                pch = strtok (NULL, FS);
                logg("web:COOLINGDOWN=" + String(atoi(pch)));
                setCoolingDown(atoi(pch));
                break;
        }
        pch = strtok (NULL, FS);
    }
    
}


