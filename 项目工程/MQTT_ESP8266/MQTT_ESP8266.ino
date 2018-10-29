#include "espmqtt.h"
#include "helpers.h"
#include "global.h"

void setup() {
  Serial.begin(115200);
  delay(500);

  String configdata="";
  String comedata="";

  mqttclient.setServer(config.mqtt_server,1883);

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
        if(maxRanges==4){
          configdata = comedata;
          Serial.print(comedata);
        }
      }
    }
  }

  if(WiFi.isConnected()){//wifi连接成功
    Serial.print("WTF");
  }
  
  mqttclient.setServer(config.mqtt_server, 1883);                              //设定MQTT服务器与使用的端口，1883是默认的MQTT端口
  mqttclient.setCallback(callback);                                          //设定回调方式，当ESP8266收到订阅消息时会调用此方法

  while(WiFi.isConnected()){
    if (!mqttclient.connected()) {
      reconnect();
    }
    mqttclient.loop();
    if(Serial.available()>0){
       comedata = Serial.readStringUntil('?');
       mqttclient.publish(string2char(String("home/")+config.client_id+String("/")+config.target), comedata.c_str());
    }
  }

  Serial.print("MD");
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
