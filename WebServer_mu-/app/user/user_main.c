/*
 * user_main.c
 *
 *  Created on: 2018年3月12日
 *      Author: mac
 */

#include "user_main.h"
#include "temp.h"

static ETSTimer dhtTimer;
void hw_test_timer_cb(void)
{

}

void test_task(os_event_t *e)
{

	switch(e -> sig)
	{
	case 0:

		//os_sprintf("温度为：%d\r\n", ds18b20ReadTemp());
		break;
	default:
		break;

	}
}

void ICACHE_FLASH_ATTR Wifi_conned(void *arg)
{
	static uint8 count = 0;
	uint8 status;


	os_timer_disarm(&connect_timer);
	count++;
	status = wifi_station_get_connect_status();

	if(status == STATION_GOT_IP)
	{
		os_printf("WIFI connect success!");

		struct ip_info info;
		//获取IP
		wifi_get_ip_info(STATION_IF, &info);

		server_init(&info.ip, 80);

		return;
	}
	else
	{
		if(count >= 7)
		{
			os_printf("WIFI connect fail!");
			return;
		}
	}
	os_timer_arm(&connect_timer, 2000, NULL);
}

void to_scan(void)
{
	struct station_config stationConf;
	os_memcpy(&stationConf.ssid, "SANLONG-2.4G", 32);
	os_memcpy(&stationConf.password, "deng123456", 64);

	wifi_station_set_config_current(&stationConf);
	wifi_station_connect();
	os_timer_setfn(&connect_timer, Wifi_conned, NULL);
	os_timer_arm(&connect_timer, 2000, NULL);
}

void user_init()
{
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	wifi_set_opmode(3);
	//os_event_t *testQueue;

	//system_os_task(test_task, 0, testQueue,4);
	//hw_timer_init(FRC1_SOURCE, 1);
	//hw_timer_set_func(hw_test_timer_cb);
	//hw_timer_arm(1000000);
	DHTInit(SENSOR_DHT11);

	system_init_done_cb(to_scan);
}

void user_rf_pre_init()
{

}
