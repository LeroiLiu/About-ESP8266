/*
 * user_main.c
 *
 *  Created on: 2018年3月23日
 *      Author: Leroi
 */
#include "user_main.h"


void user_init(){
	uart_init(115200,115200);
	os_printf("\r\n start \r\n");
	char index1[]="<!DOCTYPE html><html><head><title>123</title></head><body><p>成功把</p></body></html>";
	spi_flash_write(0x10000,(uint32 *)index1,4096);
	char index2[256];
	spi_flash_read(0x10000,(uint32 *)index2,4096);
	os_printf("\r\n %s \r\n",index2);
	os_printf("\r\n end \r\n");

}

void user_rf_pre_init(){}
