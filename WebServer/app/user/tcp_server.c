/*
 * server.c
 *
 *  Created on: 2018��3��24��
 *      Author: Leroi
 */
#include "tcp_server.h"

bool ICACHE_FLASH_ATTR leroi_save_ap_param(char *precv){
	uint32* input=(uint32*)precv;
	spi_flash_write(0x3bc00,input,256);

	char tcp_server[256];
	spi_flash_read(0x3bc00,(uint32 *)tcp_server,256);
	os_printf("\r\nCLIENT0:%s\r\n",tcp_server);
	os_printf("\r\nCLIENT1:%s\r\n",tcp_server);
	os_printf("\r\nCLIENT2:%s\r\n",tcp_server);
	return true;
}



/*
 * TCP ��Ϊ�����
 *
 * */

void ICACHE_FLASH_ATTR leroi_server_recv(void *arg,char *pdata,unsigned short len){

	char index_html[INDEX_HTML];

	URLFrame *pURLFrame=NULL;
	char *pParseBuffer=NULL;
	pURLFrame=(URLFrame *)os_zalloc(sizeof(URLFrame));
	char *precvdata=(char *)os_zalloc(len+1);
	os_memcpy(precvdata,pdata,len);
	leroi_parse_url(precvdata,pURLFrame);//����URL

	switch(pURLFrame->Type)
	{

	case GET:
		spi_flash_read(0x10000,(uint32 *)index_html,INDEX_HTML);
		index_html[INDEX_HTML]=0;
		leroi_data_send(arg,true,index_html);
		os_printf("\r\n\r\nget\r\n\r\n");
		os_printf("�յ�PC������get���ݣ�%s",pdata);
		os_printf("\r\n\r\nget\r\n\r\n");
		break;
	case POST:
		os_printf("\r\n\r\npost\r\n\r\n");
		os_printf("�յ�PC������post���ݣ�%s",pdata);
		os_printf("\r\n\r\npost\r\n\r\n");
	}

	os_free(pURLFrame);
	pURLFrame=NULL;
	os_free(precvdata);
	precvdata=NULL;

}
void ICACHE_FLASH_ATTR leroi_server_sent(void *arg){
	os_printf("���ͳɹ�����");
}
void ICACHE_FLASH_ATTR leroi_server_discon(void *arg){
	os_printf("�Ͽ�����");
}

void ICACHE_FLASH_ATTR leroi_server_recon(void *arg,sint8 err){
	os_printf("���Ӵ��󣬴������Ϊ��%d\r\n",err);
}

void ICACHE_FLASH_ATTR leroi_server_listen(void *arg){
	struct espconn *pespconn = arg;
	espconn_regist_recvcb(pespconn,leroi_server_recv);
	espconn_regist_sentcb(pespconn,leroi_server_sent);
	espconn_regist_disconcb(pespconn,leroi_server_discon);
}

/*
 * TCP ��Ϊ�����
 *
 * */
void ICACHE_FLASH_ATTR leroi_server_init(uint8 opmode,int port){
	struct ip_info ap_ip;
	wifi_get_ip_info(opmode,&ap_ip);
	LOCAL struct espconn esp_conn;
	//��ʼ������
	esp_conn.type = ESPCONN_TCP;
	esp_conn.state= ESPCONN_NONE;
	esp_conn.proto.tcp=(esp_tcp *)os_malloc(sizeof(esp_tcp));
	os_memcpy(esp_conn.proto.tcp->local_ip,&ap_ip.ip,4);
	esp_conn.proto.tcp->local_port=port;
	//ע�����ӳɹ��ص������������ص�����
	espconn_regist_connectcb(&esp_conn,leroi_server_listen);
	espconn_regist_reconcb(&esp_conn,leroi_server_recon);
	espconn_accept(&esp_conn);
}
