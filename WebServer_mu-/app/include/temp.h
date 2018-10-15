/*
 * temp.h
 *
 *  Created on: 2018��3��17��
 *      Author: mac
 */

#ifndef APP_INCLUDE_TEMP_H_
#define APP_INCLUDE_TEMP_H_

#include "user_main.h"
#include "c_types.h"
void ICACHE_FLASH_ATTR pollDHTCb(void * arg);

//����һ��ö�����ͣ���DHT11����DHT22
enum sensor_type{
	SENSOR_DHT11,SENSOR_DHT22
};
//����һ����ȡDHTģ�����ݵĽṹ������Ϊ�¶ȡ�ʪ�ȡ�ĳ��ģ�顢��������ż��Ƿ��ȡ�ɹ���ʶ
struct sensor_reading{
	float temperature;
	float humidity;
	const char* source;
	uint8_t sensor_id[16];
	BOOL success;
};

extern uint8 wendu,shidu;

//DHTģ���ʼ��������
void DHTInit(enum sensor_type);

#endif /* APP_INCLUDE_TEMP_H_ */
