#ifndef _IIC_H
#define _IIC_H
#include "sys.h"

void IIC_Init(void);
void IIC_SDA_Out(void);
void IIC_SDA_In(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
u8 IIC_Read_Byte(u8 ACK);
void IIC_Send_Byte(u8 txd);

#endif

