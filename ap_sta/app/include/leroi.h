/*
 * leroi.h
 *
 *  Created on: 2018年3月27日
 *      Author: Leroi
 */

#ifndef APP_INCLUDE_LEROI_H_
#define APP_INCLUDE_LEROI_H_

#include "driver/uart.h"
#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "ets_sys.h"

ETSTimer connect_timer;
ETSTimer station_check;

char *sent_data;

//设置IP
void leroi_set_station_ip(int * station_ip,int * station_gw);
void leroi_set_softap_ip(int * softap_ip,int * softap_gw);

//AP
//设置AP信息
void leroi_set_softap(struct softap_config config,char *ssid,char *password);


//STATION

//wifi扫描检测
void to_scan(void);
static void ICACHE_FLASH_ATTR scan_done(void *arg, STATUS status);

//连接wifi的WiFi配置函数
void leroi_connect_wifi(char ssid[32],char password[64]);

//wifi连接结果
void leroi_connect_result(void *arg);


//作为TCP server监听的端口
void leroi_server_init(uint8 opmode,int port);
void leroi_server_listen(void *arg);
void leroi_server_recon(void *arg,sint8 err);
void leroi_server_recv(void *arg,char *pdata,unsigned short len);
void leroi_server_sent(void *arg);
void leroi_server_discon(void *arg);

//作为TCP client
void leroi_client_init(uint8 opmode,struct ip_addr *remote_ip,int remote_port,char *data);
void leroi_user_tcp_connect_cb(void *arg);
void leroi_user_tcp_recon_cb(void *arg,sint8 err);
void leroi_user_tcp_discon_cb(void *arg);
void leroi_user_tcp_sent_cb(void *arg);
void leroi_user_tcp_recv_cb(void *arg,char *pdata,unsigned short len);

/*TCP长期监听*/
void leroi_wifi_handle_event_cb(System_Event_t *evt);
void leroi_check_station(void *arg);




#endif /* APP_INCLUDE_LEROI_H_ */
