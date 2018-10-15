/*
 * user_main.c
 *
 *  Created on: 2015年7月13日
 *      Author: Administrator
 */
#include "driver/uart.h"
#include "user_main.h"
void ICACHE_FLASH_ATTR Wifi_conned(void *arg){
	static uint8 count=0;
	uint8 status;

	os_timer_disarm(&connect_timer);
	count++;
	status=wifi_station_get_connect_status();
	if(status==STATION_GOT_IP){
		os_printf("Wifi connect success!");
		HttpReadFile("http://test.pptmodel.cn/iot/post_get.php","GET","d=9");
		//my_station_init((struct ip_addr *)remote_ip,&info.ip,1025);
		//server_init(&info.ip,1213);
		return;
	}else{
	if(count>=7){
		os_printf("Wifi connect fail！");
		return;
	}
}



os_timer_arm(&connect_timer,2000,NULL);
}
void ICACHE_FLASH_ATTR scan_done(void *arg,STATUS status){
	uint8 ssid[33];
	struct station_config stationConf;

	  if (status == OK)
	  {
	    struct bss_info *bss_link = (struct bss_info *)arg;
	    bss_link = bss_link->next.stqe_next;//ignore first

	    while (bss_link != NULL)
	    {
	      os_memset(ssid, 0, 33);
	      if (os_strlen(bss_link->ssid) <= 32)
	      {
	        os_memcpy(ssid, bss_link->ssid, os_strlen(bss_link->ssid));
	      }
	      else
	      {
	        os_memcpy(ssid, bss_link->ssid, 32);
	      }
	      os_printf("+CWLAP:(%d,\"%s\",%d,\""MACSTR"\",%d)\r\n",
	                 bss_link->authmode, ssid, bss_link->rssi,
	                 MAC2STR(bss_link->bssid),bss_link->channel);

	      bss_link = bss_link->next.stqe_next;
	    }
	    os_memcpy(&stationConf.ssid, "SANLONG-2.4G", 32);
	    os_memcpy(&stationConf.password, "deng123456", 64);
	    wifi_station_set_config_current(&stationConf);
	    wifi_station_connect();
	    os_timer_setfn(&connect_timer,Wifi_conned,NULL);
	    os_timer_arm(&connect_timer,2000,NULL);
	  }
	  else
	  {
	//  	os_sprintf(temp,"err, scan status %d\r\n", status);
	//  	uart0_sendStr(temp);

	  }

}
void to_scan(void) { wifi_station_scan(NULL,scan_done); }
void user_init(){
	uart_init(115200,115200);
	//初始化ap
	wifi_set_opmode(0x01);
	system_init_done_cb(to_scan);
}
void user_rf_pre_init(){}

