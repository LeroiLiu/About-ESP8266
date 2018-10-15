/*
 * wifi.c
 *
 *  Created on: 2018��4��8��
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
		os_printf("WiFi ���ӳɹ�");

		leroi_tcp_client_init();

		return;
	}else{
		if(count>=7){
			os_printf("WiFi ����ʧ��");
			os_timer_disarm(&connect_timer);
			return;
		}
	}
	os_timer_arm(&connect_timer,2000,NULL);
}


//�Ƿ���Station���ã��еĻ�������wifi
void ICACHE_FLASH_ATTR wifi_init(){
	char station[256];
	spi_flash_read(0x3b800,(uint32 *)station,256);
	if(strstr(station,"sta_name")&&strstr(station,"sta_password")){
		os_printf("\r\n station:%s \r\n",station);
		struct station_config stationConf;
		char *sta_name = leroi_getStr(station,"sta_name=");
		os_printf("\r\n sta_name:%s \r\n",sta_name);
		os_memcpy(&stationConf.ssid, sta_name, 32);
		spi_flash_read(0x3b800,(uint32 *)station,256);
		os_printf("\r\n station:%s \r\n",station);
		char *sta_password = leroi_getStr(station,"sta_password=");
		os_printf("\r\n sta_password:%s \r\n",sta_password);
		os_memcpy(&stationConf.password, sta_password, 64);
		wifi_station_set_config(&stationConf);
		wifi_station_connect();
		os_timer_setfn(&connect_timer,WiFi_connect,NULL);
		os_timer_arm(&connect_timer,2000,NULL);
	}
}
