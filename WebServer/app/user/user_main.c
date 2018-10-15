/*
 * user_main.c
 *
 *  Created on: 2018Äê3ÔÂ27ÈÕ
 *      Author: Leroi
 */
#include "user_main.h"

void user_init(){

	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	wifi_set_opmode(0x03);

	leroi_server_init(SOFTAP_IF,80);

	char tcp_server[256];
	spi_flash_read(0x3bc00,(uint32 *)tcp_server,256);
	os_printf("\r\nCLIENT:%s\r\n",tcp_server);

}

void user_rf_pre_init(){}
