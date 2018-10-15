/*
 * leroi.h
 *
 *  Created on: 2018��3��27��
 *      Author: Leroi
 */

#ifndef APP_INCLUDE_LEROI_H_
#define APP_INCLUDE_LEROI_H_

#include "stdio.h"
#include "string.h"
#include "driver/uart.h"
#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "ets_sys.h"
#include "wifi.h"
#include "initesp.h"
#include "tcp_server.h"
#include "tcp_client.h"

/*
 * 0x10000  HTML��λ��
 * 0x3b000  CLIENT��SERVER��TCP��UDP��flash�洢λ�ã�1k��С��
 * 0x3b400
 * 0x3b800  STATION������
 * 0x3bc00  AP��λ��
*/

char* leroi_getStr(char *precv,char *want);
char* leroi_authmode2str(AUTH_MODE authmode);
#endif /* APP_INCLUDE_LEROI_H_ */

