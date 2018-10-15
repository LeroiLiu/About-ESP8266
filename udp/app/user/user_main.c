/*
 * user_main.c
 *
 *  Created on: 2018年3月23日
 *      Author: Leroi
 */

#include "driver/uart.h"
#include "user_main.h"

void ICACHE_FLASH_ATTR user_udp_send(void){
	char hwaddr[6];
	char DeviceBuffer[40]={0};
	wifi_get_macaddr(STATION_IF,hwaddr);
	os_sprintf(DeviceBuffer,"设备Mac地址为"MACSTR"!!\r\n",MAC2STR(hwaddr));
	espconn_sent(&user_udp_espconn,DeviceBuffer,os_strlen(DeviceBuffer));
}

void ICACHE_FLASH_ATTR user_udp_sent_cb(void *arg){
	os_printf("发送成功！！！");
	os_timer_disarm(&udp_sent_timer);
	os_timer_setfn(&udp_sent_timer,user_udp_send,NULL);
	os_timer_arm(&udp_sent_timer,1000,NULL);
}

void ICACHE_FLASH_ATTR user_udp_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("udp已经接收数据：%s",pdata);
}

void ICACHE_FLASH_ATTR WiFi_connect(void *arg){
	static uint8 count=0;
		   uint8 status;
	os_timer_disarm(&connect_timer);
	count++;
	status = wifi_station_get_connect_status();
	if(status==STATION_GOT_IP){
		os_printf("WiFi 连接成功");
		wifi_set_broadcast_if(STATION_MODE);
		user_udp_espconn.type=ESPCONN_UDP;
		user_udp_espconn.proto.udp=(esp_udp *)os_zalloc(sizeof(esp_udp));
		user_udp_espconn.proto.udp->local_port=2525;
		user_udp_espconn.proto.udp->remote_port=1112;
		const char udp_remote_ip[4]={255,255,255,255};
		os_memcpy(user_udp_espconn.proto.udp->remote_ip,udp_remote_ip,4);
		espconn_regist_recvcb(&user_udp_espconn,user_udp_recv_cb);
		espconn_regist_sentcb(&user_udp_espconn,user_udp_sent_cb);
		espconn_create(&user_udp_espconn);
		user_udp_send();
		return;
	}else{
		if(count>=7){
			os_printf("WiFi 连接失败");
			return;
		}
	}
	os_timer_arm(&connect_timer,2000,NULL);
}

// wifi_station_scan的回调函数
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

	    os_memcpy(&stationConf.ssid, "Leroi", 32);
	    os_memcpy(&stationConf.password, "735934542", 64);
	    wifi_station_set_config_current (&stationConf);
	    wifi_station_connect();
	    os_timer_setfn(&connect_timer,WiFi_connect,NULL);
	    os_timer_arm(&connect_timer,2000,NULL);
	  }
	  else
	  {
	//  	os_sprintf(temp,"err, scan status %d\r\n", status);
	//  	uart0_sendStr(temp);
	  }

}

void to_scan(void)  { wifi_station_scan(NULL,scan_done);}

void user_init(){
	struct softap_config config;
	uart_init(115200,115200);
	wifi_set_opmode_current(0x03);
	wifi_softap_get_config_default(&config);
	os_memcpy(config.ssid,"ESP8266_Leroi",strlen("ESP8266_Leroi"));
	os_memcpy(config.password,"123456789",strlen("123456789"));
	config.ssid_len = strlen("ESP8266_Leroi");
	wifi_softap_set_config_current(&config);
	system_init_done_cb(to_scan);
}

void user_rf_pre_init(){}
