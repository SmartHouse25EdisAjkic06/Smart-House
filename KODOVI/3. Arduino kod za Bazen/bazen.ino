#define REMOTEXY_MODE__WIFI_POINT

#include <ESP8266WiFi.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 76 bytes
  { 255,2,0,1,0,69,0,19,0,0,0,0,31,1,106,200,1,1,3,0,
  2,29,22,44,22,0,1,26,31,31,79,78,0,79,70,70,0,2,29,66,
  44,22,0,12,26,31,31,79,78,0,79,70,70,0,73,37,118,25,54,12,
  128,0,2,26,0,0,0,0,0,0,200,66,0,0,0,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t punjenje; // =1 if switch ON and =0 if OFF
  uint8_t praznjenje; // =1 if switch ON and =0 if OFF

    // output variables
  int8_t linearbar_01; // from 0 to 100

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_PUNJENJE 0
#define PIN_PRAZNJENJE 4


void setup() 
{
  RemoteXY_Init (); 
  Serial.begin(115200);
  pinMode (PIN_PUNJENJE, OUTPUT);
  pinMode (PIN_PRAZNJENJE, OUTPUT);
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  digitalWrite(PIN_PUNJENJE, (RemoteXY.punjenje==0)?LOW:HIGH);
  digitalWrite(PIN_PRAZNJENJE, (RemoteXY.praznjenje==0)?LOW:HIGH);
  int senzor_vrijednost = analogRead(A0);
  int nivo=0;
  Serial.println(senzor_vrijednost);
    if (senzor_vrijednost>=0 && senzor_vrijednost<25) nivo = 0; 
    if (senzor_vrijednost>=25 && senzor_vrijednost<72) nivo = 15;
    if (senzor_vrijednost>=73 && senzor_vrijednost<102) nivo = 30;
    if (senzor_vrijednost>=102 && senzor_vrijednost<116) nivo = 45;
    if (senzor_vrijednost>=116 && senzor_vrijednost<122) nivo = 60;
    if (senzor_vrijednost>=122 && senzor_vrijednost<136) nivo = 75;
    if (senzor_vrijednost>=136 && senzor_vrijednost<148) nivo = 85;
    if (senzor_vrijednost>=148 && senzor_vrijednost<163) nivo = 95;
    if (senzor_vrijednost>=163) nivo = 100;
  RemoteXY.linearbar_01 = nivo; 
  RemoteXY_delay(2000);
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay() 
}
