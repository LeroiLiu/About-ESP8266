/*
 * leroi.h
 *
 *  Created on: 2018��3��27��
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

//����IP
void leroi_set_station_ip(int * station_ip,int * station_gw);
void leroi_set_softap_ip(int * softap_ip,int * softap_gw);

//AP
//����AP��Ϣ
void leroi_set_softap(struct softap_config config,char *ssid,char *password);


//STATION

//wifiɨ����
void to_scan(void);
static void ICACHE_FLASH_ATTR scan_done(void *arg, STATUS status);

//����wifi��WiFi���ú���
void leroi_connect_wifi(char ssid[32],char password[64]);

//wifi���ӽ��
void leroi_connect_result(void *arg);


//��ΪTCP server�����Ķ˿�
void leroi_server_init(uint8 opmode,int port);
void leroi_server_listen(void *arg);
void leroi_server_recon(void *arg,sint8 err);
void leroi_server_recv(void *arg,char *pdata,unsigned short len);
void leroi_server_sent(void *arg);
void leroi_server_discon(void *arg);

//��ΪTCP client
void leroi_client_init(uint8 opmode,struct ip_addr *remote_ip,int remote_port,char *data);
void leroi_user_tcp_connect_cb(void *arg);
void leroi_user_tcp_recon_cb(void *arg,sint8 err);
void leroi_user_tcp_discon_cb(void *arg);
void leroi_user_tcp_sent_cb(void *arg);
void leroi_user_tcp_recv_cb(void *arg,char *pdata,unsigned short len);

/*TCP���ڼ���*/
void leroi_wifi_handle_event_cb(System_Event_t *evt);
void leroi_check_station(void *arg);




#endif /* APP_INCLUDE_LEROI_H_ */
