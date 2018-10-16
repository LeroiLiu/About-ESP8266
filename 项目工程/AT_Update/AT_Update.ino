#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include "helpers.h"
#include "global.h"

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFiUDP Udp;
  String configdata="";
  String comedata="";

  while(1){//配置wifi
    if(Serial.available()>0){
      comedata = Serial.readStringUntil('?');
      if(comedata=="ok"){//如果接收到了ok则说明configdata配置的值是正确的
        if(ConfigESP(configdata)){//存储wifi配置
          ConfigureWifi();//配置正确则连接wifi
          break;
        }//若没有配置正确则继续接收configdata
      }else{//一直配置configdata
        char maxRange[32][32];int maxRanges = split(maxRange,string2char(comedata),":");
        if(maxRanges==3){
          configdata = comedata;
          Serial.print(comedata);
        }
      }
    }
  }

  if(WiFi.isConnected()){//wifi连接成功
    Serial.print("WTF");
  }

  while(WiFi.isConnected()){
    if(Serial.available()>0){
       comedata = Serial.readStringUntil('?');
       Udp.beginPacket("192.168.1.1",8899);
       Udp.write(comedata.c_str());
       Udp.endPacket();
    }
  }

  Serial.print("MD");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
