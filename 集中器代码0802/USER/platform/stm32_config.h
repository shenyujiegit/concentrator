#ifndef _STM32_CONFIG_H
#define _STM32_CONFIG_H
#include "sys.h" 
#include "stm32f4xx.h" 

/********************************调试开关*************************************/
#define _DEBUG 1
     
#if _DEBUG
#define debugprintf(format,...)  printf(format, ##__VA_ARGS__)
#else  
#define debugprintf(format,...)
#endif

/*************************** 定义LED连接的GPIO端口 ****************************/
#define LEDR_GPIO_PORT         			GPIOA                          
#define LEDR_GPIO_Pin          			GPIO_Pin_3                     
#define LEDR_GPIO_CLK         			RCC_AHB1Periph_GPIOA            
#define LEDR                   			PAout(3)

/*************************** 定义RELAY连接的GPIO端口 ****************************/
#define RELAY1_GPIO_PORT        		GPIOE                          
#define RELAY1_GPIO_Pin         		GPIO_Pin_10                     
#define RELAY1_GPIO_CLK         		RCC_AHB1Periph_GPIOE            
#define RELAY1                  		PEout(10)

#define RELAY2_GPIO_PORT        		GPIOE                          
#define RELAY2_GPIO_Pin         		GPIO_Pin_11                     
#define RELAY2_GPIO_CLK         		RCC_AHB1Periph_GPIOE            
#define RELAY2                  		PEout(11)

#define RELAY3_GPIO_PORT        		GPIOE                          
#define RELAY3_GPIO_Pin         		GPIO_Pin_12                     
#define RELAY3_GPIO_CLK         		RCC_AHB1Periph_GPIOE            
#define RELAY3                  		PEout(12)

#define RELAY4_GPIO_PORT        		GPIOE                          
#define RELAY4_GPIO_Pin         		GPIO_Pin_13                     
#define RELAY4_GPIO_CLK         		RCC_AHB1Periph_GPIOE            
#define RELAY4                  		PEout(13)

#define RELAY5_GPIO_PORT        		GPIOE                          
#define RELAY5_GPIO_Pin         		GPIO_Pin_14                     
#define RELAY5_GPIO_CLK         		RCC_AHB1Periph_GPIOE            
#define RELAY5                  		PEout(14)

#define RELAY6_GPIO_PORT        		GPIOE                          
#define RELAY6_GPIO_Pin         		GPIO_Pin_15                     
#define RELAY6_GPIO_CLK         		RCC_AHB1Periph_GPIOE            
#define RELAY6                  		PEout(15)

#define PLCRST_GPIO_PORT        		GPIOD                         
#define PLCRST_GPIO_Pin         		GPIO_Pin_4                     
#define PLCRST_GPIO_CLK         		RCC_AHB1Periph_GPIOD            
#define PLCRST                  		PDout(4)

#define ZIGBEE_GPIO_PORT        		GPIOG                         
#define ZIGBEE_GPIO_Pin         		GPIO_Pin_3                     
#define ZIGBEE_GPIO_CLK         		RCC_AHB1Periph_GPIOG           
#define ZIGBEERST               		PGout(3)

#define ZIGBEEEN_GPIO_PORT      		GPIOD                        
#define ZIGBEEEN_GPIO_Pin       		GPIO_Pin_13                     
#define ZIGBEEEN_GPIO_CLK       		RCC_AHB1Periph_GPIOD           
#define ZIGBEEEN               			PDout(13)

/******************************** 定义USART连接的GPIO端口 *************************************/
#define USART1_GPIO_PORT      			GPIOA                          
#define USART1_TX_GPIO_Pin      		GPIO_Pin_9
#define USART1_TX_GPIO_PinSource        GPIO_PinSource9   
#define USART1_RX_GPIO_Pin      		GPIO_Pin_10 
#define USART1_RX_GPIO_PinSource        GPIO_PinSource10 
#define USART1_GPIO_CLK       			RCC_AHB1Periph_GPIOA  

#define USART2_GPIO_PORT      			GPIOD                          
#define USART2_TX_GPIO_Pin      		GPIO_Pin_5
#define USART2_TX_GPIO_PinSource        GPIO_PinSource5   
#define USART2_RX_GPIO_Pin      		GPIO_Pin_6 
#define USART2_RX_GPIO_PinSource        GPIO_PinSource6 
#define USART2_GPIO_CLK       			RCC_AHB1Periph_GPIOD  

#define USART3_GPIO_PORT      			GPIOD                          
#define USART3_TX_GPIO_Pin      		GPIO_Pin_8
#define USART3_TX_GPIO_PinSource        GPIO_PinSource8   
#define USART3_RX_GPIO_Pin      		GPIO_Pin_9 
#define USART3_RX_GPIO_PinSource        GPIO_PinSource9 
#define USART3_GPIO_CLK       			RCC_AHB1Periph_GPIOD  

#define UART4_GPIO_PORT      			GPIOC                          
#define UART4_TX_GPIO_Pin      			GPIO_Pin_10
#define UART4_TX_GPIO_PinSource         GPIO_PinSource10   
#define UART4_RX_GPIO_Pin      			GPIO_Pin_11 
#define UART4_RX_GPIO_PinSource         GPIO_PinSource11 
#define UART4_GPIO_CLK       			RCC_AHB1Periph_GPIOC 

#define UART5_TX_GPIO_PORT      		GPIOC                          
#define UART5_TX_GPIO_Pin      			GPIO_Pin_12
#define UART5_TX_GPIO_PinSource         GPIO_PinSource12  
#define UART5_TX_GPIO_CLK       		RCC_AHB1Periph_GPIOC
#define UART5_RX_GPIO_PORT      		GPIOD
#define UART5_RX_GPIO_Pin      			GPIO_Pin_2 
#define UART5_RX_GPIO_PinSource         GPIO_PinSource2
#define UART5_RX_GPIO_CLK       		RCC_AHB1Periph_GPIOD



#define RS485_DIR_GPIO_PORT      		GPIOD
#define RS485_DIR_GPIO_Pin      		GPIO_Pin_15 
#define RS485_DIR_GPIO_CLK       		RCC_AHB1Periph_GPIOD 
#define	RS485_DIR						PDout(15)

/********************************** 定义EC20 4G模块接口 **************************************/
#define USART6_GPIO_PORT      			GPIOC                          
#define USART6_TX_GPIO_Pin      		GPIO_Pin_6
#define USART6_TX_GPIO_PinSource        GPIO_PinSource6   
#define USART6_RX_GPIO_Pin      		GPIO_Pin_7 
#define USART6_RX_GPIO_PinSource        GPIO_PinSource7 
#define USART6_GPIO_CLK       			RCC_AHB1Periph_GPIOC 

#define EC20_PWRKEY_GPIO_PORT        	GPIOC                          
#define EC20_PWRKEY_GPIO_Pin         	GPIO_Pin_8                     
#define EC20_PWRKEY_GPIO_CLK         	RCC_AHB1Periph_GPIOC            
#define EC20_PWRKEY                  	PCout(8)

#define EC20_RESET_GPIO_PORT        	GPIOC                          
#define EC20_RESET_GPIO_Pin         	GPIO_Pin_0                     
#define EC20_RESET_GPIO_CLK         	RCC_AHB1Periph_GPIOC            
#define EC20_RESET                  	PCout(0)

#define EC20_STATUS_GPIO_PORT        	GPIOC                          
#define EC20_STATUS_GPIO_Pin         	GPIO_Pin_2                     
#define EC20_STATUS_GPIO_CLK         	RCC_AHB1Periph_GPIOC            
#define EC20_STATUS                  	PCin(2)

#define EC20_NETMOD_GPIO_PORT        	GPIOC                          
#define EC20_NETMOD_GPIO_Pin         	GPIO_Pin_3                     
#define EC20_NETMOD_GPIO_CLK         	RCC_AHB1Periph_GPIOC            
#define EC20_NETMOD                  	PCin(3)


/******************************** 定义RMII连接的GPIO端口 *************************************/
#define RMII_RX_CLK_GPIO_PORT       	GPIOA                          
#define RMII_RX_CLK_GPIO_Pin        	GPIO_Pin_1                     
#define RMII_RX_CLK_GPIO_CLK        	RCC_AHB1Periph_GPIOA 
#define RMII_RX_CLK_GPIO_PinSource      GPIO_PinSource1   
#define RMII_RX_CLK                 	PAout(1)

#define RMII_MDIO_GPIO_PORT        		GPIOA                          
#define RMII_MDIO_GPIO_Pin         		GPIO_Pin_2                     
#define RMII_MDIO_GPIO_CLK         		RCC_AHB1Periph_GPIOA 
#define RMII_MDIO_GPIO_PinSource        GPIO_PinSource2
#define RMII_MDIO                  		PAout(2)

#define RMII_RX_DV_GPIO_PORT        	GPIOA                          
#define RMII_RX_DV_GPIO_Pin         	GPIO_Pin_7                     
#define RMII_RX_DV_GPIO_CLK         	RCC_AHB1Periph_GPIOA 
#define RMII_RX_DV_GPIO_PinSource       GPIO_PinSource7
#define RMII_RX_DV                  	PAout(7)

#define RMII_MDC_GPIO_PORT        		GPIOC                          
#define RMII_MDC_GPIO_Pin         		GPIO_Pin_1                     
#define RMII_MDC_GPIO_CLK         		RCC_AHB1Periph_GPIOC 
#define RMII_MDC_GPIO_PinSource         GPIO_PinSource1
#define RMII_MDC                  		PCout(1)

#define RMII_RXD0_GPIO_PORT        		GPIOC                          
#define RMII_RXD0_GPIO_Pin         		GPIO_Pin_4                     
#define RMII_RXD0_GPIO_CLK         		RCC_AHB1Periph_GPIOC 
#define RMII_RXD0_GPIO_PinSource        GPIO_PinSource4
#define RMII_RXD0                  		PCout(4)

#define RMII_RXD1_GPIO_PORT        		GPIOC                          
#define RMII_RXD1_GPIO_Pin         		GPIO_Pin_5                     
#define RMII_RXD1_GPIO_CLK         		RCC_AHB1Periph_GPIOC 
#define RMII_RXD1_GPIO_PinSource        GPIO_PinSource5
#define RMII_RXD1                  		PCout(5)

#define RMII_TX_EN_GPIO_PORT        	GPIOG                          
#define RMII_TX_EN_GPIO_Pin         	GPIO_Pin_11                     
#define RMII_TX_EN_GPIO_CLK         	RCC_AHB1Periph_GPIOG 
#define RMII_TX_EN_GPIO_PinSource       GPIO_PinSource11
#define RMII_TX_EN                  	PGout(11)

#define RMII_TXD0_GPIO_PORT        		GPIOG                          
#define RMII_TXD0_GPIO_Pin         		GPIO_Pin_13                     
#define RMII_TXD0_GPIO_CLK         		RCC_AHB1Periph_GPIOG 
#define RMII_TXD0_GPIO_PinSource        GPIO_PinSource13
#define RMII_TXD0                  		PGout(13)

#define RMII_TXD1_GPIO_PORT        		GPIOG                          
#define RMII_TXD1_GPIO_Pin         		GPIO_Pin_14                     
#define RMII_TXD1_GPIO_CLK         		RCC_AHB1Periph_GPIOG 
#define RMII_TXD1_GPIO_PinSource        GPIO_PinSource14
#define RMII_TXD1                 		PGout(14)

#define DP83848_RST_GPIO_PORT        	GPIOF                          
#define DP83848_RST_GPIO_Pin         	GPIO_Pin_12                     
#define DP83848_RST_GPIO_CLK         	RCC_AHB1Periph_GPIOF 
#define DP83848_RST_GPIO_PinSource      GPIO_PinSource12
#define DP83848_RST                 	PFout(12)

/*************************** IIC *******************************/
#define IIC_GPIO_PORT              	 	GPIOB
#define IIC_SCL_GPIO_Pin             	GPIO_Pin_8
#define IIC_SDA_GPIO_Pin             	GPIO_Pin_9
#define IIC_GPIO_CLK                 	RCC_AHB1Periph_GPIOB
#define IIC_SCL                      	PBout(8)
#define IIC_SDA               		 	PBout(9)
#define IIC_SCL_H					 	(PBout(8)=1)
#define IIC_SCL_L					 	(PBout(8)=0)
#define IIC_SDA_H					 	(PBout(9)=1)
#define IIC_SDA_L					 	(PBout(9)=0)
#define IIC_SDA_IN					 	PBin(9)


/******************************** 定义CAT2564连接的GPIO端口 *************************************/
#define SPI2_GPIO_PORT               	GPIOB
#define SPI2_SCK_GPIO_Pin            	GPIO_Pin_13
#define SPI2_MISO_GPIO_Pin           	GPIO_Pin_14
#define SPI2_MOSI_GPIO_Pin           	GPIO_Pin_15
#define SPI2_SCK_GPIO_PinSource      	GPIO_PinSource13
#define SPI2_MISO_GPIO_PinSource     	GPIO_PinSource14
#define SPI2_MOSI_GPIO_PinSource     	GPIO_PinSource15
#define SPI2_GPIO_CLK                	RCC_AHB1Periph_GPIOB

#define CAT25X_CS_GPIO_PORT          	GPIOB
#define CAT25X_CS_GPIO_Pin           	GPIO_Pin_12
#define CAT25X_CS_GPIO_CLK           	RCC_AHB1Periph_GPIOB
#define CAT25X_CS                    	PBout(12)
#define SPI2_SCK                     	PBout(13)
#define SPI2_MISO                    	PBout(14)
#define SPI2_MOSI                    	PBout(15)










#endif
