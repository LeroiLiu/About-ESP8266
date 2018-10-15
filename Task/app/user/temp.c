/*
 * temp.c
 *
 *  Created on: 2015��9��25��
 *      Author: Administrator
 */
#include "temp.h"
unsigned char ICACHE_FLASH_ATTR Ds18b20ReadByte(){
	uint8 j;
	unsigned char bi,byte;
for(j=0;j<8;j++){
	gpio_output_set(0,BIT5,BIT5,0);//�Ƚ���������
	os_delay_us(4);
	gpio_output_set(0,0,0,BIT5);//�ͷ�����
	os_delay_us(9);
	bi=GPIO_INPUT_GET(5);
	byte=(byte>>1) | (bi<<7);
	os_delay_us(51);

}
return byte;
}

void ICACHE_FLASH_ATTR Ds18b20WriteByte(unsigned char dat){
	uint8 j;
	for(j=0;j<8;j++){
		gpio_output_set(0,BIT5,BIT5,0);//�Ƚ���������
		os_delay_us(5);
		if(dat & 0x01){
			gpio_output_set(BIT5,0,BIT5,0);//д��1
		}else{
			gpio_output_set(0,BIT5,BIT5,0);//д��0��
		}
		os_delay_us(68);
		gpio_output_set(0,0,0,BIT5);//�ͷ�����
		dat>>=1;
		os_delay_us(4);

	}

}

bool ICACHE_FLASH_ATTR Ds18b20_init(){
	unsigned int i=0;
	gpio_output_set(0,BIT5,BIT5,0);//�Ƚ���������
	os_delay_us(642);
	gpio_output_set(BIT5,0,BIT5,BIT5);//�ͷ�����
	while(GPIO_INPUT_GET(5)){
		os_delay_us(60);
		i++;
		if(i>5)
		{
			return false;
		}
	}
	return true;

}
void ICACHE_FLASH_ATTR Ds18b20ReadTempCom(){
	Ds18b20_init();//��ds18b20��ʼ��
	os_delay_us(250);
	Ds18b20WriteByte(0xcc);//����ROM��������
	Ds18b20WriteByte(0xbe);//�¶�ת������


}
void ICACHE_FLASH_ATTR Ds18b20ChangeTemp(){
	Ds18b20_init();//��ds18b20��ʼ��
	os_delay_us(250);
	Ds18b20WriteByte(0xcc);//����ROM��������
	Ds18b20WriteByte(0x44);//�¶�ת������
	os_delay_us(1000);
}
int ICACHE_FLASH_ATTR ds18b20ReadTemp(){
float tp;
int temp=0;
unsigned char tml,tmh;
Ds18b20ChangeTemp();//��д��ת������
os_delay_us(250);
Ds18b20ReadTempCom();//��ȡ�¶�
tml=Ds18b20ReadByte();//��ȡ��λ
tmh=Ds18b20ReadByte();//��ȡ��λ
temp=tmh;
temp<<=8;
temp |=tml;
if(temp<0)
{
	temp--;
temp=~temp;
tp=temp;
temp=tp*0.0625*100+0.5;
	}else
	{
		tp=temp;
		temp=tp*0.0625*100+0.5;

	}
return temp;
}


