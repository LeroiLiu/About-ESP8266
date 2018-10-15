/*
 * user_main.c
 *
 *  Created on: 2018年3月23日
 *      Author: Leroi
 */

#include "driver/uart.h"
#include "user_main.h"

void ICACHE_FLASH_ATTR check_station(void *arg){
	struct ip_info ap_ip;
	os_timer_disarm(&station_check);
	struct station_info * station = wifi_softap_get_station_info();
	if(station){
		wifi_get_ip_info(SOFTAP_IF,&ap_ip);
		my_station_init(&station->ip,&ap_ip.ip,1025);
		os_free(station);
	}else{
		os_timer_arm(&station_check,2000,NULL);
	}
}

void ICACHE_FLASH_ATTR wifi_handle_event_cb(System_Event_t *evt){
	switch (evt->event) {
		case EVENT_SOFTAPMODE_STACONNECTED:
			os_timer_disarm(&station_check);
			os_timer_setfn(&station_check,check_station,NULL);
			os_timer_arm(&station_check,2000,NULL);
			break;
		default:
			break;
	}
}

void ICACHE_FLASH_ATTR WiFi_connect(void *arg){
	static uint8 count=0;
		   uint8 status;
	os_timer_disarm(&connect_timer);
	count++;
	status = wifi_station_get_connect_status();
	if(status==STATION_GOT_IP){
		os_printf("WiFi 连接成功");
		struct ip_info info;
		const char remote_ip[4]={10,0,0,8};
		wifi_get_ip_info(STATION_IF,&info);
		my_station_init((struct ip_addr *)remote_ip,&info.ip,1234);
		return;
	}else{
		if(count>=7){
			os_printf("WiFi 连接失败");
			return;
		}
	}
	os_timer_arm(&connect_timer,2000,NULL);
}

void to_scan(void)  {
    struct station_config stationConf;
    os_memcpy(&stationConf.ssid, "SANLONG-2.4G", 32);
    os_memcpy(&stationConf.password, "deng123456", 64);
    wifi_station_set_config_current (&stationConf);
    wifi_station_connect();
    os_timer_setfn(&connect_timer,WiFi_connect,NULL);
    os_timer_arm(&connect_timer,2000,NULL);
}

void user_init(){
	struct softap_config config;
	struct ip_info ap_ip;
	uart_init(115200,115200);
	wifi_set_opmode(0x03);
	system_init_done_cb(to_scan);
}

void user_rf_pre_init(){}
