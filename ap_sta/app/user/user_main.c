/*
 * user_main.c
 *
 *  Created on: 2018年3月27日
 *      Author: Leroi
 */
#include "user_main.h"



void user_init(){

	uint8 opmode;
	struct softap_config config;
	bool ap_set_success;
	uart_init(BIT_RATE_115200,BIT_RATE_115200);

	opmode=wifi_get_opmode();
	os_printf("\r\n当前模式为：%d\r\n",opmode);
	wifi_set_opmode(0x03);
	opmode=wifi_get_opmode();
	os_printf("\r\n当前模式为：%d\r\n",opmode);

/*
	int station_ip[] = {192,168,1,1};
	int station_gw[] = {192,168,1,1};
	leroi_set_station_ip(station_ip,station_gw);
*/

	int softap_ip[] = {192,168,2,1};
	int softap_gw[] = {192,168,2,1};
	leroi_set_softap_ip(softap_ip,softap_gw);

	leroi_set_softap(config,"ESP_LLLL_2","123456789");
	leroi_connect_wifi("SANLONG-2.4G","deng123456");

	system_init_done_cb(to_scan);

	leroi_server_init(SOFTAP_IF,1213);

	wifi_set_event_handler_cb(leroi_wifi_handle_event_cb);

}

void user_rf_pre_init(){}
