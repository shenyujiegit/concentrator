#include "relay.h" 
#include "stm32_config.h"


//RELAY IO初始化
void RELAY_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RELAY1_GPIO_CLK | RELAY2_GPIO_CLK | RELAY3_GPIO_CLK 
	| RELAY4_GPIO_CLK | RELAY5_GPIO_CLK | RELAY6_GPIO_CLK, ENABLE);	//使能GPIOF时钟

	GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_Pin | RELAY2_GPIO_Pin | RELAY3_GPIO_Pin | RELAY4_GPIO_Pin
	| RELAY5_GPIO_Pin | RELAY6_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(RELAY1_GPIO_PORT, &GPIO_InitStructure);
}

void RELAY_ON(u8 val)
{
	switch(val)
	{
		case 1:
			RELAY1 = 1;
		break;
		case 2:
			RELAY2 = 1;
		break;
		case 3:
			RELAY3 = 1;
		break;
		case 4:
			RELAY4 = 1;
		break;
		case 5:
			RELAY5 = 1;
		break;
		case 6:
			RELAY6 = 1;
		break;
		case 0xFF:
			RELAY1 = 1;
			RELAY2 = 1;
			RELAY3 = 1;
			RELAY4 = 1;
			RELAY5 = 1;
			RELAY6 = 1;
		break;
		default:
		break;
	}
}
//继电器关闭
void RELAY_OFF(u8 val)
{
	switch(val)
	{
		case 1:
			RELAY1 = 0;
		break;
		case 2:
			RELAY2 = 0;
		break;
		case 3:
			RELAY3 = 0;
		break;
		case 4:
			RELAY4 = 0;
		break;
		case 5:
			RELAY5 = 0;
		break;
		case 6:
			RELAY6 = 0;
		break;
		case 0xFF:
			RELAY1 = 0;
			RELAY2 = 0;
			RELAY3 = 0;
			RELAY4 = 0;
			RELAY5 = 0;
			RELAY6 = 0;
		break;
		default:
		break;
	}
}
