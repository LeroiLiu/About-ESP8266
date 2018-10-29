#ifndef GLOBAL_H
#define GLOBAL_H

#include "helpers.h"

struct strConfig {
  String ssid;
  String password;
  char* mqtt_server = "mqtt.shanlong.tech";
  String client_id;
  String target;
  String TOPIC;
}config;
/*
**
** CONFIGURATION HANDLING
**
*/
void ConfigureWifi()
{
  Serial.printf("Connecting to");
  Serial.println(config.ssid.c_str());
  //Serial.println(config.password.c_str());
  WiFi.begin (config.ssid.c_str(), config.password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

boolean ConfigESP(String comedata)
{
     char maxRange[32][32];
     int maxRanges = split(maxRange,string2char(comedata),":");
     if(maxRanges==4){
        config.ssid = maxRange[0];
        config.password = maxRange[1];
        config.client_id = maxRange[2];
        config.target = maxRange[3];
        config.TOPIC = string2char(String("home/")+String("+/")+config.client_id);
        return true;
     }
     return false;
}




#endif
