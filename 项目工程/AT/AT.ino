#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include "helpers.h"
#include "global.h"

WiFiUDP Udp;
String comedata="";
String configdata="";

boolean c_wifi_flag = false;
boolean d_wifi_flag = true;
boolean ifconfig = false;

void setup() {
  Serial.begin(115200);
  delay(500);
}

void loop() {
      if(!ifconfig){
        if(Serial.available()>0){
          comedata = Serial.readStringUntil('?');
          char maxRange[32][32];int maxRanges = split(maxRange,string2char(comedata),":");
          if(maxRanges==3){
            configdata = comedata;
            Serial.print(comedata);
          }else{
            if(comedata=="ok"){
              if(ConfigESP(configdata)){
                ifconfig = true;
              }
            }
          }
        }
      }else{
            if(WiFi.isConnected()){
              if(c_wifi_flag==false&&d_wifi_flag == true){
                Serial.print("WTF");
              }
              c_wifi_flag = true;
              d_wifi_flag = false;
              if(Serial.available()>0){
                 comedata = Serial.readStringUntil('?');
                 Udp.beginPacket("192.168.1.1",8899);
                 Udp.write(comedata.c_str());
                 Udp.endPacket();
              }
            }else{
              if(c_wifi_flag==true&&d_wifi_flag == false){
                Serial.print("MD");
              }
              c_wifi_flag = false;
              d_wifi_flag = true;
           }
      }



}
