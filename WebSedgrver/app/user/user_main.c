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

	char precv[1024];
	spi_flash_read(0x3b000,(uint32 *)precv,1024);
	os_printf("\r\nread:%s\r\n",precv);
	leroi_tcp_server_init(precv);
	leroi_set_softap_ip(precv);

	leroi_server_init(SOFTAP_IF,80);

}

void user_rf_pre_init(){}
