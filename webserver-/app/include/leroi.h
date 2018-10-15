/*
 * leroi.h
 *
 *  Created on: 2018年3月27日
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
 * 0x10000  HTML的位置
 * 0x3b000  CLIENT、SERVER、TCP、UDP的flash存储位置（1k大小）
 * 0x3b400
 * 0x3b800  STATION的设置
 * 0x3bc00  AP的位置
*/

char* leroi_getStr(char *precv,char *want);
char* leroi_authmode2str(AUTH_MODE authmode);
#endif /* APP_INCLUDE_LEROI_H_ */

