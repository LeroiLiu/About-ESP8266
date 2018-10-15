/*
 * initesp.c
 *
 *  Created on: 2018年4月8日
 *      Author: Leroi
 */


#include "initesp.h"


bool ICACHE_FLASH_ATTR leroi_tcp_server_init(char *precv){
	os_printf("\r\nstart\r\n");
	if(strstr(precv,"Server")&&strstr(precv,"TCP")){
		os_printf("\r\nserver\r\n");
		char local_port_arr[1024];strcpy(local_port_arr,precv);
		char *local_port =strtok(strstr(local_port_arr,"local_port=")+11,"&");
		if(strstr(local_port,"remote_port")){return false;}
		leroi_server_init(SOFTAP_IF,atoi(local_port));
	}else if(strstr(precv,"Client")&&strstr(precv,"TCP")){
		os_printf("\r\nclient\r\n");
		char remote_port_arr[1024];strcpy(remote_port_arr,precv);
		char *remote_port =strtok(strstr(remote_port_arr,"remote_port=")+12,"&");
		if(strstr(remote_port,"submit")){return false;}

		char remote_ip_arr[1024];strcpy(remote_ip_arr,precv);
		char *remote_ip =strtok(strstr(remote_ip_arr,"remote_ip=")+10,"&");
		if(strstr(remote_ip,"local_port")){return false;}

		os_printf("\r\nremote_port:%s\r\nremote_ip:%s\r\n",remote_port,remote_ip);

		char *token = strtok(remote_ip,".");int i=0;
		int ipres[4];
		while( token != NULL )
		{
			ipres[i]=atoi(token);
			token = strtok(NULL, ".");
			i++;
		}
		struct ip_info info;
		os_printf("\r\n%d-%d-%d-%d\r\n",ipres[0],ipres[1],ipres[2],ipres[3]);
		const char remote_iparr[4]={ipres[0],ipres[1],ipres[2],ipres[3]};
		wifi_get_ip_info(STATION_IF,&info);
		leroi_client_init((struct ip_addr *)remote_iparr,&info.ip,atoi(remote_port));
	}
	os_printf("\r\nend\r\n");
	return true;
}

//设置AP信息
bool ICACHE_FLASH_ATTR leroi_set_softap_ip(char *precv){
	if(strstr(precv,"ipaddress")){
		char ipaddress[1024];strcpy(ipaddress,precv);
		char netmask[1024];strcpy(netmask,precv);
		char gateway_address[1024];strcpy(gateway_address,precv);
		char *ap_ipaddress =strtok(strstr(ipaddress,"ap_ipaddress=")+13,"&");
		if(strstr(ap_ipaddress,"ap_netmask")){return false;}
		char *ap_netmask =strtok(strstr(netmask,"ap_netmask=")+11,"&");
		if(strstr(ap_ipaddress,"ap_gateway_address")){return false;}
		char *ap_gateway_address =strtok(strstr(gateway_address,"ap_gateway_address=")+19,"&");
		if(strstr(ap_ipaddress,"sta_name")){return false;}

		struct ip_info info;
		wifi_softap_dhcps_stop();
		//IP设置
		/* 获取第一个子字符串 */
		char *token = strtok(ap_ipaddress,".");int i=0;
		int ipres[4];
		/* 继续获取其他的子字符串 */
		while( token != NULL )
		{
			ipres[i]=atoi(token);
			token = strtok(NULL, ".");
			i++;
		}
		IP4_ADDR(&info.ip, ipres[0],ipres[1],ipres[2],ipres[3]);
		//GW设置
		/* 获取第一个子字符串 */
		token = strtok(ap_gateway_address,".");i=0;
		/* 继续获取其他的子字符串 */
		while( token != NULL )
		{
			ipres[i]=atoi(token);
			token = strtok(NULL, ".");
			i++;
		}
		IP4_ADDR(&info.gw, ipres[0],ipres[1],ipres[2],ipres[3]);
		//GW设置
		/* 获取第一个子字符串 */
		token = strtok(ap_netmask,".");i=0;
		/* 继续获取其他的子字符串 */
		while( token != NULL )
		{
			ipres[i]=atoi(token);
			token = strtok(NULL, ".");
			i++;
		}
		IP4_ADDR(&info.netmask, ipres[0],ipres[1],ipres[2],ipres[3]);
		wifi_set_ip_info(SOFTAP_IF, &info);
		wifi_softap_dhcps_start();
		return true;
	}else{
		return false;
	}

}

bool ICACHE_FLASH_ATTR leroi_set_softap_init(char *precv){

    char name[1024];strcpy(name,precv);
    char password[1024];strcpy(password,precv);
    char encrypt[1024];strcpy(encrypt,precv);

	char *ap_name =strtok(strstr(name,"ap_name=")+8,"&");
	if(strstr(ap_name,"ap_password")){return false;}
	char *ap_password =strtok(strstr(password,"ap_password=")+12,"&");
	if(strstr(ap_password,"ap_encrypt")){return false;}
	char *ap_encrypt =strtok(strstr(encrypt,"ap_encrypt=")+11,"&");
	if(strstr(ap_encrypt,"ap_ipaddress")){return false;}

	os_printf("\r\n\r\nap_name:%s\r\nap_password:%s\r\nap_encrypt:%s\r\n\r\n",ap_name,ap_password,ap_encrypt);

	uint8 ap_auth;

		struct softap_config config;
		wifi_softap_get_config_default(&config);
		os_memcpy(config.ssid,(uint8 *)ap_name,os_strlen(ap_name));
		config.ssid_len = os_strlen(ap_name);
		os_memcpy(config.password,(uint8 *)ap_password,os_strlen(ap_password));

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
}
