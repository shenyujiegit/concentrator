#include "sys.h"
#include "usart.h"
#include "string.h"
#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "common.h"
#include "stm32_config.h"

#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
	USART1->DR = (u8) ch;
	return ch;
}
#endif



u8 USART1_RX_CNT = 0;//�������ݸ���
u8 USART1_RX_STA = 0;//����״̬��1������ɣ�0����δ���
char USART1_RX_BUF[MAX_USART1_RX_LEN];//���ջ�����

u8 USART2_RX_CNT = 0;//�������ݸ���
u8 USART2_RX_STA = 0;//����״̬��1������ɣ�0����δ���
char USART2_RX_BUF[MAX_USART2_RX_LEN];//���ջ�����

u8 USART3_RX_CNT = 0;//�������ݸ���
u8 USART3_RX_STA = 0;//����״̬��1������ɣ�0����δ���
char USART3_RX_BUF[MAX_USART3_RX_LEN];//���ջ�����

u8 UART4_RX_CNT = 0;//�������ݸ���
u8 UART4_RX_STA = 0;//����״̬��1������ɣ�0����δ���
char UART4_RX_BUF[MAX_UART4_RX_LEN];//���ջ�����

u8 UART5_RX_CNT = 0;//�������ݸ���
u8 UART5_RX_STA = 0;//����״̬��1������ɣ�0����δ���
char UART5_RX_BUF[MAX_UART5_RX_LEN];//���ջ�����

u8 USART6_RX_CNT = 0;//�������ݸ���
u8 USART6_RX_STA = 0;//����״̬��1������ɣ�0����δ���
char USART6_RX_BUF[MAX_USART6_RX_LEN];//���ջ�����

//����1��ʼ��
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

	/*����USART1�ķ��͹ܽ�TXD(PA9)Ϊ�����������*/
	GPIO_InitStructure.GPIO_Pin=USART1_TX_GPIO_Pin;                                       //���ó�ʼ��GPIOΪPIN9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;                               //����GPIO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;                                    //����GPIOģʽΪ����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								  //����GPIO�������Ϊ�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						          //����GPIOΪ������������
	GPIO_Init(USART1_GPIO_PORT,&GPIO_InitStructure);                                         //��ʼ��GPIOA��PIN9
	GPIO_InitStructure.GPIO_Pin=USART1_RX_GPIO_Pin;                                      //���ó�ʼ��GPIOΪPIN10
	/*����USART1�Ľ��չܽ�RXD(PA10)Ϊ��������*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;                                    //����GPIO��ģʽΪ����ģʽ
	GPIO_Init(USART1_GPIO_PORT,&GPIO_InitStructure);                                         //��ʼ��GPIOA��PIN10

    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate =BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//���������ж�

    USART_Cmd(USART1, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ClearFlag(USART1, USART_FLAG_TXE);
}

//����2��ʼ��
void usart2_config(u32 BaudRate)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(USART2_GPIO_CLK,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��

	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_TX_GPIO_PinSource,GPIO_AF_USART2);
	GPIO_PinAFConfig(USART2_GPIO_PORT,USART2_RX_GPIO_PinSource,GPIO_AF_USART2); 

	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_Pin | USART2_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(USART2_GPIO_PORT,&GPIO_InitStructure); 

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������1

	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_TXE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);//���������ж�
	

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}

void usart3_config(u32 BaudRate)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(USART3_GPIO_CLK,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART2ʱ��

	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(USART3_GPIO_PORT,USART3_TX_GPIO_PinSource,GPIO_AF_USART3);
	GPIO_PinAFConfig(USART3_GPIO_PORT,USART3_RX_GPIO_PinSource,GPIO_AF_USART3); 

	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_Pin | USART3_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(USART3_GPIO_PORT,&GPIO_InitStructure); 

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������1

	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���2
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ClearFlag(USART3, USART_FLAG_TXE);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);//���������ж�
	

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}
void uart4_config(u32 BaudRate)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(UART4_GPIO_CLK,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��UART5ʱ��

	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(UART4_GPIO_PORT,UART4_TX_GPIO_PinSource,GPIO_AF_UART4);
	GPIO_PinAFConfig(UART4_GPIO_PORT,UART4_RX_GPIO_PinSource,GPIO_AF_UART4); 

	//USART5 TX�˿�����
	GPIO_InitStructure.GPIO_Pin = UART4_TX_GPIO_Pin | UART4_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART4_GPIO_PORT,&GPIO_InitStructure);

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������4

	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���5
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_ClearFlag(UART4, USART_FLAG_TXE);

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(UART4, USART_IT_IDLE,ENABLE);//���������ж�
	

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}
void uart5_config(u32 BaudRate)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(UART5_RX_GPIO_CLK | UART5_TX_GPIO_CLK,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//ʹ��UART5ʱ��

	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(UART5_TX_GPIO_PORT,UART5_TX_GPIO_PinSource,GPIO_AF_UART5);
	GPIO_PinAFConfig(UART5_RX_GPIO_PORT,UART5_RX_GPIO_PinSource,GPIO_AF_UART5); 

	//USART5 TX�˿�����
	GPIO_InitStructure.GPIO_Pin = UART5_TX_GPIO_Pin | UART5_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART5_TX_GPIO_PORT,&GPIO_InitStructure);

	//USART5 RX�˿�����
	GPIO_InitStructure.GPIO_Pin = UART5_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART5_RX_GPIO_PORT,&GPIO_InitStructure); 	

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART5, &USART_InitStructure); //��ʼ������5

	USART_Cmd(UART5, ENABLE);  //ʹ�ܴ���5
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_ClearFlag(UART5, USART_FLAG_TXE);

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(UART5, USART_IT_IDLE,ENABLE);//���������ж�
	

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
}

void usart6_config(u32 BaudRate)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(USART6_GPIO_CLK,ENABLE); //ʹ��GPIODʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART2ʱ��

	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(USART6_GPIO_PORT,USART6_TX_GPIO_PinSource,GPIO_AF_USART6);
	GPIO_PinAFConfig(USART6_GPIO_PORT,USART6_RX_GPIO_PinSource,GPIO_AF_USART6); 

	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = USART6_TX_GPIO_Pin | USART6_RX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(USART6_GPIO_PORT,&GPIO_InitStructure); 

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������1

	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���2
	USART_ClearFlag(USART6, USART_FLAG_TC);
	USART_ClearFlag(USART6, USART_FLAG_TXE);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);//���������ж�
	

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
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
//��ʱ��2��ʼ������Ҫ���ڼ�⴮���Ƿ������һ֡����
void TIM2_Config(u16 TIM2_Interval_xus)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;//�����ж����ȼ�
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);	
	  
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//����ʱ��ʹ��  				
  	TIM_TimeBaseStructure.TIM_Period = 8400 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler = TIM2_Interval_xus-1;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);  		
}

//��ʱ��2�ж����
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





