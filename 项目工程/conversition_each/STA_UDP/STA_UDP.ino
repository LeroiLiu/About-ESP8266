#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

WiFiUDP Udp;
String comedata="";
unsigned int localUdpPort = 8899;
char incomingPacket[255];

const char* ssid = "Test";
const char* password = "12345678";

IPAddress staticIP(192,168,0,2);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
IPAddress remote_IP(192,168,0,1);

void setup(void)
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  WiFi.config(staticIP, gateway, subnet); 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

//char buffer [19];

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

//  if (Serial.available()>0)
//  {
//    Serial.readBytes(buffer,8);
//    Udp.beginPacket(WiFi.gatewayIP().toString().c_str(),9502);
//    Serial.print(buffer);
//    Udp.write(buffer);
//    Udp.endPacket();
 // }
 // delay(1);

  //if (Serial.available()>0)
  //{
    //Udp.beginPacket(WiFi.gatewayIP().toString().c_str(),9502);
    //Serial.readBytes(buffer,19);
    //Serial.print(buffer);
    //Udp.write(buffer);
    //Udp.endPacket();
  //}
}
