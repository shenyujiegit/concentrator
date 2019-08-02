#include "led.h" 
#include "stm32_config.h"


//LED IO初始化
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LEDR_GPIO_CLK, ENABLE);	//使能GPIOF时钟

	GPIO_InitStructure.GPIO_Pin = LEDR_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LEDR_GPIO_PORT, &GPIO_InitStructure);

}






