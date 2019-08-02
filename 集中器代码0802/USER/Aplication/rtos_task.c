//创建任务和消息队列
#include "string.h"
#include "stdio.h"
#include "rtos_task.h"
#include "common.h"
#include "task_led.h"
#include "task_lwip_init.h"
#include "stm32_config.h"
#include "task_tcp_client.h"
#include "httpd.h"
#include "lwip_comm.h" 
#include "plc.h" 
#include "zigbee.h" 
#include "rs485.h"
#include "ec20.h"
#include "rx8010s.h"
#include "spi.h"
#include "eeprom.h"

//任务优先级
#define LED0_TASK_PRIO					7
#define DATACOLLECT_TASK_PRIO				3
#define LWIP_TASK_PRIO					4
#define LWIP_DHCP_TASK_PRIO				5
#define TCP_CLIENT_TASK_PRIO			6

//任务堆栈大小	
#define LED0_STK_SIZE 					500
#define DATACOLLECT_STK_SIZE 			500
#define LWIP_STK_SIZE 					1000
#define LWIP_DHCP_STK_SIZE 				150
#define TCP_CLIENT_STK_SIZE 			150

//任务句柄
TaskHandle_t LED0Task_Handler;
TaskHandle_t DATACOLLECTTask_Handler;
TaskHandle_t LWIPTask_Handler;
TaskHandle_t LWIP_DHCPTask_Handler;
TaskHandle_t TCP_CLIENTTask_Handler;

//任务函数
void led0_task(void *pvParameters);
void data_collect_task(void *pvParameters);
void LWIP_task(void *pvParameters);
void LWIP_DHCP_task(void *pvParameters);
void TCP_CLIENT_task(void *pvParameters);

void AppTaskCreate(void)
{	
	taskENTER_CRITICAL();           //进入临界区
	//创建LED0任务
	xTaskCreate((TaskFunction_t )led0_task,     	
							(const char*    )"led0_task",   	
							(uint16_t       )LED0_STK_SIZE, 
							(void*          )NULL,				
							(UBaseType_t    )LED0_TASK_PRIO,	
							(TaskHandle_t*  )&LED0Task_Handler);   
	//创建PRINTF任务
	xTaskCreate((TaskFunction_t )data_collect_task,     
							(const char*    )"data_collect_task",   
							(uint16_t       )DATACOLLECT_STK_SIZE, 
							(void*          )NULL,
							(UBaseType_t    )DATACOLLECT_TASK_PRIO,
							(TaskHandle_t*  )&DATACOLLECTTask_Handler); 
#ifdef USE_LWIP	
	//创建LWIP任务
	xTaskCreate((TaskFunction_t )LWIP_task,     
							(const char*    )"LWIP_task",   
							(uint16_t       )LWIP_STK_SIZE, 
							(void*          )NULL,
							(UBaseType_t    )LWIP_TASK_PRIO,
							(TaskHandle_t*  )&LWIPTask_Handler); 
#endif

	taskEXIT_CRITICAL();            //退出临界区
}

//LED0任务函数 
//79 07 00 98 02 53 45 42 16 04 12 00 A7 C9 
void led0_task(void *pvParameters)
{
    uint16_t cnt=0;

	while(1)
    {
        cnt++;
		LEDR = ~LEDR;
//		if(USART1_RX_STA == 1)
//		{
//			usart1_senddata(USART1_RX_BUF,USART1_RX_CNT);
//			memset(USART1_RX_BUF,0,USART1_RX_CNT);
//			USART1_RX_STA =0;
//			USART1_RX_CNT =0;
//		}
		if(USART2_RX_STA == 1)
		{
			usart1_senddata(USART2_RX_BUF,USART2_RX_CNT);
			memset(USART2_RX_BUF,0,USART2_RX_CNT);
			USART2_RX_STA =0;
			USART2_RX_CNT =0;
		}
		if(USART3_RX_STA == 1)
		{
			debugprintf("%s\r\n",USART3_RX_BUF);
			memset(USART3_RX_BUF,0,USART3_RX_CNT);
			USART3_RX_STA =0;
			USART3_RX_CNT =0;
		}
		if(UART5_RX_STA == 1)
		{
			debugprintf("%s\r\n",UART5_RX_BUF);
			memset(UART5_RX_BUF,0,UART5_RX_CNT);
			UART5_RX_STA =0;
			UART5_RX_CNT =0;
		}
		if(USART6_RX_STA == 1)
		{
			debugprintf("%s\r\n",USART6_RX_BUF);
			memset(USART6_RX_BUF,0,USART6_RX_CNT);
			USART6_RX_STA =0;
			USART6_RX_CNT =0;
		}
        vTaskDelay(100);
    }
}   

//LED1任务函数
void data_collect_task(void *pvParameters)
{
    uint16_t cnt=0;
//	unsigned char usart2_tx_buff[10]={0x51,0,0XA5,0x58,0x03,0x5B,0x5A,9};
	char usart2_tx_buff[200]="EEPROM TEST HELLO good!55";
	 char Buf[256];
	
//	res = ec20_pwr_on();
//	if(res)
//	{
//		debugprintf("EC20开机失败\r\n");
//	}
//	else
		debugprintf("EC20开机成功\r\n");
	CAT25X_Write_Enable();
	delay_ms(10);
	while(1)
    {
//	  CAT25X_Write(0x400, 7,  usart2_tx_buff);
//	  CAT25X_Write(0x450, 7,  usart2_tx_buff);
//	  CAT25X_Read (0x400, 7, Buf);
//	  debugprintf("Buf:%s\r\n",i,Buf);
		cnt++;
		sprintf(usart2_tx_buff,"EEPROM TEST HELLO good!%d\r\n",cnt);
		CAT25X_Write(usart2_tx_buff,0x1000,strlen(usart2_tx_buff));
//		delay_ms(10);
//		CAT25X_Write_Enable();
//		CAT25X_Write_SR(0x80);  
//		delay_ms(1);
//		CAT25X_Read_SR();  
		CAT25X_Read(Buf,0x1000,strlen(usart2_tx_buff)); 
//		for(i=0;i<strlen(usart2_tx_buff);i++)
//			debugprintf("Buf[%d]=%x\r\n",i,Buf[i]);
		debugprintf("=%s\r\n",Buf);
		memset(Buf,0,128);
//		plc_send_cmd(CMD_GET_VERSION,NULL,0);
//		zigbee_send_cmd("AT+DEV=C","+OK",50);
//		usart1_senddata("AT\r\n",4);
//		rs485_senddata("AT\r\n",4);
//		ec20_senddata("ATI\r\n",5);
//		RX8010S_Get_Time();
//		debugprintf("%d-%d-%d %d-%d-%d 星期%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec,calendar.week);
		vTaskDelay(500);	
    }
}

void LWIP_task(void *pvParameters)
{
	/* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
	ETH_BSP_Config();

	/* Initilaize the LwIP stack */
	LwIP_Init();
	
	xTaskCreate((TaskFunction_t )TCP_CLIENT_task,     
						(const char*    )"TCP_CLIENT_task",   
						(uint16_t       )TCP_CLIENT_STK_SIZE, 
						(void*          )NULL,
						(UBaseType_t    )TCP_CLIENT_TASK_PRIO,
						(TaskHandle_t*  )&TCP_CLIENTTask_Handler);
	
#ifdef USE_DHCP
  /* Start DHCPClient */
	xTaskCreate((TaskFunction_t )LWIP_DHCP_task,     
						(const char*    )"LWIP_DHCP_task",   
						(uint16_t       )LWIP_DHCP_STK_SIZE, 
						(void*          )NULL,
						(UBaseType_t    )LWIP_DHCP_TASK_PRIO,
						(TaskHandle_t*  )&LWIP_DHCPTask_Handler); 
#endif

	vTaskDelete(LWIPTask_Handler);
}

//创建消息队列
void AppObjCreate(void)
{
	
}










































