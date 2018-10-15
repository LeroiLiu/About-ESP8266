#ifndef HELPERS_H
#define HELPERS_H

/*
    dst 保存分割好的数据的数组
    str 被分割的字符串
    spl 分割符
 */
int split(char dst[][32], char* str, const char* spl)
{
    int n = 0;
    char *result = NULL;
    result = strtok(str, spl);

    while( result != NULL )
    {
        strcpy(dst[n++], result);
        result = strtok(NULL, spl);
    }
    return n;
}

char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

boolean wifi_connect(String command){
     char dst[32][32];
     int cnt = split(dst,string2char(command),":");
     if(cnt>3&&String(dst[0]).equals("ssid")&&String(dst[2]).equals("password")){
        Serial.printf("Connecting to %s ", dst[1]);
        WiFi.begin(dst[1], dst[3]);
        while (WiFi.status() != WL_CONNECTED)
        {
          delay(500);
          Serial.print(".");
        }
        Serial.println(" connected");
        return true;
     }
     return false;
}

void http_get(String comedata){
  if(comedata==String("QWC")){
        if(WiFi.isConnected()){
          Serial.println("AWC");
        }else{
          Serial.println("AWN");
        }
  }else{
        HTTPClient http;
        http.begin("http://gjs.shanlong.tech/index/index/index?"+String(comedata));
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK)
        {
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

void prinScanResult(int networksFound)
{
  Serial.printf("%d network(s) found\n", networksFound);
  for (int i = 0; i < networksFound; i++)
  {
    Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
  }
}

#endif
