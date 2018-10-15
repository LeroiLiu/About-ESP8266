#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiUDP.h>

#include "helpers.h"
#include "global.h"

WiFiUDP Udp;
String comedata="";

void setup() {
  EEPROM.begin(512);
  Serial.begin(115200);
  delay(500);
  Serial.println("Starting ES8266");
  if (!ReadConfig())
  {
    // DEFAULT CONFIG
    config.ssid = "SANLONG-2.4G";
    config.password = "deng123456";
    config.StaticIP[0] = 192;config.StaticIP[1] = 168;config.StaticIP[2] = 1;config.StaticIP[3] = 9;
    config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
    config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;
    config.RemoteIP[0] = 192;config.RemoteIP[1] = 168;config.RemoteIP[2] = 1;config.RemoteIP[3] = 1;
    config.RemotePort = 8899;
    config.LocalPort = 8899;
    config.dhcp = false;
    WriteConfig();
    Serial.println("General config applied");
  }
  ConfigureWifi();
}

IPAddress remote_IP(config.RemoteIP[0],config.RemoteIP[1],config.RemoteIP[2],config.RemoteIP[3]);

void loop() {
  if(Serial.available()>0){
     comedata = Serial.readStringUntil('?');
     Serial.println(comedata);
     if(ConfigESP(comedata)){
       Udp.beginPacket(remote_IP,config.LocalPort);
       Udp.write(comedata.c_str());
       Udp.endPacket();
     }
  }
}
