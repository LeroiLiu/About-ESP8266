#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

WiFiUDP Udp;
String comedata="";

void setup(void)
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Wi-Fi mode set to WIFI_STA %s\n", WiFi.mode(WIFI_STA) ? "" : "Failed!");
  Serial.println(WiFi.beginSmartConfig()?"Success":"Failed");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
}

void loop() {
  if(Serial.available()>0){
     comedata = Serial.readStringUntil('?');
     Udp.beginPacket(WiFi.gatewayIP().toString().c_str(),9502);
     Udp.write(comedata.c_str());
     Udp.endPacket();
  }
}
