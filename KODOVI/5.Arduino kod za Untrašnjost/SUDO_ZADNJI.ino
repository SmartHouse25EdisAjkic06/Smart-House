#include <Bonezegei_DHT11.h>
Bonezegei_DHT11 dht(8);
/*
   -- Suad Mikara --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX 2
#define REMOTEXY_SERIAL_TX 3
#define REMOTEXY_SERIAL_SPEED 9600


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 130 bytes
  { 255,4,0,1,0,123,0,19,0,0,0,85,110,117,116,114,97,115,110,106,
  111,115,116,0,31,2,106,200,200,84,1,1,5,0,2,10,54,51,24,13,
  27,31,15,0,2,26,31,31,79,78,0,79,70,70,0,67,26,8,54,24,
  141,18,33,10,94,2,26,2,5,47,16,36,14,58,31,15,0,2,26,31,
  31,79,78,0,79,70,70,0,2,38,69,16,36,70,58,31,15,0,206,26,
  31,31,79,78,0,79,70,70,0,2,38,140,16,36,69,27,31,15,0,36,
  26,31,31,79,78,0,79,70,70,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t Sijalica; // =1 if switch ON and =0 if OFF
  uint8_t Prozor; // =1 if switch ON and =0 if OFF
  uint8_t Hladjenje; // =1 if switch ON and =0 if OFF
  uint8_t Grijanje; // =1 if switch ON and =0 if OFF

    // output variables
  int8_t Temp; // -128 .. 127

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_SIJALICA 4
#define PIN_PROZOR 5
#define PIN_HLADJENJE 7
#define PIN_GRIJANJE 6


void setup() 
{
  RemoteXY_Init (); 
  
  pinMode (PIN_SIJALICA, OUTPUT);
  pinMode (PIN_PROZOR, OUTPUT);
  pinMode (PIN_HLADJENJE, OUTPUT);
  pinMode (PIN_GRIJANJE, OUTPUT);
    Serial.begin(9600);
  dht.begin();
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  digitalWrite(PIN_SIJALICA, (RemoteXY.Sijalica==0)?LOW:HIGH);
  digitalWrite(PIN_PROZOR, (RemoteXY.Prozor==0)?LOW:HIGH);
  digitalWrite(PIN_HLADJENJE, (RemoteXY.Hladjenje==0)?LOW:HIGH);
  digitalWrite(PIN_GRIJANJE, (RemoteXY.Grijanje==0)?LOW:HIGH);
  if (dht.getData()) {                   
    float tempDeg = dht.getTemperature();      
    Serial.print("Temperatura je :");
    Serial.print(tempDeg);
    Serial.println("°C");
    RemoteXY.Temp = tempDeg;
  }
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay() 


}
