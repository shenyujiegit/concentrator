#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtos_task.h"
#include "watch_dog.h"
#include "led.h"
#include "plc.h"
#include "zigbee.h"
#include "rs485.h"
#include "ec20.h"
#include "spi.h"
#include "rx8010s.h"
#include "stm32_config.h"
#include "eeprom.h"
#include "relay.h" 


//u8 eprombuf[100];
//u8 i = 0;
RCC_ClocksTypeDef RCC_Clocks;
int main(void)
{
	RCC_GetClocksFreq(&RCC_Clocks);
//	IWDG_Init(6,1000);									//���Ź���ʱ8��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		//����ϵͳ�ж����ȼ�����ţ�4����Ӧ���ȼ������飬��ռ���ȼ���16������
	delay_init(168);									//��ʼ����ʱ����
	
	LED_Init();
	RELAY_Init();
//	TIM2_Config(100);									//��ʱ��2��ʼ��
	usart1_config(115200);								//ά�����ڳ�ʼ��
	plc_config(9600);
	zigbee_init(115200);
	rs485_config(115200);
	ec20_config(115200);
	RX8010S_Init();
	CAT25X_Init();

	IWDG_Feed_Thread();									//�ϵ��һ��ι���Ź�

	AppObjCreate();										//������Ϣ����
	AppTaskCreate();									//��������

	vTaskStartScheduler();          					//�����������
}














