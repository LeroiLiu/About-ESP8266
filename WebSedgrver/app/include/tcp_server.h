/*
 * server.h
 *
 *  Created on: 2018Äê3ÔÂ24ÈÕ
 *      Author: Leroi
 */

#ifndef APP_INCLUDE_TCP_SERVER_H_
#define APP_INCLUDE_TCP_SERVER_H_

#include "user_main.h"
#include "leroi.h"

#define URLSize 1024

typedef enum ProtocolType{
	GET=0,
	POST,
} ProtocolType;
typedef struct URLFrame{
	enum ProtocolType Type;
	char pFilename[URLSize];
}URLFrame;

#define INDEX_HTML 2950
#endif /* APP_INCLUDE_TCP_SERVER_H_ */
