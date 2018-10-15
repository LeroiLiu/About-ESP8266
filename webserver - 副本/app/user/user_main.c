/*
 * user_main.c
 *
 *  Created on: 2018��3��27��
 *      Author: Leroi
 */
#include "user_main.h"

void user_init(){

	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	wifi_set_opmode(0x03);

	leroi_server_init(SOFTAP_IF,80);


	char tcp_server[256];
	spi_flash_read(0x3b000,(uint32 *)tcp_server,256);
	os_printf("\r\nCLIENT:%s\r\n",tcp_server);

	char softap_info[1024];
	spi_flash_read(0x3bc00,(uint32 *)softap_info,256);
	os_printf("\r\nCLIENT:%s\r\n",softap_info);

	char tcp_client[256];
	spi_flash_read(0x3b000,(uint32 *)tcp_client,256);
	os_printf("\r\nCLIENT:%s\r\n",tcp_client);


	//��ʼ��AP
	//leroi_set_softap_init();
	//��ʼ��TCP_SERVER
	//leroi_tcp_server_init();

	//�Ƿ���Station���ã��еĻ�������wifi
	//wifi_init();

}

void user_rf_pre_init(){}
