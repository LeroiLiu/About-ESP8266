/*
 * server.c
 *
 *  Created on: 2018年3月24日
 *      Author: Leroi
 */
#include "tcp_server.h"

/*
 * TCP 作为服务端
 *
 * */

bool ICACHE_FLASH_ATTR leroi_save_param(char *precv){
	os_printf("\r\n\r\n save_parma:%s \r\n\r\n",precv);
	strcpy(global_precv,precv);

	uint32* input=(uint32*)precv;
	spi_flash_write(0x3b000,input,1024);

	char precvs[1024];
	spi_flash_read(0x3b000,(uint32 *)precvs,1024);
	os_printf("\r\nread:%s\r\n",precvs);

	wifi_init(precv);
	leroi_set_softap_init(precv);

	return true;
}

void ICACHE_FLASH_ATTR leroi_server_recv(void *arg,char *pdata,unsigned short len){

	os_printf("\r\n\r\npdata:%s\r\n\r\n",pdata);

	if(strstr(pdata,"HTTP")&&strstr(pdata,"Host")&&strstr(pdata,"Connection")&&strstr(pdata,"Cache-Control")){
		char index_html[INDEX_HTML];
		URLFrame *pURLFrame=NULL;
		char *pParseBuffer=NULL;
		pURLFrame=(URLFrame *)os_zalloc(sizeof(URLFrame));
		char *precvdata=(char *)os_zalloc(len+1);
		os_memcpy(precvdata,pdata,len);
		leroi_parse_url(precvdata,pURLFrame);//解析URL

		switch(pURLFrame->Type)
		{
		case GET:
			if(strncmp(pURLFrame->pFilename,"index.html",10)==0){
				spi_flash_read(0x10000,(uint32 *)index_html,INDEX_HTML);
				index_html[INDEX_HTML]=0;
				leroi_data_send(arg,true,index_html);
			}else{
				leroi_data_send(arg,false,NULL);
			}
			break;
		case POST:
			if(strncmp(pURLFrame->pFilename,"info.php",8)==0){
				pParseBuffer=(char *)os_strstr(precvdata,"\r\n\r\n");
				if(pParseBuffer==NULL){
					leroi_data_send(arg,false,NULL);
					break;
				}
				pParseBuffer+=4;
				if(leroi_save_param(pParseBuffer)==true)
					leroi_data_send(arg,true,"success");
				else{
					leroi_data_send(arg,false,NULL);
				}
				break;
			}else{
				leroi_data_send(arg,false,NULL);
			}
		}
		os_free(pURLFrame);
		pURLFrame=NULL;
		os_free(precvdata);
		precvdata=NULL;
	}else{
		leroi_data_send(arg,true,pdata);
	}

}
void ICACHE_FLASH_ATTR leroi_server_sent(void *arg){
	os_printf("发送成功！！");
}
void ICACHE_FLASH_ATTR leroi_server_discon(void *arg){
	os_printf("断开连接");
}

void ICACHE_FLASH_ATTR leroi_server_recon(void *arg,sint8 err){
	os_printf("连接错误，错误代码为：%d\r\n",err);
}

void ICACHE_FLASH_ATTR leroi_server_listen(void *arg){
	struct espconn *pespconn = arg;
	espconn_regist_recvcb(pespconn,leroi_server_recv);
	espconn_regist_sentcb(pespconn,leroi_server_sent);
	espconn_regist_disconcb(pespconn,leroi_server_discon);
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
