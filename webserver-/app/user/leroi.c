/*
 * leroi.c
 *
 *  Created on: 2018年3月27日
 *      Author: Leroi
 */

#include "leroi.h"

bool ICACHE_FLASH_ATTR leroi_sprintf_ip(char *ip){
	char temp[4];
	int i=0,m=0,n=0;
	while(i<15){
		if(ip[i]=='.'){
			temp[m]=0;
			iparr[n]=atoi(temp);
			n++;
			m=0;
		}else{
			temp[m]=ip[i];
			m++;
		}
		i++;
	}temp[m]=0;iparr[n]=atoi(temp);

	return true;
}



void ICACHE_FLASH_ATTR leroi_data_send(void *arg,bool response_OK,char *psend){
	char httphead[256];
	char *pbuf=NULL;
	uint16 length;
	struct espconn *pespconn=arg;
	if(response_OK){
		os_sprintf(httphead,"HTTP/1.1 200 OK\r\nContent-Length: %d\r\nServer: lwIP/1.4.0\r\n",psend ? os_strlen(psend) : 0);
		if(psend)
		{
			os_sprintf(httphead+strlen(httphead),"Content-Type: text/html; charset=utf-8\r\nPragma: no-cache\r\n\r\n");
			length=os_strlen(httphead)+os_strlen(psend);
			pbuf=(char *)os_zalloc(length+1);
			os_memcpy(pbuf,httphead,os_strlen(httphead));
			os_memcpy(pbuf+os_strlen(httphead),psend,os_strlen(psend));
		}else{
			os_sprintf(httphead+os_strlen(httphead),"\r\n");
			length=os_strlen(httphead);
		}
	}else{
		os_sprintf(httphead,"HTTP/1.1 400 BadRequest\r\nContent-Length: 0\r\nServer: lwIP/1.4.0\r\n");
		length=os_strlen(httphead);
	}
	if(psend)
	{
		espconn_sent(pespconn,pbuf,length);
	}else
	{
		espconn_sent(pespconn,httphead,length);
	}
	if(pbuf){
		os_free(pbuf);
		pbuf=NULL;
	}
}


//解析URL
bool ICACHE_FLASH_ATTR leroi_parse_url(char *precv,URLFrame *purl_frame){
	char *pbuffer=NULL;
	char *str=NULL;
	int length=0;
	if(precv==NULL || purl_frame==NULL){
		os_printf("%s","Wrong");
		return false;
	}
	pbuffer=(char *)os_strstr(precv,"Host: ");
	if(pbuffer!=NULL)
	{
		os_memset(purl_frame->pFilename,0,URLSize);
		if(os_strncmp(precv,"GET ",4)==0)
		{
			purl_frame->Type=GET;
			pbuffer=precv+4;
		}else if(os_strncmp(precv,"POST ",5)==0){
			purl_frame->Type=POST;
			pbuffer=precv+5;
		}else{
			return false;
		}
		pbuffer++;
		str=(char *)os_strstr(pbuffer," HTTP");
		if(str !=NULL){
			length=str-pbuffer;
			os_memcpy(purl_frame->pFilename,pbuffer,length);

		}else{
			os_printf("%s","Can not find HTTP");
		}

	}else{
		os_printf("%s","Can not find Host");
		return false;
	}

	return true;
}

//分离字符串
char* ICACHE_FLASH_ATTR leroi_getStr(char *precv,char *want){
	char *_precv = precv;
	int length;
	char *temp=NULL;
	char *self = strstr(_precv,want);
	if(self==NULL) return "err";
	self = self + strlen(want);
	if(self==NULL) return "err";
	char *left = strstr(self,"&");
	if(left==NULL){
		length = os_strlen(self);
	}else{
		length = os_strlen(self)-os_strlen(left);
	}
	self[length]=0;
	if(self==NULL) return "err";
	return self;
}

char* ICACHE_FLASH_ATTR leroi_authmode2str(AUTH_MODE authmode){
	char *authmodestr="";
	if(authmode==0){
		authmodestr = "AUTH_OPEN";
	}else if(authmode==1){
		authmodestr = "AUTH_WEP";
	}else if(authmode==2){
		authmodestr = "AUTH_WPA_PSK";
	}else if(authmode==3){
		authmodestr = "AUTH_WPA2_PSK";
	}else if(authmode==4){
		authmodestr = "AUTH_WPA_WPA2_PSK";
	}
	return authmodestr;
}




