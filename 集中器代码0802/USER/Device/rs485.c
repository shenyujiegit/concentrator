#include "rs485.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "stm32_config.h"



void rs485_config(u32 BaudRate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RS485_DIR_GPIO_CLK, ENABLE);	//Ê¹ÄÜGPIOFÊ±ÖÓ

	GPIO_InitStructure.GPIO_Pin = RS485_DIR_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(RS485_DIR_GPIO_PORT, &GPIO_InitStructure);
	
	RS485_DIR = 0;
//	usart2_config(BaudRate);
	uart5_config(BaudRate);
}

void rs485_senddata(char* buff,u8 len)
{
	RS485_DIR = 1;
	uart5_senddata(buff,len);
	RS485_DIR = 0;
}
