#ifndef GLOBAL_H
#define GLOBAL_H

struct strConfig {
  String ssid;
  String password;
  byte  StaticIP[4];//静态IP
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

  IPAddress staticIP(config.StaticIP[0],config.StaticIP[1],config.StaticIP[2],config.StaticIP[3]);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  WiFi.config(staticIP,gateway,subnet);

  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

boolean ConfigESP(String comedata)
{
     char maxRange[32][32];
     char staticIp[32][32];
     int maxRanges = split(maxRange,string2char(comedata),":");
     int staticips = split(staticIp,maxRange[2],".");
     if(maxRanges==3){
        config.ssid = maxRange[0];
        config.password = maxRange[1];
        config.StaticIP[0] = atol(staticIp[0]);
        config.StaticIP[1] = atol(staticIp[1]);
        config.StaticIP[2] = atol(staticIp[2]);
        config.StaticIP[3] = atol(staticIp[3]);
        ConfigureWifi();
        return true;
     }
     return false;
}



#endif
