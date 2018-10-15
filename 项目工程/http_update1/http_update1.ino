#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "helpers.h"

const char* ssid = "SANLONG-2.4G";
const char* password = "deng123456";
String comedata="";

void setup()
{
  Serial.begin(115200);
  Serial.println();
//  WiFi.scanNetworksAsync(prinScanResult);
}

void loop()
{
  if(WiFi.isConnected()){
    if(Serial.available()>0){
       comedata = Serial.readStringUntil('?');
       http_get(comedata);
    }
  }else{
    if(Serial.available()>0){
       comedata = Serial.readStringUntil('?');
       if(!wifi_connect(comedata)){
          Serial.println("Wifi connect Failed!!");
       }
    }
  }
}
