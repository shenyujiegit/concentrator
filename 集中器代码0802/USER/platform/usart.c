#include "sys.h"
#include "usart.h"
#include "string.h"
#include "FreeRTOS.h"					//FreeRTOS使用
#include "common.h"
#include "stm32_config.h"

#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
	USART1->DR = (u8) ch;
	return ch;
}
#endif



u8 USART1_RX_CNT = 0;//接收数据个数
u8 USART1_RX_STA = 0;//接收状态，1接收完成，0接收未完成
char USART1_RX_BUF[MAX_USART1_RX_LEN];//接收缓存区

u8 USART2_RX_CNT = 0;//接收数据个数
u8 USART2_RX_STA = 0;//接收状态，1接收完成，0接收未完成
char USART2_RX_BUF[MAX_USART2_RX_LEN];//接收缓存区

u8 USART3_RX_CNT = 0;//接收数据个数
u8 USART3_RX_STA = 0;//接收状态，1接收完成，0接收未完成
char USART3_RX_BUF[MAX_USART3_RX_LEN];//接收缓存区

u8 UART4_RX_CNT = 0;//接收数据个数
u8 UART4_RX_STA = 0;//接收状态，1接收完成，0接收未完成
char UART4_RX_BUF[MAX_UART4_RX_LEN];//接收缓存区

u8 UART5_RX_CNT = 0;//接收数据个数
u8 UART5_RX_STA = 0;//接收状态，1接收完成，0接收未完成
char UART5_RX_BUF[MAX_UART5_RX_LEN];//接收缓存区

u8 USART6_RX_CNT = 0;//接收数据个数
u8 USART6_RX_STA = 0;//接收状态，1接收完成，0接收未完成
char USART6_RX_BUF[MAX_USART6_RX_LEN];//接收缓存区

//串口1初始化
void usart1_config(u32 BaudRate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

	USART_Cmd(USART1, DISABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_AHB1PeriphClockCmd(USART1_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    GPIO_PinAFConfig(USART1_GPIO_PORT, USART1_TX_GPIO_PinSource, GPIO_AF_USART1);
    GPIO_PinAFConfig(USART1_GPIO_PORT, USART1_RX_GPIO_PinSource, GPIO_AF_USART1);

	/*配置USART1的发送管脚TXD(PA9)为复用推挽输出*/
	GPIO_InitStructure.GPIO_Pin=USART1_TX_GPIO_Pin;                                       //设置初始化GPIO为PIN9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;                               //设置GPIO的速度为50MHz
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;                                    //设置GPIO模式为复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								  //设置GPIO输出类型为推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						          //设置GPIO为连接上拉电阻
	GPIO_Init(USART1_GPIO_PORT,&GPIO_InitStructure);                                         //初始化GPIOA的PIN9
	GPIO_InitStructure.GPIO_Pin=USART1_RX_GPIO_Pin;                                      //设置初始化GPIO为PIN10
	/*配置USART1的接收管脚RXD(PA10)为浮空输入*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;                                    //设置GPIO的模式为复用模式
	GPIO_Init(USART1_GPIO_PORT,&GPIO_InitStructure);                                         //初始化GPIOA的PIN10

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate =BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//开启空闲中断

    USART_Cmd(USART1, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ClearFlag(USART1, USART_FLAG_TXE);
}

//串口2初始化
void usart2_config(u32 BaudRate)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(USART2_GPIO_CLK,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟

	//串口2对应引脚复用映射
	GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_TX_GPIO_PinSource,GPIO_AF_USART2);
	GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_RX_GPIO_PinSource,GPIO_AF_USART2); 

	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_Pin | USART2_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(USART2_GPIO_PORT,&GPIO_InitStructure); 

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART2, ENABLE);  //使能串口2
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_TXE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);//开启空闲中断
	

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

void usart3_config(u32 BaudRate)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(USART3_GPIO_CLK,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART2时钟

	//串口2对应引脚复用映射
	GPIO_PinAFConfig(USART3_GPIO_PORT,USART3_TX_GPIO_PinSource,GPIO_AF_USART3);
	GPIO_PinAFConfig(USART3_GPIO_PORT,USART3_RX_GPIO_PinSource,GPIO_AF_USART3); 

	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_Pin | USART3_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(USART3_GPIO_PORT,&GPIO_InitStructure); 

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART3, ENABLE);  //使能串口2
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ClearFlag(USART3, USART_FLAG_TXE);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);//开启空闲中断
	

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}
void uart4_config(u32 BaudRate)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(UART4_GPIO_CLK,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能UART5时钟

	//串口2对应引脚复用映射
	GPIO_PinAFConfig(UART4_GPIO_PORT,UART4_TX_GPIO_PinSource,GPIO_AF_UART4);
	GPIO_PinAFConfig(UART4_GPIO_PORT,UART4_RX_GPIO_PinSource,GPIO_AF_UART4); 

	//USART5 TX端口配置
	GPIO_InitStructure.GPIO_Pin = UART4_TX_GPIO_Pin | UART4_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(UART4_GPIO_PORT,&GPIO_InitStructure);

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口4

	USART_Cmd(UART4, ENABLE);  //使能串口5
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_ClearFlag(UART4, USART_FLAG_TXE);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(UART4, USART_IT_IDLE,ENABLE);//开启空闲中断
	

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}
void uart5_config(u32 BaudRate)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(UART5_RX_GPIO_CLK | UART5_TX_GPIO_CLK,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能UART5时钟

	//串口2对应引脚复用映射
	GPIO_PinAFConfig(UART5_TX_GPIO_PORT,UART5_TX_GPIO_PinSource,GPIO_AF_UART5);
	GPIO_PinAFConfig(UART5_RX_GPIO_PORT,UART5_RX_GPIO_PinSource,GPIO_AF_UART5); 

	//USART5 TX端口配置
	GPIO_InitStructure.GPIO_Pin = UART5_TX_GPIO_Pin | UART5_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(UART5_TX_GPIO_PORT,&GPIO_InitStructure);

	//USART5 RX端口配置
	GPIO_InitStructure.GPIO_Pin = UART5_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(UART5_RX_GPIO_PORT,&GPIO_InitStructure); 	

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART5, &USART_InitStructure); //初始化串口5

	USART_Cmd(UART5, ENABLE);  //使能串口5
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_ClearFlag(UART5, USART_FLAG_TXE);

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(UART5, USART_IT_IDLE,ENABLE);//开启空闲中断
	

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

void usart6_config(u32 BaudRate)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(USART6_GPIO_CLK,ENABLE); //使能GPIOD时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART2时钟

	//串口2对应引脚复用映射
	GPIO_PinAFConfig(USART6_GPIO_PORT,USART6_TX_GPIO_PinSource,GPIO_AF_USART6);
	GPIO_PinAFConfig(USART6_GPIO_PORT,USART6_RX_GPIO_PinSource,GPIO_AF_USART6); 

	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = USART6_TX_GPIO_Pin | USART6_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(USART6_GPIO_PORT,&GPIO_InitStructure); 

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART6, ENABLE);  //使能串口2
	USART_ClearFlag(USART6, USART_FLAG_TC);
	USART_ClearFlag(USART6, USART_FLAG_TXE);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);//开启空闲中断
	

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}
void usart1_senddata(char* buff,u8 len)
{
	while(len--)
	{
		USART_SendData(USART1, *buff++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
}
void usart2_senddata(char* buff,u8 len)
{
	while(len--)
	{
		USART_SendData(USART2, *buff++);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	}
}

void usart3_senddata(char* buff,u8 len)
{
	while(len--)
	{
		USART_SendData(USART3, *buff++);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}
void uart4_senddata(char* buff,u8 len)
{
	while(len--)
	{
		USART_SendData(UART4, *buff++);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
	}
}
void uart5_senddata(char* buff,u8 len)
{
	while(len--)
	{
		USART_SendData(UART5, *buff++);
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
	}
}
void usart6_senddata(char* buff,u8 len)
{
	while(len--)
	{
		USART_SendData(USART6, *buff++);
		while(USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
	}
}
//定时器2初始化，主要用于监测串口是否接收完一帧数据
void TIM2_Config(u16 TIM2_Interval_xus)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;//设置中断优先级
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);	
	  
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//外设时钟使能  				
  	TIM_TimeBaseStructure.TIM_Period = 8400 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler = TIM2_Interval_xus-1;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);  		
}

//定时器2中断入口
void TIM2_IRQHandler(void)
{
	static u16 cnt = 0;
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
		
		if((cnt ++) >= 100)
		{
			cnt = 0;
			
			SysTick1s ++;
		}
	}
}





