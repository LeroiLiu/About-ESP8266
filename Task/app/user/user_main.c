/*
 * user_main.c
 *
 *  Created on: 2015年7月13日
 *      Author: Administrator
 */
#include "user_main.h"
void hw_test_timer_cb(void){
	system_os_post(0,0,0);

}
void test_task (os_event_t *e) {
switch (e->sig) {
case 0:

	os_printf("温度为:%d\r\n",ds18b20ReadTemp());

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

		wifi_get_ip_info(STATION_IF,&info);
		server_init(&info.ip,1213);

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
    wifi_station_set_config(&stationConf);
    wifi_station_connect();
    os_timer_setfn(&connect_timer,WiFi_connect,NULL);
    os_timer_arm(&connect_timer,2000,NULL);
}

void user_init(){
	os_event_t *testQueue;
	uart_init(115200,115200);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,FUNC_GPIO5);
	testQueue=(os_event_t *)os_malloc(sizeof(os_event_t)*4);
	system_os_task(test_task,0,testQueue,4);
	hw_timer_init(FRC1_SOURCE, 1);
	hw_timer_set_func(hw_test_timer_cb);
	hw_timer_arm(1000000);
	system_init_done_cb(to_scan);
}
void user_rf_pre_init(){}

