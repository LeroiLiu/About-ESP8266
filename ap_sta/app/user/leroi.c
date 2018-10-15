/*
 * leroi.c
 *
 *  Created on: 2018年3月27日
 *      Author: Leroi
 */

#include "leroi.h"

//设置IP
void ICACHE_FLASH_ATTR leroi_set_station_ip(int * station_ip,int * station_gw){
	struct ip_info info;
	wifi_station_dhcpc_stop();
	IP4_ADDR(&info.ip, station_ip[0], station_ip[1], station_ip[2], station_ip[3]);//IP地址
	IP4_ADDR(&info.gw, station_gw[0], station_gw[1], station_gw[2], station_gw[3]);//网关地址
	IP4_ADDR(&info.netmask, 255, 255, 255, 0);
	wifi_set_ip_info(STATION_IF, &info);
	wifi_station_dhcpc_start();
}

void ICACHE_FLASH_ATTR leroi_set_softap_ip(int * softap_ip,int * softap_gw){
	struct ip_info info;
	wifi_softap_dhcps_stop();
	IP4_ADDR(&info.ip, softap_ip[0], softap_ip[1], softap_ip[2], softap_ip[3]);//IP地址
	IP4_ADDR(&info.gw, softap_gw[0], softap_gw[1], softap_gw[2], softap_gw[3]);//网关地址
	IP4_ADDR(&info.netmask, 255, 255, 255, 0);
	wifi_set_ip_info(SOFTAP_IF, &info);
	wifi_softap_dhcps_start();
}

//设置AP信息
void ICACHE_FLASH_ATTR leroi_set_softap(struct softap_config config,char *ssid,char *password){
	wifi_softap_get_config_default(&config);
	os_memcpy(config.ssid,ssid,strlen(ssid));
	os_memcpy(config.password,password,strlen(password));
	config.ssid_len = strlen(ssid);
	wifi_softap_set_config_current(&config);
}


//wifi扫描检测
void to_scan(void){ wifi_station_scan(NULL,scan_done); }
static void ICACHE_FLASH_ATTR scan_done(void *arg, STATUS status) {
	if (status == OK) {
		uint8 ssid[33];
		struct bss_info *bss_link = (struct bss_info *)arg;
		bss_link = bss_link->next.stqe_next; //ignore first

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
	}else{

	}
}

//连接wifi的WiFi配置函数
void ICACHE_FLASH_ATTR leroi_connect_wifi(char ssid[32],char password[64]){
	struct station_config stationConf;
	os_memcpy(&stationConf.ssid, ssid, 32);
	os_memcpy(&stationConf.password, password, 64);
	wifi_station_set_config_current(&stationConf);
	wifi_station_connect();
	os_timer_setfn(&connect_timer,leroi_connect_result,NULL);
	os_timer_arm(&connect_timer,2000,NULL);
}

//wifi连接结果
void ICACHE_FLASH_ATTR leroi_connect_result(void *arg){
	static uint8 count=0;
		   uint8 status;
	os_timer_disarm(&connect_timer);
	count++;
	status = wifi_station_get_connect_status();
	if(status==STATION_GOT_IP){
		os_printf("WiFi 连接成功");
		return;
	}else{
		if(count>=7){
			os_printf("WiFi 连接失败");
			return;
		}
	}
	os_timer_arm(&connect_timer,2000,NULL);
}



/*
 * TCP 作为服务端
 *
 * */
void ICACHE_FLASH_ATTR leroi_server_init(uint8 opmode,int port){
	struct ip_info ap_ip;
	wifi_get_ip_info(opmode,&ap_ip);
	LOCAL struct espconn esp_conn;
	//初始化参数
	esp_conn.type = ESPCONN_TCP;
	esp_conn.state= ESPCONN_NONE;
	esp_conn.proto.tcp=(esp_tcp *)os_malloc(sizeof(esp_tcp));
	os_memcpy(esp_conn.proto.tcp->local_ip,&ap_ip.ip,4);
	esp_conn.proto.tcp->local_port=port;
	//注册连接成功回掉函数和重连回掉函数
	espconn_regist_connectcb(&esp_conn,leroi_server_listen);
	espconn_regist_reconcb(&esp_conn,leroi_server_recon);
	espconn_accept(&esp_conn);
}

void ICACHE_FLASH_ATTR leroi_server_listen(void *arg){
	struct espconn *pespconn = arg;
	espconn_regist_recvcb(pespconn,leroi_server_recv);
	espconn_regist_sentcb(pespconn,leroi_server_sent);
	espconn_regist_disconcb(pespconn,leroi_server_discon);
}
void ICACHE_FLASH_ATTR leroi_server_recon(void *arg,sint8 err){
	os_printf("连接错误，错误代码为：%d\r\n",err);
}

void ICACHE_FLASH_ATTR leroi_server_recv(void *arg,char *pdata,unsigned short len){
	os_printf("收到PC发来的数据：%s",pdata);
	espconn_sent((struct espconn *)arg,"已经接收",strlen("已经接收"));
}
void ICACHE_FLASH_ATTR leroi_server_sent(void *arg){
	os_printf("发送成功！！");
}
void ICACHE_FLASH_ATTR leroi_server_discon(void *arg){
	os_printf("断开连接");
}


/*
 * TCP 作为客户端
 *
 * */
void ICACHE_FLASH_ATTR leroi_client_init(uint8 opmode,struct ip_addr *remote_ip,int remote_port,char *data){
	struct ip_info ap_ip;
	wifi_get_ip_info(opmode,&ap_ip);
	LOCAL struct espconn user_tcp_conn;
	sent_data = data;
	//参数配置
	user_tcp_conn.type = ESPCONN_TCP;
	user_tcp_conn.state=ESPCONN_NONE;
	user_tcp_conn.proto.tcp=(esp_tcp *)os_zalloc(sizeof(esp_tcp));
	os_memcpy(user_tcp_conn.proto.tcp->local_ip,&ap_ip.ip,4);
	os_memcpy(user_tcp_conn.proto.tcp->remote_ip,remote_ip,4);
	user_tcp_conn.proto.tcp->local_port=espconn_port();
	user_tcp_conn.proto.tcp->remote_port=remote_port;
	//注册连接回掉函数和重连回掉函数
	espconn_regist_connectcb(&user_tcp_conn,leroi_user_tcp_connect_cb);
	espconn_regist_reconcb(&user_tcp_conn,leroi_user_tcp_recon_cb);
	//启用连接
	espconn_connect(&user_tcp_conn);
}

void ICACHE_FLASH_ATTR leroi_user_tcp_connect_cb(void *arg){
	struct espconn *pespconn=arg;
	espconn_regist_recvcb(pespconn,leroi_user_tcp_recv_cb);//接收回掉函数
	espconn_regist_sentcb(pespconn,leroi_user_tcp_sent_cb);//发送成功的回掉函数
	espconn_regist_disconcb(pespconn,leroi_user_tcp_discon_cb);//重连的回掉函数
	espconn_sent(pespconn,sent_data,strlen(sent_data));
}
void ICACHE_FLASH_ATTR leroi_user_tcp_recon_cb(void *arg,sint8 err){
	os_printf("连接错误，错误代码为 %d\r\n",err);
	espconn_connect((struct espconn *)arg);
}
void ICACHE_FLASH_ATTR leroi_user_tcp_discon_cb(void *arg){
	os_printf("断开连接成功！\r\n");
}
void ICACHE_FLASH_ATTR leroi_user_tcp_sent_cb(void *arg){
	os_printf("发送成功！\r\n");
}
void ICACHE_FLASH_ATTR leroi_user_tcp_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("收到数据：%s\r\n",pdata);
	os_delay_us(300);
	espconn_disconnect((struct espconn *)arg);
}

/*TCP长期监听*/
void ICACHE_FLASH_ATTR leroi_wifi_handle_event_cb(System_Event_t *evt){
	switch (evt->event) {
		case EVENT_SOFTAPMODE_STACONNECTED:
			os_timer_disarm(&station_check);
			os_timer_setfn(&station_check,leroi_check_station,NULL);
			os_timer_arm(&station_check,2000,NULL);
			break;
		default:
			break;
	}
}
void ICACHE_FLASH_ATTR leroi_check_station(void *arg){
	os_timer_disarm(&station_check);
	struct station_info * station = wifi_softap_get_station_info();
	if(station){
		leroi_client_init(SOFTAP_IF,&station->ip,2345,"come from sent_data");
		os_printf("\r\n%s\r\n",&station->ip);
		os_free(station);
	}else{
		os_timer_arm(&station_check,2000,NULL);
	}
}
