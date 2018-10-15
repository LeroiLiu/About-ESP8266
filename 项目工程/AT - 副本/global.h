#ifndef GLOBAL_H
#define GLOBAL_H

struct strConfig {
  String ssid;
  String password;
  byte  StaticIP[4];//静态IP
  byte  Netmask[4];//子网掩码
  byte  Gateway[4];//网关IP
  byte  RemoteIP[4];//远程IP
  byte  RemotePort;//远程端口
  byte  LocalPort;//本地IP
  boolean  dhcp;//是否开启静态IP
}config;

/*
**
** CONFIGURATION HANDLING
**
*/
void ConfigureWifi()
{
  Serial.println("Configuring Wifi");
  Serial.println(config.ssid.c_str());
  Serial.println(config.password.c_str());
  WiFi.begin (config.ssid.c_str(), config.password.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(5);
    Serial.print(".");
  }
  if(config.dhcp){
    IPAddress staticIP(config.StaticIP[0],config.StaticIP[1],config.StaticIP[2],config.StaticIP[3]);
    IPAddress gateway(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3]);
    IPAddress subnet(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3]);
    Serial.println("Configuring Static IP");
    Serial.println(staticIP);
    Serial.println(gateway);
    WiFi.config(staticIP,gateway,subnet);
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void ChangeSSIDandPassword(String comedata){
     Serial.print("ATTEMPT TO MODIFY THE SSID AND PASSWORD");
     Serial.println();
     char dst[32][32];
     int cnt = split(dst,string2char(comedata),":");
     WriteStringToEEPROM(32,dst[1]);
     if(cnt>3){
        WriteStringToEEPROM(64,dst[3]);
        Serial.print("The AP has modified as:");Serial.print(dst[1]);
        Serial.print("The AP password has modified as:");Serial.print(dst[3]);
     }else{
        WriteStringToEEPROM(64,"");
        Serial.print("The AP has modified as:");Serial.print(dst[1]);
        Serial.print("The AP password has modified as none");
     }
     EEPROM.end();
     Serial.println();
     Serial.print("Modified Done,You Can Reboot to Continue Configure,Please ATTENTION !!!Make Sure Your configure Right Or you'll can't process the project");
}

void ChangeDHCP(String comedata){
     Serial.print("ATTEMPT TO MODIFY THE DHCP");
     Serial.println();
     char dst[32][32];
     int cnt = split(dst,string2char(comedata),":");
     if(String(dst[1]).equals("1")){
        Serial.print("DHCP OPEN");
        EEPROM.write(21,true);
     }else if(String(dst[1]).equals("0")){
        Serial.print("DHCP CLOSE");
        EEPROM.write(21,false);
     }else{
        Serial.print("DHCP HASN'T MODIFIED");
     }
     EEPROM.end();
     Serial.println();
     Serial.print("Modified Done,You Can Reboot to Continue Configure,Please ATTENTION !!!Make Sure Your configure Right Or you'll can't process the project");
     Serial.println();
}

void ChangeStaticIP(String comedata){
     Serial.print("ATTEMPT TO MODIFY THE STATIC IP");
     Serial.println();
     char dst[32][32];
     int cnt = split(dst,string2char(comedata),":");
     EEPROM.write(3,atol(dst[1]));
     EEPROM.write(4,atol(dst[2]));
     EEPROM.write(5,atol(dst[3]));
     EEPROM.write(6,atol(dst[4]));
     EEPROM.end();
     Serial.print("Static IP has modified as");
     Serial.print(String(dst[1]));Serial.print(".");Serial.print(String(dst[2]));Serial.print(".");Serial.print(String(dst[3]));Serial.print(".");Serial.print(String(dst[4]));
     Serial.println();
     Serial.print("Modified Done,You Can Reboot to Continue Configure,Please ATTENTION !!!Make Sure Your configure Right Or you'll can't process the project");
     Serial.println();
}
void ChangeNetmask(String comedata){
     Serial.print("ATTEMPT TO MODIFY THE STATIC IP");
     Serial.println();
     char dst[32][32];
     int cnt = split(dst,string2char(comedata),":");
     EEPROM.write(7,atol(dst[1]));
     EEPROM.write(8,atol(dst[2]));
     EEPROM.write(9,atol(dst[3]));
     EEPROM.write(10,atol(dst[4]));
     EEPROM.end();
     Serial.print("Netmask has modified as");
     Serial.print(String(dst[1]));Serial.print(".");Serial.print(String(dst[2]));Serial.print(".");Serial.print(String(dst[3]));Serial.print(".");Serial.print(String(dst[4]));
     Serial.println();
     Serial.print("Modified Done,You Can Reboot to Continue Configure,Please ATTENTION !!!Make Sure Your configure Right Or you'll can't process the project");
     Serial.println();
}
void ChangeGateway(String comedata){
     Serial.print("ATTEMPT TO MODIFY THE GATEWAY");
     Serial.println();
     char dst[32][32];
     int cnt = split(dst,string2char(comedata),":");
     EEPROM.write(11,atol(dst[1]));
     EEPROM.write(12,atol(dst[2]));
     EEPROM.write(13,atol(dst[3]));
     EEPROM.write(14,atol(dst[4]));
     EEPROM.end();
     Serial.print("Gateway has modified as");
     Serial.print(String(dst[1]));Serial.print(".");Serial.print(String(dst[2]));Serial.print(".");Serial.print(String(dst[3]));Serial.print(".");Serial.print(String(dst[4]));
     Serial.println();
     Serial.print("Modified Done,You Can Reboot to Continue Configure,Please ATTENTION !!!Make Sure Your configure Right Or you'll can't process the project");
     Serial.println();
}

void ChangeRemoteIP(String comedata){
     Serial.print("ATTEMPT TO MODIFY THE REMOTE IP");
     Serial.println();
     char dst[32][32];
     int cnt = split(dst,string2char(comedata),":");
     EEPROM.write(15,atol(dst[1]));
     EEPROM.write(16,atol(dst[2]));
     EEPROM.write(17,atol(dst[3]));
     EEPROM.write(18,atol(dst[4]));
     EEPROM.end();
     Serial.print("Remote IP has modified as");
     Serial.print(String(dst[1]));Serial.print(".");Serial.print(String(dst[2]));Serial.print(".");Serial.print(String(dst[3]));Serial.print(".");Serial.print(String(dst[4]));
     Serial.println();
     Serial.print("Modified Done,You Can Reboot to Continue Configure,Please ATTENTION !!!Make Sure Your configure Right Or you'll can't process the project");
     Serial.println();
}

void ChangeRemotePort(String comedata){
     Serial.print("ATTEMPT TO MODIFY THE REMOTE PORT");
     Serial.println();
     char dst[32][32];
     int cnt = split(dst,string2char(comedata),":");
     EEPROM.write(19,atol(dst[1]));
     EEPROM.end();
     Serial.print("Remote Port has modified as:");Serial.print(String(dst[1]));
     Serial.println();
     Serial.print("Modified Done,You Can Reboot to Continue Configure,Please ATTENTION !!!Make Sure Your configure Right Or you'll can't process the project");
     Serial.println();
}

void ChangeLocalPort(String comedata){
     Serial.print("ATTEMPT TO MODIFY THE Local PORT");
     Serial.println();
     char dst[32][32];
     int cnt = split(dst,string2char(comedata),":");
     EEPROM.write(20,atol(dst[1]));
     EEPROM.end();
     Serial.print("Local Port has modified as:");Serial.print(String(dst[1]));
     Serial.println();
     Serial.print("Modified Done,You Can Reboot to Continue Configure,Please ATTENTION !!!Make Sure Your configure Right Or you'll can't process the project");
     Serial.println();
}

boolean ConfigESP(String comedata)
{
     char dst[32][32];
     int cnt = split(dst,string2char(comedata),":");
     if(cnt>1){
        if(String(dst[0]).equals("ssid")){
            ChangeSSIDandPassword(comedata);
        }else if(String(dst[0]).equals("dhcp")){
            ChangeDHCP(comedata);
        }else if(String(dst[0]).equals("staticip")){
            ChangeStaticIP(comedata);
        }else if(String(dst[0]).equals("gateway")){
            ChangeGateway(comedata);
        }else if(String(dst[0]).equals("remoteport")){
            ChangeRemotePort(comedata);
        }else if(String(dst[0]).equals("localport")){
            ChangeLocalPort(comedata);
        }
        return false;
     }
     return true;
}

void WriteConfig()
{
  Serial.println("Writing Config");
  EEPROM.write(0,'C');
  EEPROM.write(1,'F');
  EEPROM.write(2,'G');
  EEPROM.write(3,config.StaticIP[0]);
  EEPROM.write(4,config.StaticIP[1]);
  EEPROM.write(5,config.StaticIP[2]);
  EEPROM.write(6,config.StaticIP[3]);
  EEPROM.write(7,config.Netmask[0]);
  EEPROM.write(8,config.Netmask[1]);
  EEPROM.write(9,config.Netmask[2]);
  EEPROM.write(10,config.Netmask[3]);
  EEPROM.write(11,config.Gateway[0]);
  EEPROM.write(12,config.Gateway[1]);
  EEPROM.write(13,config.Gateway[2]);
  EEPROM.write(14,config.Gateway[3]); 
  EEPROM.write(15,config.RemoteIP[0]);
  EEPROM.write(16,config.RemoteIP[1]);
  EEPROM.write(17,config.RemoteIP[2]);
  EEPROM.write(18,config.RemoteIP[3]);  
  EEPROM.write(19,config.RemotePort);
  EEPROM.write(20,config.LocalPort);
  EEPROM.write(21,config.dhcp);
  WriteStringToEEPROM(32,config.ssid);
  WriteStringToEEPROM(64,config.password);
  EEPROM.commit();
}

boolean ReadConfig()
{
  Serial.println("Reading Configuration");
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
  {
    Serial.println("Configurarion Found!");
    config.StaticIP[0] = EEPROM.read(3);
    config.StaticIP[1] = EEPROM.read(4);
    config.StaticIP[2] = EEPROM.read(5);
    config.StaticIP[3] = EEPROM.read(6);
    config.Netmask[0] = EEPROM.read(7);
    config.Netmask[1] = EEPROM.read(8);
    config.Netmask[2] = EEPROM.read(9);
    config.Netmask[3] = EEPROM.read(10);
    config.Gateway[0] = EEPROM.read(11);
    config.Gateway[1] = EEPROM.read(12);
    config.Gateway[2] = EEPROM.read(13);
    config.Gateway[3] = EEPROM.read(14);    
    config.RemoteIP[0] = EEPROM.read(15);
    config.RemoteIP[1] = EEPROM.read(16);
    config.RemoteIP[2] = EEPROM.read(17);
    config.RemoteIP[3] = EEPROM.read(18);    
    config.RemotePort = EEPROM.read(19);
    config.LocalPort = EEPROM.read(20);
    config.dhcp = EEPROM.read(21);
    config.ssid = ReadStringFromEEPROM(32);
    config.password = ReadStringFromEEPROM(64);
    return true;
  }
  else
  {
    Serial.println("Configurarion NOT FOUND!!!!");
    return false;
  }
}

#endif
