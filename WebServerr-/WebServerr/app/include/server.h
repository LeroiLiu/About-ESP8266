/*
 * server.h
 *
 *  Created on: 2018Äê3ÔÂ13ÈÕ
 *      Author: mac
 */

#ifndef APP_INCLUDE_SERVER_H_
#define APP_INCLUDE_SERVER_H_

#include "user_main.h"

void ICACHE_FLASH_ATTR server_init(struct ip_addr *local_ip, int port);

#define tempjson "{\"success\": {\"key\": \"temp\",\"value\": \"%d\"}}"
//{"success": {"key": "temp.","value": "%d"}}
//{success:{"key":temp","value":"%d"}}
#define URLSize 125
typedef enum ProtocolType{
	GET = 0,
	POST,
} ProtocolType;
typedef struct URLFrame{
	enum ProtocolType Type;
	char pFilename[URLSize];
}URLFrame;

#endif /* APP_INCLUDE_SERVER_H_ */
