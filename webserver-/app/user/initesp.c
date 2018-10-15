/*
 * initesp.c
 *
 *  Created on: 2018年4月8日
 *      Author: Leroi
 */


#include "initesp.h"


bool ICACHE_FLASH_ATTR leroi_tcp_server_init(){
	os_printf("\r\n start \r\n");
	char tcp_server[256];
	spi_flash_read(0x3b000,(uint32 *)tcp_server,256);
	if(strstr(tcp_server,"Client")){
		char *local_port = leroi_getStr(tcp_server,"local_port=");
		if(strncmp(local_port,"err",3)!=0){
			leroi_server_init(SOFTAP_IF,atoi(local_port));
		}
	}

	return true;
}

//设置AP信息
bool ICACHE_FLASH_ATTR leroi_set_softap_init(){

	char softap_info[1024];
	spi_flash_read(0x3bc00,(uint32 *)softap_info,1024);
	if(strstr(softap_info,"submit")){
		struct softap_config config;
		wifi_softap_get_config_default(&config);

		spi_flash_read(0x3bc00,(uint32 *)softap_info,1024);
		char *ap_name = leroi_getStr(softap_info,"ap_name=");
		os_memcpy(config.ssid,(uint8 *)ap_name,os_strlen(ap_name));
		config.ssid_len = os_strlen(ap_name);

		spi_flash_read(0x3bc00,(uint32 *)softap_info,1024);
		char *ap_password = leroi_getStr(softap_info,"ap_password=");
		os_memcpy(config.password,(uint8 *)ap_password,os_strlen(ap_password));

		spi_flash_read(0x3bc00,(uint32 *)softap_info,1024);
		char *ap_encrypt = leroi_getStr(softap_info,"ap_encrypt=");

		uint8 ap_auth;
			if(strstr(ap_encrypt,"AUTH_OPEN")){
				ap_auth = AUTH_OPEN;
			}else if(strstr(ap_encrypt,"AUTH_WEP")){
				ap_auth = AUTH_WEP;
			}else if(strstr(ap_encrypt,"AUTH_WPA_PSK")){
				ap_auth = AUTH_WPA_PSK;
			}else if(strstr(ap_encrypt,"AUTH_WPA2_PSK")){
				ap_auth = AUTH_WPA2_PSK;
			}else if(strstr(ap_encrypt,"AUTH_WPA_WPA2_PSK")){
				ap_auth = AUTH_WPA_WPA2_PSK;
			}
			config.authmode = ap_auth;

		wifi_softap_set_config(&config);

		return true;

	}else{
		return false;
	}

}

bool ICACHE_FLASH_ATTR leroi_tcp_client_init(){
	os_printf("\r\nCLIENT\r\n");
	char tcp_client[256];
	spi_flash_read(0x3b000,(uint32 *)tcp_client,256);
	os_printf("\r\nCLIENT:%s\r\n",tcp_client);
	if(strstr(tcp_client,"Client")){

		spi_flash_read(0x3b000,(uint32 *)tcp_client,256);
		char *remote_ip = leroi_getStr(tcp_client,"remote_ip=");
		os_printf("\r\nremote_port:%s\r\n",remote_ip);

		spi_flash_read(0x3b000,(uint32 *)tcp_client,256);
		char *remote_port = leroi_getStr(tcp_client,"remote_port=");
		os_printf("\r\nremote_port:%d\r\n",atoi(remote_port));
		leroi_sprintf_ip(remote_ip);

		struct ip_info info;
		const char remote_iparr[4]={iparr[0],iparr[1],iparr[2],iparr[3]};
		wifi_get_ip_info(STATION_IF,&info);
		leroi_client_init((struct ip_addr *)remote_iparr,&info.ip,atoi(remote_port));

	}
	return true;
}
