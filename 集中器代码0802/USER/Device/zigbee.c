#include "delay.h"
#include "zigbee.h"
#include "usart.h"
#include "string.h"
#include "stm32_config.h"

void zigbee_init(u32 BaudRate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(ZIGBEE_GPIO_CLK | ZIGBEEEN_GPIO_CLK, ENABLE);		

	GPIO_InitStructure.GPIO_Pin = ZIGBEE_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ZIGBEE_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ZIGBEEEN_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ZIGBEEEN_GPIO_PORT, &GPIO_InitStructure);
	
	ZIGBEERST =0;
	ZIGBEEEN = 1;
	usart3_config(BaudRate);	
}

int8_t zigbee_send_cmd(char *cmd,char *respond,u16 times)
{
	usart3_senddata(cmd,strlen(cmd));
	while(times--)
	{
		if(USART3_RX_STA == 0x60)
		{	
			if(strstr(USART3_RX_BUF,respond) !=NULL)
			{			
				USART3_RX_STA = 0x80;
				debugprintf("%s\r\n",USART3_RX_BUF);
				return 0;
			}
			else if(strstr(USART3_RX_BUF,"ERROR") !=NULL)
			{			
				USART3_RX_STA = 0x88;
				debugprintf("%s\r\n",USART3_RX_BUF);
				return -1;
			}
			else;
		}
		delay_ms(10);
	}
	USART3_RX_STA = 0x88;
	return -1;	
}
