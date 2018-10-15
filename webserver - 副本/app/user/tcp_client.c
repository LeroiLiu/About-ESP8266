/*
 * client.c
 *
 *  Created on: 2018年3月23日
 *      Author: Leroi
 */
#include "tcp_client.h"

void ICACHE_FLASH_ATTR user_tcp_discon_cb(void *arg){
	os_printf("断开连接成功！\r\n");
}
void ICACHE_FLASH_ATTR user_tcp_sent_cb(void *arg){
	os_printf("发送成功！\r\n");
}
void ICACHE_FLASH_ATTR user_tcp_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("收到数据：%s\r\n",pdata);
	os_delay_us(300);
	espconn_disconnect((struct espconn *)arg);
}
void ICACHE_FLASH_ATTR user_tcp_recon_cb(void *arg,sint8 err){
	os_printf("连接错误，错误代码为 %d\r\n",err);
	espconn_connect((struct espconn *)arg);
}
void ICACHE_FLASH_ATTR user_tcp_connect_cb(void *arg){
	os_printf("\r\n 连接 \r\n");
	struct espconn *pespconn=arg;
	espconn_regist_recvcb(pespconn,user_tcp_recv_cb);//接收回掉函数
	espconn_regist_sentcb(pespconn,user_tcp_sent_cb);//发送成功的回掉函数
	espconn_regist_disconcb(pespconn,user_tcp_discon_cb);//重连的回掉函数
	espconn_sent(pespconn,"这是esp8266",strlen("这是esp8266"));
}


void ICACHE_FLASH_ATTR leroi_client_init(struct ip_addr *remote_ip,struct ip_addr *local_ip,int remote_port){
	os_printf("\r\n 设置参数 \r\n");
	//参数配置
	user_tcp_conn.type = ESPCONN_TCP;
	user_tcp_conn.state=ESPCONN_NONE;
	user_tcp_conn.proto.tcp=(esp_tcp *)os_zalloc(sizeof(esp_tcp));
	os_memcpy(user_tcp_conn.proto.tcp->local_ip,local_ip,4);
	os_memcpy(user_tcp_conn.proto.tcp->remote_ip,remote_ip,4);
	user_tcp_conn.proto.tcp->local_port=espconn_port();
	user_tcp_conn.proto.tcp->remote_port=remote_port;
	os_printf("\r\n 参数完成 \r\n");
	//注册连接回掉函数和重连回掉函数
	espconn_regist_connectcb(&user_tcp_conn,user_tcp_connect_cb);
	espconn_regist_reconcb(&user_tcp_conn,user_tcp_recon_cb);
	//启用连接
	espconn_connect(&user_tcp_conn);

}

