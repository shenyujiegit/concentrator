/**
  ******************************************************************************
  * @file    stm32f4x7_eth_bsp.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   STM32F4x7 Ethernet hardware configuration.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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
#include "lwip/opt.h"
#include "stm32f4x7_eth.h"
#include "dp83848.h"
#include "common.h"
#include "netif.h"
#include "netconf.h"
#include "lwip/dhcp.h"
#include "lwip_comm.h"
#include "stm32_config.h"
/* Scheduler includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* The time to block waiting for input. */
#define ETH_LINK_TASK_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define ETH_LINK_TASK_PRIORITY		        ( tskIDLE_PRIORITY + 3 )
#define emacBLOCK_TIME_WAITING_ETH_LINK_IT	( ( portTickType ) 100 )
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ETH_InitTypeDef ETH_InitStructure;
__IO uint32_t  EthStatus = 0;
extern struct netif xnetif;
#ifdef USE_DHCP
extern __IO uint8_t DHCP_state;
#endif /* LWIP_DHCP */
xSemaphoreHandle ETH_link_xSemaphore = NULL;

/* Private function prototypes -----------------------------------------------*/
static void ETH_GPIO_Config(void);
static void ETH_NVIC_Config(void);
static void ETH_MACDMA_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  ETH_BSP_Config
  * @param  None
  * @retval None
  */
void ETH_BSP_Config(void)
{
  /* Configure the GPIO ports for ethernet pins */
  ETH_GPIO_Config();

  /* Config NVIC for Ethernet */
  ETH_NVIC_Config();

  /* Configure the Ethernet MAC/DMA */
  ETH_MACDMA_Config();

  /* Get Ethernet link status*/
  if(ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_SR) & 1)
  {
    EthStatus |= ETH_LINK_FLAG;
  }

  /* Configure the PHY to generate an interrupt on change of link status */
//  Eth_Link_PHYITConfig(DP83848_PHY_ADDRESS);

  /* Configure the EXTI for Ethernet link status. */
//  Eth_Link_EXTIConfig();

  if (ETH_link_xSemaphore == NULL)
  {
    /* create binary semaphore used for ETH_link handling */
    vSemaphoreCreateBinary( ETH_link_xSemaphore );
  }

  /* create the task that handles the ETH_link */
  xTaskCreate(Eth_Link_IT_task, (char*) "E_link", ETH_LINK_TASK_STACK_SIZE, (void *)DP83848_PHY_ADDRESS,
              ETH_LINK_TASK_PRIORITY,NULL);
}

/**
  * @brief  Configures the Ethernet Interface
  * @param  None
  * @retval None
  */
static void ETH_MACDMA_Config(void)
{
  /* Enable ETHERNET clock  */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
                         RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);

  /* Reset ETHERNET on AHB Bus */
  ETH_DeInit();

  /* Software reset */
  ETH_SoftwareReset();

  /* Wait for software reset */
  while (ETH_GetSoftwareResetStatus() == SET);

  /* ETHERNET Configuration --------------------------------------------------*/
  /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
  ETH_StructInit(&ETH_InitStructure);

  /* Fill ETH_InitStructure parametrs */
  /*------------------------   MAC   -----------------------------------*/
  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
//  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
//  ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
//  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;

  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
  ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

  /*------------------------   DMA   -----------------------------------*/

  /* When we use the Checksum offload feature, we need to enable the Store and Forward mode:
  the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
  if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
  ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
  ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
  ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;

  ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
  ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
  ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
  ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
  ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
  ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
  ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
  ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

  /* Configure Ethernet */
  EthStatus = ETH_Init(&ETH_InitStructure, DP83848_PHY_ADDRESS);

  /* Enable the Ethernet Rx Interrupt */
  ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void ETH_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RMII_RX_CLK_GPIO_CLK | RMII_MDIO_GPIO_CLK | RMII_RX_DV_GPIO_CLK ,ENABLE);	
	RCC_AHB1PeriphClockCmd(RMII_MDC_GPIO_CLK | RMII_RXD0_GPIO_CLK | RMII_RXD1_GPIO_CLK ,ENABLE);
	RCC_AHB1PeriphClockCmd(RMII_TX_EN_GPIO_CLK | RMII_TXD0_GPIO_CLK | RMII_TXD1_GPIO_CLK ,ENABLE);
	RCC_AHB1PeriphClockCmd(DP83848_RST_GPIO_CLK,ENABLE);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);

	/* RMII_RX_CLK PA1 pins configuration ************************************************/

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = RMII_RX_CLK_GPIO_Pin;
	GPIO_Init(RMII_RX_CLK_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_RX_CLK_GPIO_PORT, RMII_RX_CLK_GPIO_PinSource, GPIO_AF_ETH);
	
	/* RMII_MDIO PA2 pins configuration ************************************************/
	GPIO_InitStructure.GPIO_Pin = RMII_MDIO_GPIO_Pin;
	GPIO_Init(RMII_MDIO_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_MDIO_GPIO_PORT, RMII_MDIO_GPIO_PinSource, GPIO_AF_ETH);
	
	/* RMII_RX_DV PA7 pins configuration ************************************************/
	GPIO_InitStructure.GPIO_Pin = RMII_RX_DV_GPIO_Pin;
	GPIO_Init(RMII_RX_DV_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_RX_DV_GPIO_PORT, RMII_RX_DV_GPIO_PinSource, GPIO_AF_ETH);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* RMII_MDC PC1 pins configuration ************************************************/
	GPIO_InitStructure.GPIO_Pin = RMII_MDC_GPIO_Pin;
	GPIO_Init(RMII_MDC_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_MDC_GPIO_PORT, RMII_MDC_GPIO_PinSource, GPIO_AF_ETH);
	
	/* RMII_MDC PC1 pins configuration ************************************************/
	GPIO_InitStructure.GPIO_Pin = RMII_MDC_GPIO_Pin;
	GPIO_Init(RMII_MDC_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_MDC_GPIO_PORT, RMII_MDC_GPIO_PinSource, GPIO_AF_ETH);
	
	/* RMII_RXD0 PC4 pins configuration ************************************************/
	GPIO_InitStructure.GPIO_Pin = RMII_RXD0_GPIO_Pin;
	GPIO_Init(RMII_RXD0_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_RXD0_GPIO_PORT, RMII_RXD0_GPIO_PinSource, GPIO_AF_ETH);
	
	/* RMII_RXD1 PC5 pins configuration ************************************************/
	GPIO_InitStructure.GPIO_Pin = RMII_RXD1_GPIO_Pin;
	GPIO_Init(RMII_RXD1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_RXD1_GPIO_PORT, RMII_RXD1_GPIO_PinSource, GPIO_AF_ETH);
	
	/* RMII_TX_EN PG11 pins configuration ************************************************/
	GPIO_InitStructure.GPIO_Pin = RMII_TX_EN_GPIO_Pin;
	GPIO_Init(RMII_TX_EN_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_TX_EN_GPIO_PORT, RMII_TX_EN_GPIO_PinSource, GPIO_AF_ETH);
	
	/* RMII_TXD0 PG13 pins configuration ************************************************/
	GPIO_InitStructure.GPIO_Pin = RMII_TXD0_GPIO_Pin;
	GPIO_Init(RMII_TXD0_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_TXD0_GPIO_PORT, RMII_TXD0_GPIO_PinSource, GPIO_AF_ETH);
	
	/* RMII_TXD1 PG14 pins configuration ************************************************/
	GPIO_InitStructure.GPIO_Pin = RMII_TXD1_GPIO_Pin;
	GPIO_Init(RMII_TXD1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(RMII_TXD1_GPIO_PORT, RMII_TXD1_GPIO_PinSource, GPIO_AF_ETH);

	//DP83848��λ�ܽţ�PF12
	GPIO_InitStructure.GPIO_Pin = DP83848_RST_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DP83848_RST_GPIO_PORT, &GPIO_InitStructure);
	DP83848_RST =1;

	Eth_HardReset();
}

/**
  * @brief  hard reset phy.
  * @param  None
  * @retval None
  */
void Eth_HardReset(void)
{
	DP83848_RST=0;
	delay_ms(50);	
	DP83848_RST=1;
}

/**
  * @brief  Configures and enable the Ethernet global interrupt.
  * @param  None
  * @retval None
  */
void ETH_NVIC_Config(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable the Ethernet global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure the PHY to generate an interrupt on change of link status.
  * @param PHYAddress: external PHY address
  * @retval None
  */
uint32_t Eth_Link_PHYITConfig(uint16_t PHYAddress)
{
  uint16_t tmpreg = 0;

  /* Read MICR register */
  tmpreg = ETH_ReadPHYRegister(PHYAddress, PHY_MICR);

  /* Enable output interrupt events to signal via the INT pin */
  tmpreg |= (uint32_t)PHY_MICR_INT_EN | PHY_MICR_INT_OE;
  if(!(ETH_WritePHYRegister(PHYAddress, PHY_MICR, tmpreg)))
  {
    /* Return ERROR in case of write timeout */
    return ETH_ERROR;
  }

  /* Read MISR register */
  tmpreg = ETH_ReadPHYRegister(PHYAddress, PHY_MISR);

  /* Enable Interrupt on change of link status */
  tmpreg |= (uint32_t)PHY_MISR_LINK_INT_EN;
  if(!(ETH_WritePHYRegister(PHYAddress, PHY_MISR, tmpreg)))
  {
    /* Return ERROR in case of write timeout */
    return ETH_ERROR;
  }
  /* Return SUCCESS */
  return ETH_SUCCESS;
}

/**
  * @brief  EXTI configuration for Ethernet link status.
  * @param PHYAddress: external PHY address
  * @retval None
  */
void Eth_Link_EXTIConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the INT (PB14) Clock */
  RCC_AHB1PeriphClockCmd(ETH_LINK_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure INT pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = ETH_LINK_PIN;
  GPIO_Init(ETH_LINK_GPIO_PORT, &GPIO_InitStructure);

  /* Connect EXTI Line to INT Pin */
  SYSCFG_EXTILineConfig(ETH_LINK_EXTI_PORT_SOURCE, ETH_LINK_EXTI_PIN_SOURCE);

  /* Configure EXTI line */
  EXTI_InitStructure.EXTI_Line = ETH_LINK_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set the EXTI interrupt to priority 1*/
  NVIC_InitStructure.NVIC_IRQChannel = ETH_LINK_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
* @brief  This function handles Ethernet link status.
* @param  None
* @retval None
*/
void Eth_Link_IT_task( void * pvParameters )
{
  uint32_t pcPHYAddress;

  pcPHYAddress = ( uint32_t  ) pvParameters;

  for(;;)
  {
    if (xSemaphoreTake( ETH_link_xSemaphore, emacBLOCK_TIME_WAITING_ETH_LINK_IT)==pdTRUE)
    {
      /* Check whether the link interrupt has occurred or not */
      if(((ETH_ReadPHYRegister((uint16_t) pcPHYAddress, PHY_MISR)) & PHY_LINK_STATUS) != 0)
      {
        if((ETH_ReadPHYRegister((uint16_t) pcPHYAddress, PHY_SR) & 1))
        {
          netif_set_link_up(&xnetif);
        }
        else
        {
          netif_set_link_down(&xnetif);
        }
      }
    }
  }
}
/**
  * @brief  Link callback function, this function is called on change of link status.
  * @param  The network interface
  * @retval None
  */
void ETH_link_callback(struct netif *netif)
{
  __IO uint32_t timeout = 0;
 uint32_t tmpreg,RegValue;

  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
#ifndef USE_DHCP
  uint8_t iptab[4] = {0};
  uint8_t iptxt[20];
#endif /* USE_DHCP */


  if(netif_is_link_up(netif))
  {
    /* Restart the autonegotiation */
    if(ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
    {
      /* Reset Timeout counter */
      timeout = 0;

      /* Enable Auto-Negotiation */
      ETH_WritePHYRegister(DP83848_PHY_ADDRESS, PHY_BCR, PHY_AutoNegotiation);

      /* Wait until the auto-negotiation will be completed */
      do
      {
        timeout++;
      } while (!(ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (uint32_t)PHY_READ_TO));

      /* Reset Timeout counter */
      timeout = 0;

      /* Read the result of the auto-negotiation */
      RegValue = ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_SR);

      /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
      if((RegValue & PHY_DUPLEX_STATUS) != (uint32_t)RESET)
      {
        /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
        ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
      }
      else
      {
        /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
        ETH_InitStructure.ETH_Mode = ETH_Mode_HalfDuplex;
      }
      /* Configure the MAC with the speed fixed by the auto-negotiation process */
      if(RegValue & PHY_SPEED_STATUS)
      {
        /* Set Ethernet speed to 10M following the auto-negotiation */
        ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
      }
      else
      {
        /* Set Ethernet speed to 100M following the auto-negotiation */
        ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
      }

      /*------------------------ ETHERNET MACCR Re-Configuration --------------------*/
      /* Get the ETHERNET MACCR value */
      tmpreg = ETH->MACCR;

      /* Set the FES bit according to ETH_Speed value */
      /* Set the DM bit according to ETH_Mode value */
      tmpreg |= (uint32_t)(ETH_InitStructure.ETH_Speed | ETH_InitStructure.ETH_Mode);

      /* Write to ETHERNET MACCR */
      ETH->MACCR = (uint32_t)tmpreg;

      _eth_delay_(ETH_REG_WRITE_DELAY);
      tmpreg = ETH->MACCR;
      ETH->MACCR = tmpreg;
    }

    /* Restart MAC interface */
    ETH_Start();

#ifdef USE_DHCP
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;

    DHCP_state = DHCP_START;
#else
    IP4_ADDR(&ipaddr, lwipdev.ip[0], lwipdev.ip[1], lwipdev.ip[2], lwipdev.ip[3]);
    IP4_ADDR(&netmask, lwipdev.netmask[0], lwipdev.netmask[1] , lwipdev.netmask[2], lwipdev.netmask[3]);
    IP4_ADDR(&gw, lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
#endif /* USE_DHCP */

    netif_set_addr(&xnetif, &ipaddr , &netmask, &gw);

    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&xnetif);
  }
  else
  {
    ETH_Stop();
#ifdef USE_DHCP
    DHCP_state = DHCP_LINK_DOWN;
    dhcp_stop(netif);

#endif /* USE_DHCP */

    /*  When the netif link is down this function must be called.*/
    netif_set_down(&xnetif);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
