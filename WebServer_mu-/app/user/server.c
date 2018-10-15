/*
 * server.c
 *
 *  Created on: 2018年3月13日
 *      Author: mac
 */

#include "server.h"

void ICACHE_FLASH_ATTR data_send(void *arg, bool response_OK, char *psend)
{
	char httphead[256];
	char *pbuf = NULL;
	uint16 length;
	struct espconn *pespconn = arg;

	if(response_OK)
	{
		os_sprintf(httphead, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\nServer: lwIP/1.4.0\r\n", psend ? os_strlen(psend) : 0);

		if(psend)
		{
			os_sprintf(httphead + strlen(httphead), "Content-Type: text/html; charset=utf-8\r\nPragma: no-cache\r\n\r\n");
			length = os_strlen(httphead) + os_strlen(psend);
			pbuf = (char *)os_zalloc(length + 1);
			os_memcpy(pbuf, httphead, os_strlen(httphead));
			os_memcpy(pbuf + os_strlen(httphead), psend, os_strlen(psend));
		}
		else
		{
			os_sprintf(httphead + os_strlen(httphead), "\r\n");
			length = os_strlen(httphead);
		}
	}
	else
	{
		os_sprintf(httphead, "HTTP/1.1 400 BadRequest\r\nContent-Length: 0\r\nServer: lwIP/1.4.0\r\n");
		length = os_strlen(httphead);
	}
	if(psend)
	{
		espconn_sent(pespconn, pbuf, length);
	}
	else
	{
		espconn_sent(pespconn, httphead, length);
	}
	if(pbuf)
	{
		os_free(pbuf);
		pbuf = NULL;
	}
}



void ICACHE_FLASH_ATTR server_recv(void *arg, char *pdata, unsigned short len)
{
	char index2[4096];//4096随便写的

	spi_flash_read(0x100 * 4096,(uint32 *)index2, 4096);
	index2[INDEX_SIZE] = 0;

	data_send(arg, true, index2);

}

void ICACHE_FLASH_ATTR server_sent(void *arg)
{
	os_printf("\r\n 发送成功 \r\n");
}

void ICACHE_FLASH_ATTR server_discon(void *arg)
{
	os_printf("\r\n 连接已经断开 \r\n");
}

void ICACHE_FLASH_ATTR server_connect_cb(void *arg)
{
	struct espconn *pespconn = arg;

	espconn_regist_recvcb(pespconn, server_recv);

	espconn_regist_sentcb(pespconn, server_sent);

	espconn_regist_disconcb(pespconn, server_discon);
}

void ICACHE_FLASH_ATTR server_recon_cb(void *arg, sint8 err)
{
	os_printf("\r\n连接错误，错误代码为：%d", err);
}

void ICACHE_FLASH_ATTR server_init(struct ip_addr *local_ip, int port)
{
	LOCAL struct espconn esp_conn;
	//初始化espconn参数
	esp_conn.type = ESPCONN_TCP;
	esp_conn.state = ESPCONN_NONE;
	esp_conn.proto.tcp = (esp_tcp *)os_malloc(sizeof(esp_tcp));
	os_memcpy(esp_conn.proto.tcp -> local_ip, local_ip, 4);
	esp_conn.proto.tcp -> local_port = port;

	//注册连接成功回调函数和重新连接回调函数
	espconn_regist_connectcb(&esp_conn, server_connect_cb);
	espconn_regist_reconcb(&esp_conn, server_recon_cb);

	espconn_accept(&esp_conn);
}


