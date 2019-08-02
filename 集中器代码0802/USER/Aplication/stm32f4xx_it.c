/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm32f4x7_eth.h"

#include "lwip/lwip_sys.h"
#include "dp83848.h"
#include "common.h"
#include "usart.h"
 

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

extern xSemaphoreHandle s_xSemaphore;
extern xSemaphoreHandle ETH_link_xSemaphore;


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	char Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)//接收中断标志位
	{
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		Res = USART_ReceiveData(USART1);
		USART1_RX_BUF[USART1_RX_CNT++] = Res;
		if(USART1_RX_CNT >= MAX_USART1_RX_LEN) 
			USART1_RX_CNT = 0;
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)//空闲帧标志位
	{
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);         //清除中断标志
		Res=USART1->SR;
		Res = USART_ReceiveData(USART1);		 
		USART_ClearFlag(USART1,USART_FLAG_IDLE);//清楚空闲标志位
		
		USART1_RX_STA = 1;
	}
}

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	char Res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//接收中断标志位
	{
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
		Res = USART_ReceiveData(USART2);
		USART2_RX_BUF[USART2_RX_CNT++] = Res;
		if(USART2_RX_CNT >= MAX_USART2_RX_LEN) 
			USART2_RX_CNT = 0;
	}
	if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET)//空闲帧标志位
	{
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);         //清除中断标志
		Res=USART2->SR;
		Res = USART_ReceiveData(USART2);		 
		USART_ClearFlag(USART2,USART_FLAG_IDLE);//清楚空闲标志位
		
		USART2_RX_STA = 1;
	}
}

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	char Res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//接收中断标志位
	{
		USART_ClearFlag(USART3,USART_FLAG_RXNE);
		Res = USART_ReceiveData(USART3);
		USART3_RX_BUF[USART3_RX_CNT++] = Res;
		if(USART3_RX_CNT >= MAX_USART3_RX_LEN) 
			USART3_RX_CNT = 0;
	}
	if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET)//空闲帧标志位
	{
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);         //清除中断标志
		Res=USART3->SR;
		Res = USART_ReceiveData(USART3);		 
		USART_ClearFlag(USART3,USART_FLAG_IDLE);//清楚空闲标志位
		
		USART3_RX_STA = 1;
	}
}
void UART4_IRQHandler(void)                	//串口1中断服务程序
{
	char Res;
	if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)//接收中断标志位
	{
		USART_ClearFlag(UART4,USART_FLAG_RXNE);
		Res = USART_ReceiveData(UART4);
		UART4_RX_BUF[UART4_RX_CNT++] = Res;
		if(UART4_RX_CNT >= MAX_UART4_RX_LEN) 
			UART4_RX_CNT = 0;
	}
	if(USART_GetITStatus(UART4,USART_IT_IDLE)!=RESET)//空闲帧标志位
	{
		USART_ClearITPendingBit(UART4, USART_IT_IDLE);         //清除中断标志
		Res=UART4->SR;
		Res = USART_ReceiveData(UART4);		 
		USART_ClearFlag(UART4,USART_FLAG_IDLE);//清楚空闲标志位
		
		UART4_RX_STA = 1;
	}
}
void UART5_IRQHandler(void)                	//串口1中断服务程序
{
	char Res;
	if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)//接收中断标志位
	{
		USART_ClearFlag(UART5,USART_FLAG_RXNE);
		Res = USART_ReceiveData(UART5);
		UART5_RX_BUF[UART5_RX_CNT++] = Res;
		if(UART5_RX_CNT >= MAX_UART5_RX_LEN) 
			UART5_RX_CNT = 0;
	}
	if(USART_GetITStatus(UART5,USART_IT_IDLE)!=RESET)//空闲帧标志位
	{
		USART_ClearITPendingBit(UART5, USART_IT_IDLE);         //清除中断标志
		Res=UART5->SR;
		Res = USART_ReceiveData(UART5);		 
		USART_ClearFlag(UART5,USART_FLAG_IDLE);//清楚空闲标志位
		
		UART5_RX_STA = 1;
	}
}
void USART6_IRQHandler(void)                	//串口1中断服务程序
{
	char Res;
	if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET)//接收中断标志位
	{
		USART_ClearFlag(USART6,USART_FLAG_RXNE);
		Res = USART_ReceiveData(USART6);
		USART6_RX_BUF[USART6_RX_CNT++] = Res;
		if(USART6_RX_CNT >= MAX_USART6_RX_LEN) 
			USART6_RX_CNT = 0;
	}
	if(USART_GetITStatus(USART6,USART_IT_IDLE)!=RESET)//空闲帧标志位
	{
		USART_ClearITPendingBit(USART6, USART_IT_IDLE);         //清除中断标志
		Res=USART6->SR;
		Res = USART_ReceiveData(USART6);		 
		USART_ClearFlag(USART6,USART_FLAG_IDLE);//清楚空闲标志位
		
		USART6_RX_STA = 1;
	}
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

///**
//  * @brief  This function handles SVCall exception.
//  * @param  None
//  * @retval None
//  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

///**
//  * @brief  This function handles PendSVC exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{
//}

///**
//  * @brief  This function handles SysTick Handler.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
// 
//}

/**
  * @brief  This function handles External line 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
  
  if(EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
  {
  /* Give the semaphore to wakeup LwIP task */
  xSemaphoreGiveFromISR( ETH_link_xSemaphore, &xHigherPriorityTaskWoken ); 
  }
   /* Clear interrupt pending bit */
   EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);
  
    /* Switch tasks if necessary. */	
  if( xHigherPriorityTaskWoken != pdFALSE )
  {
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
  }
}

/**
  * @brief  This function handles ethernet DMA interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler()
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	/// give away taked semaphore. Used to inform the ethernet_input
	/// thread that new data is available
	/// give away only if frame received interrupt was triggered
	if(ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R) == SET)
	{
		xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken );
		ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
	}
	if(ETH_GetDMAFlagStatus(ETH_DMA_FLAG_T)==SET)
	{
		xHigherPriorityTaskWoken = pdFALSE;
	}
	if(ETH_GetDMAFlagStatus(ETH_DMA_FLAG_TBU)==SET)
	{
		xHigherPriorityTaskWoken = pdFALSE;
	}
	
	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
	
//	if( xHigherPriorityTaskWoken != pdFALSE )
//	{
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
//	}
}

//void ETH_IRQHandler()
//{
//	long xHigherPriorityTaskWoken = pdFALSE;
//	/// give away taked semaphore. Used to inform the ethernet_input
//	/// thread that new data is available
//	/// give away only if frame received interrupt was triggered
//	if(ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R) == SET)
//	{
//		xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken );
//		ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
//	}
//	if(ETH_GetDMAFlagStatus(ETH_DMA_FLAG_T)==SET)
//	{
//		xHigherPriorityTaskWoken = pdFALSE;
//	}
//	if(ETH_GetDMAFlagStatus(ETH_DMA_FLAG_TBU)==SET)
//	{
//		xHigherPriorityTaskWoken = pdFALSE;
//	}
//	ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
//	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
//	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
