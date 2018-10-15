/*
 * wifi.c
 *
 *  Created on: 2018年4月8日
 *      Author: Leroi
 */
#include "wifi.h"

void ICACHE_FLASH_ATTR WiFi_connect(void *arg){
	static uint8 count=0;
		   uint8 status;
	os_timer_disarm(&connect_timer);
	count++;
	status = wifi_station_get_connect_status();
	if(status==STATION_GOT_IP){
		leroi_tcp_server_init(global_precv);
		os_printf("WiFi 连接成功");
		return;
	}else{
		if(count>=7){
			os_printf("WiFi 连接失败");
			os_timer_disarm(&connect_timer);
			return;
		}
	}
	os_timer_arm(&connect_timer,2000,NULL);
}

bool ICACHE_FLASH_ATTR wifi_init(char *precv){
    char name[1024];strcpy(name,precv);
    char password[1024];strcpy(password,precv);

	char *sta_name =strtok(strstr(name,"sta_name=")+9,"&");
	if(strstr(sta_name,"sta_password")){return false;}
	char *sta_password =strtok(strstr(password,"sta_password=")+13,"&");
	if(strstr(sta_name,"nw_type")){return false;}

	os_printf("\r\n\r\nsta_name:%s\r\nsta_password:%s\r\n\r\n",sta_name,sta_password);

	struct station_config stationConf;
	os_memcpy(&stationConf.ssid, sta_name, 32);
	os_memcpy(&stationConf.password, sta_password, 64);
	wifi_station_set_config(&stationConf);
	wifi_station_connect();
	os_timer_setfn(&connect_timer,WiFi_connect,NULL);
	os_timer_arm(&connect_timer,2000,NULL);

	return true;
}
