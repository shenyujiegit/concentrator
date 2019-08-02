#include "eeprom.h"
#include "spi.h"
#include "stm32_config.h"
#include "delay.h"
#include "stdio.h"

void CAT25X_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(CAT25X_CS_GPIO_CLK, ENABLE);

	//CAT 25640 CSÅäÖÃ
	GPIO_InitStructure.GPIO_Pin = CAT25X_CS_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(CAT25X_CS_GPIO_PORT, &GPIO_InitStructure);
	
	CAT25X_CS = 1;
	SPI2_Init();
//	SPI2_SetSpeed(SPI_BaudRatePrescaler_256);
	
}

//Ð´Ê¹ÄÜ
void CAT25X_Write_Enable(void)   
{
	CAT25X_CS=0;                           
    SPI2_ReadWriteByte(EEPROM_WRITE_EN);	
}
//Ð´½ûÖ¹
void CAT25X_Write_Disenable(void)   
{
	CAT25X_CS=0;                           
    SPI2_ReadWriteByte(EEPROM_WRITE_DIS); 		
}
//Ð´×´Ì¬
void CAT25X_Write_SR(u8 stat)   
{   
	CAT25X_CS=0;                            
	SPI2_ReadWriteByte(EEPROM_WRITE_SR);   
	SPI2_ReadWriteByte(stat);              
	CAT25X_CS=1;                                 
}  
//¶Á×´Ì¬
u8 CAT25X_Read_SR(void)   
{   
	u8 stat=0;
	CAT25X_CS=0;                            
	SPI2_ReadWriteByte(EEPROM_READ_SR); 
	stat = SPI2_ReadWriteByte(0xFF); 	
	CAT25X_CS=1;
	return stat;
} 
//µÈ´ý¿ÕÏÐ
void CAT25X_Wait_Busy(void)   
{   
	while((CAT25X_Read_SR()&0x01)==0x01);   // µÈ´ýRDYÎ»Çå¿Õ
}
//¶ÁÊý¾Ý
void CAT25X_Read(char* Buff,u16 ReadAddr,u16 Num)   
{ 
 	u16 i;   										    
	CAT25X_CS=0;                            
    SPI2_ReadWriteByte(EEPROM_READ_MMY);           
    SPI2_ReadWriteByte((u8)(ReadAddr>>8));   	
    SPI2_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<Num;i++)
	{ 
        Buff[i]=SPI2_ReadWriteByte(0XFF);   
    }
	CAT25X_CS=1; 
	
} 

//Ð´Êý¾Ý
void CAT25X_Write(char* Buff,u16 ReadAddr,u16 Num)   
{ 
 	u16 i,n;
	
	CAT25X_CS=0;
	CAT25X_Write_Enable();
//	delay_us(10);
	CAT25X_CS=1;
	CAT25X_CS=0;		
	SPI2_ReadWriteByte(EEPROM_WRITE_MMY);
	SPI2_ReadWriteByte((u8)(ReadAddr>>8));   	
	SPI2_ReadWriteByte((u8)ReadAddr);	
	for(i=0;i<Num;i++)
	{ 
		SPI2_ReadWriteByte(*Buff++);   
	}
	CAT25X_CS=1; 
	CAT25X_Wait_Busy();		
}
