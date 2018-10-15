/*
 * user_main.h
 *
 *  Created on: 2018Äê3ÔÂ23ÈÕ
 *      Author: Leroi
 */

#ifndef APP_INCLUDE_USER_MAIN_H_
#define APP_INCLUDE_USER_MAIN_H_

#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
ETSTimer connect_timer;
ETSTimer udp_sent_timer;
struct espconn user_udp_espconn;

#endif /* APP_INCLUDE_USER_MAIN_H_ */
