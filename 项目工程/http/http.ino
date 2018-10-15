#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "SANLONG-2.4G";
const char* password = "deng123456";
String comedata="";

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

void loop()
{
  if(Serial.available()>0){
     comedata = Serial.readStringUntil('?');
      HTTPClient http;
      http.begin("http://gjs.shanlong.tech/index/index/index?"+String(comedata));
      int httpCode = http.GET();
      if(httpCode == HTTP_CODE_OK)
      {
//         Serial.print("HTTP response code ");
//         Serial.println(httpCode);
         String response = http.getString();
         Serial.println(response);
      }
      else
      {
        Serial.println("Error in HTTP request");
      }
      http.end();
  }
}
