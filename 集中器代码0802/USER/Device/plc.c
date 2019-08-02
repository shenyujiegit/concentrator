#include "plc.h"
#include "usart.h"
#include "stm32_config.h"

plc_at_info ps1642_cmd;

void plc_config(u32 BaudRate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(PLCRST_GPIO_CLK, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = PLCRST_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PLCRST_GPIO_PORT, &GPIO_InitStructure);
	PLCRST =1;
	
	usart2_config(BaudRate);
}

u8 at_csum(char *buf,u8 len)
{
	u8 csum=0,i=0;
	for(i=0;i<len;i++)
	{
		csum += *buf++; 
	}
	return csum;
}

u8 at_cxor(char *buf,u8 len)
{
	u8 csum=0,i=0;
	for(i=0;i<len;i++)
	{
		csum ^= *buf++; 
	}
	return csum;
}
//cmd:发送命令，buff:发送数据缓存，len:发送数据长度
void plc_send_cmd(u8 cmd,u8 *buff,u8 len)
{
	char tempbuf[10];
	u16 i=0;
	
	ps1642_cmd.fh=0x79;
	ps1642_cmd.len=len;
	ps1642_cmd.ctrl=0x58;
	ps1642_cmd.cmd=cmd;

	tempbuf[0] = ps1642_cmd.fh;
	tempbuf[1] = (u8)ps1642_cmd.len;
	tempbuf[2] = (u8)(ps1642_cmd.len>>8);
	tempbuf[3] = ps1642_cmd.ctrl;
	tempbuf[4] = ps1642_cmd.cmd;
	for(i=0;i<len;i++)
	{
		tempbuf[5+i]=buff[i];
	}
	ps1642_cmd.csum = at_csum(tempbuf+1,(4+len));
	ps1642_cmd.cxor = at_cxor(tempbuf+1,(4+len));
	tempbuf[5+len] =ps1642_cmd.csum;
	tempbuf[6+len] =ps1642_cmd.csum;
		
	usart2_senddata(tempbuf,(7+len));
	
}
