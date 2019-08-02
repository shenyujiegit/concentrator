#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 




#define MAX_USART2_RX_LEN 200
extern u8 USART2_RX_CNT;//接收数据个数
extern u8 USART2_RX_STA;//接收状态，1接收完成，0接收未完成
extern char USART2_RX_BUF[MAX_USART2_RX_LEN];//接收缓存区

#define MAX_USART1_RX_LEN 4096
extern u8 USART1_RX_CNT;//接收数据个数
extern u8 USART1_RX_STA;//接收状态，1接收完成，0接收未完成
extern char USART1_RX_BUF[MAX_USART1_RX_LEN];//接收缓存区

#define MAX_USART3_RX_LEN 200
extern u8 USART3_RX_CNT;//接收数据个数
extern u8 USART3_RX_STA;//接收状态，1接收完成，0接收未完成
extern char USART3_RX_BUF[MAX_USART3_RX_LEN];//接收缓存区

#define MAX_UART4_RX_LEN 200
extern u8 UART4_RX_CNT;//接收数据个数
extern u8 UART4_RX_STA;//接收状态，1接收完成，0接收未完成
extern char UART4_RX_BUF[MAX_UART4_RX_LEN];//接收缓存区

#define MAX_UART5_RX_LEN 200
extern u8 UART5_RX_CNT;//接收数据个数
extern u8 UART5_RX_STA;//接收状态，1接收完成，0接收未完成
extern char UART5_RX_BUF[MAX_UART5_RX_LEN];//接收缓存区

#define MAX_USART6_RX_LEN 200
extern u8 USART6_RX_CNT;//接收数据个数
extern u8 USART6_RX_STA;//接收状态，1接收完成，0接收未完成
extern char USART6_RX_BUF[MAX_USART6_RX_LEN];//接收缓存区

void usart1_config(u32 BaudRate);
void usart2_config(u32 BaudRate);
void usart3_config(u32 BaudRate);
void uart4_config(u32 BaudRate);
void uart5_config(u32 BaudRate);
void usart6_config(u32 BaudRate);
void usart2_senddata(char* buff,u8 len);
void usart1_senddata(char* buff,u8 len);
void usart3_senddata(char* buff,u8 len);
void uart4_senddata(char* buff,u8 len);
void uart5_senddata(char* buff,u8 len);
void usart6_senddata(char* buff,u8 len);
void Usart1FrameSend(void);


void TIM2_Config(u16 TIM2_Interval_xus);

#endif


