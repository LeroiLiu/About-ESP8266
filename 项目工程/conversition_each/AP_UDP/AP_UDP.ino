#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

IPAddress local_IP(192,168,0,1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
IPAddress remote_IP(192,168,0,2);

const char* ssid = "Test";
const char* password = "12345678";

WiFiUDP Udp;
String comedata="";
unsigned int localUdpPort = 8899;
char incomingPacket[255];

void setup(void)
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid,password) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  
}

void loop() {
  if(Serial.available()>0){
     comedata = Serial.readStringUntil('?');
     Udp.beginPacket(remote_IP,8899);
     Udp.write(comedata.c_str());
     Udp.endPacket();
  }

  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf(incomingPacket);
  }
}
