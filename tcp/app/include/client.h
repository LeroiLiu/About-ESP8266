/*
 * client.h
 *
 *  Created on: 2018��3��23��
 *      Author: Leroi
 */

#ifndef APP_INCLUDE_CLIENT_H_
#define APP_INCLUDE_CLIENT_H_

#include "user_main.h"
#include "espconn.h"
#include "mem.h"

struct espconn user_tcp_conn;

void ICACHE_FLASH_ATTR my_station_init(struct ip_addr *remote_ip,struct ip_addr *local_ip,int remote_port);




#endif /* APP_INCLUDE_CLIENT_H_ */
