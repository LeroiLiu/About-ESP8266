#ifndef ESPMQTT_H
#define ESPMQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "global.h"

WiFiClient espClient;                                                         // 定义wifiClient实例
PubSubClient mqttclient(espClient);                                         // 定义PubSubClient的实例
long lastMsg = 0;                                                               // 记录上一次发送信息的时长

void callback(char* topic, byte* payload, unsigned int length) {
  char maxRange[32][32];
  int maxRanges = split(maxRange,string2char(topic),"/");
  Serial.print(maxRange[1]);   // 打印主题信息
  Serial.print("-");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // 打印主题内容
  }
  Serial.println();
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, HIGH);   // 亮灯
  } else {
    digitalWrite(BUILTIN_LED, LOW);   // 熄灯
  }
}

void reconnect() {
  while (!mqttclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttclient.connect(string2char(config.client_id),"test","test")) {
      Serial.println("connected");
      // 连接成功时订阅主题
      mqttclient.subscribe(string2char(config.TOPIC));
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


#endif
