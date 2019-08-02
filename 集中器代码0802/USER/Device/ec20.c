#include "delay.h"
#include "ec20.h"
#include "usart.h"
#include "stm32_config.h"


void ec20_config(u32 BaudRate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(EC20_PWRKEY_GPIO_CLK | EC20_RESET_GPIO_CLK | EC20_STATUS_GPIO_CLK | EC20_NETMOD_GPIO_CLK, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = EC20_PWRKEY_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(EC20_PWRKEY_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = EC20_RESET_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(EC20_RESET_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = EC20_STATUS_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(EC20_STATUS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = EC20_NETMOD_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(EC20_NETMOD_GPIO_PORT, &GPIO_InitStructure);
	
	EC20_PWRKEY = 0;
	EC20_RESET = 0;
		
	usart6_config(BaudRate);
}
//return 0开机成功，1，开机失败
u8 ec20_pwr_on(void)
{
	u8 cnt=0;
	delay_ms(100);
	EC20_PWRKEY = 1;
	delay_ms(600);
	EC20_PWRKEY = 0;
	while((EC20_STATUS != 0) && (cnt<=80))
	{
		cnt++;
		delay_ms(100);
	}
	debugprintf("cnt=%d\r\n",cnt);
	return EC20_STATUS;
}

//return 0关机成功，1，关机失败
u8 ec20_pwr_off(void)
{
	u8 cnt=0;
	EC20_PWRKEY = 0;
	delay_ms(50);
	EC20_PWRKEY = 1;
	delay_ms(800);
	EC20_PWRKEY = 0;
	while((EC20_STATUS == 0) && (cnt<=40))
	{
		cnt++;
		delay_ms(1000);
	}
	return !EC20_STATUS;
}
void ec20_senddata(char* buff,u8 len)
{
	usart6_senddata(buff,len);
}
