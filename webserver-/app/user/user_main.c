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
	spi_flash_read(0x3b000,(uint32 *)tcp_server,256);
	os_printf("\r\ntcp_server:%s\r\n",tcp_server);

	char softap_info[256];
	spi_flash_read(0x3bc00,(uint32 *)softap_info,256);
	os_printf("\r\nsoftap_info:%s\r\n",softap_info);

	char station[256];
	spi_flash_read(0x3b800,(uint32 *)station,256);
	os_printf("\r\ntcp_client:%s\r\n",station);

}

void user_rf_pre_init(){}
