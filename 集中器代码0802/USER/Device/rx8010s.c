#include "rx8010s.h"
#include "stm32_config.h"
#include "iic.h"
#include "delay.h"
#include "math.h"

#define RX8010_Write_Address 0x64
#define RX8010_Read_Address 0x65

_calendar_obj calendar;//时钟结构体


//指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 RX8010S_ReadOneByte(u8 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  	
	IIC_Send_Byte(RX8010_Write_Address);	  
	IIC_Wait_Ack();

    IIC_Send_Byte(ReadAddr);   //发送地址
	IIC_Wait_Ack();	
    
	IIC_Start(); 
	IIC_Send_Byte(RX8010_Read_Address);	   
	IIC_Wait_Ack();	
    temp=IIC_Read_Byte(1);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}

//指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//Data:要写入的数据
void RX8010S_WriteOneByte(u8 WriteAddr,u8 Data)
{				   	  	    																 
    IIC_Start();  
	IIC_Send_Byte(RX8010_Write_Address);	   
	IIC_Wait_Ack();	
	 										  		   
	IIC_Send_Byte(WriteAddr);     							   
	IIC_Wait_Ack();  
	IIC_Send_Byte(Data);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 	 
}
u8 DATA_TO_BCD(u8 data)
{
	u8 temp=0;
	u8 bcddata=0;
	temp = (data / 10);
	bcddata = (temp << 4); 
	temp = (data % 10);
	bcddata |= temp;
	return bcddata;
}

u8 BCD_TO_DATA(u8 bcd)
{
	u8 data=0;
	data = ((bcd & 0xf0) >> 4)*10 + (bcd &0x0f);
	return data;
}

void RX8010S_Set_Time(u8 syear,u8 smonth,u8 sday,u8 shour,u8 smin,u8 ssecond,u8 sweek)
{
//	u8 year=0;
//	u8 month=0;
//	u8 day=0;
//	u8 hour=0;
//	u8 min=0;
//	u8 second=0;
//	u8 week=0;
	syear = DATA_TO_BCD(syear);
	smonth = DATA_TO_BCD(smonth);
	sday = DATA_TO_BCD(sday);
	shour = DATA_TO_BCD(shour);
	smin = DATA_TO_BCD(smin);
	ssecond = DATA_TO_BCD(ssecond);
	sweek = 1<<sweek;
	
	RX8010S_WriteOneByte(RX8010_REG_YEAR,syear);
	RX8010S_WriteOneByte(RX8010_REG_MONTH,smonth);
	RX8010S_WriteOneByte(RX8010_REG_DAY,sday);
	RX8010S_WriteOneByte(RX8010_REG_HOUR,shour);
	RX8010S_WriteOneByte(RX8010_REG_MIN,smin);
	RX8010S_WriteOneByte(RX8010_REG_SEC,ssecond);
	RX8010S_WriteOneByte(RX8010_REG_WEEK,sweek);
}
u8 RX8010S_Get_Time(void)
{
	u8 year=0,month=0,day=0,hour=0,min=0,second=0,week=0;
	u8 cnt=0;
	year = RX8010S_ReadOneByte(RX8010_REG_YEAR);
	month = RX8010S_ReadOneByte(RX8010_REG_MONTH);
	day = RX8010S_ReadOneByte(RX8010_REG_DAY);
	hour = RX8010S_ReadOneByte(RX8010_REG_HOUR);
	min = RX8010S_ReadOneByte(RX8010_REG_MIN);
	second = RX8010S_ReadOneByte(RX8010_REG_SEC);
	week = RX8010S_ReadOneByte(RX8010_REG_WEEK);
	
	calendar.w_year = BCD_TO_DATA(year);
	calendar.w_month = BCD_TO_DATA(month);
	calendar.w_date = BCD_TO_DATA(day);
	calendar.hour = BCD_TO_DATA(hour);
	calendar.min = BCD_TO_DATA(min);
	calendar.sec = BCD_TO_DATA(second);
	while(week)
	{
		week = week / 2;
		cnt++;
	}
	calendar.week = (cnt - 1);
	return 0;
}
void RX8010S_Init(void)
{
	IIC_Init();
	
	RX8010S_ReadOneByte(0x20);
	RX8010S_WriteOneByte(0x17,0x58);
	RX8010S_WriteOneByte(0x30,0x0);
	RX8010S_WriteOneByte(0x31,0x18);
	RX8010S_WriteOneByte(0x32,0x0);
	
	RX8010S_WriteOneByte(RX8010_REG_EXT,0x04);
	RX8010S_WriteOneByte(RX8010_REG_EXT,0);
	RX8010S_WriteOneByte(RX8010_REG_CTRL,0x02);
	
	RX8010S_Set_Time(19,7,31,16,25,28,3);
	RX8010S_WriteOneByte(RX8010_REG_FLAG,0x0);
	RX8010S_WriteOneByte(RX8010_REG_CTRL,0x0);

}

