#include "task_tcp_client.h"
#include "lwip/opt.h"
#include "lwip/lwip_sys.h"
#include "lwip/api.h"
#include "lwip_comm.h"
#include "netconf.h"
#include "common.h"
#include "stm32_config.h"


struct netconn *tcp_clientconn;					//TCP CLIENT网络连接结构体
u8 tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];	//TCP客户端接收数据缓冲区
u8 tcp_client_sendbuf[TCP_CLIENT_TX_BUFSIZE];	//TCP客户端发送数据缓冲区
u8 tcp_client_flag;		//TCP客户端数据发送标志位

TaskHandle_t xHandleTaskTCP_CLIENT = NULL;

void TCP_CLIENT_task(void *pvParameters)
{
	u32 data_len = 0;
	struct pbuf *q;
	err_t err,recv_err;
	static ip_addr_t server_ipaddr,loca_ipaddr;
	static u16_t 		 server_port,loca_port;
	char *tcpclicentbuf="TCP CLIENT TEST!";
	
	time_t time_heart_beat = SysTick1s;
	u8 heart_beat[24] = {0x68 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x04 ,0x68 ,0xE1 
,0x06 ,0x9C ,0xA5 ,0x25 ,0x8D ,0xEE ,0x46 ,0xE2 ,0x16 ,0xFE ,0xFD ,0xFC ,0xFB ,0xFA ,0xF9};

	server_port = REMOTE_PORT;
	IP4_ADDR(&server_ipaddr, lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]);

	while (1)
	{
#ifdef USE_DHCP
		WAIT_DHCP:
		if(DHCP_state != DHCP_ADDRESS_ASSIGNED &&
		   DHCP_state != DHCP_TIMEOUT)
		{
			delay_ms(500);
			
			goto WAIT_DHCP;
		}
#endif
		tcp_clientconn = netconn_new(NETCONN_TCP);  //创建一个TCP链接
		err = netconn_connect(tcp_clientconn,&server_ipaddr,server_port);//连接服务器
		if(err != ERR_OK)  netconn_delete(tcp_clientconn); //返回值不等于ERR_OK,删除tcp_clientconn连接
		else if (err == ERR_OK)    //处理新连接的数据
		{
			struct netbuf *recvbuf;
			tcp_clientconn->recv_timeout = 10;
			netconn_getaddr(tcp_clientconn,&loca_ipaddr,&loca_port,1); //获取本地IP主机IP地址和端口号
			printf("连接上服务器%d.%d.%d.%d,本机端口号为:%d\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3],loca_port);
			while(1)
			{
					
				tcp_client_flag |= LWIP_SEND_DATA;
				data_len = strlen(tcpclicentbuf);
				if(SysTick1s - time_heart_beat >= 30)
				{
					time_heart_beat = SysTick1s;
					
					memcpy(tcpclicentbuf,heart_beat,24);
					
					data_len = 24;
				
					tcp_client_flag |= LWIP_SEND_DATA;
				}
//				}
//				
				if((tcp_client_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //有数据要发送
				{
					err = netconn_write(tcp_clientconn ,tcpclicentbuf,data_len,NETCONN_COPY); //发送tcp_server_sentbuf中的数据
					if(err != ERR_OK)
					{
						printf("发送失败\r\n");
					}
					tcp_client_flag &= ~LWIP_SEND_DATA;
					
					data_len=0;
				}

				if((recv_err = netconn_recv(tcp_clientconn,&recvbuf)) == ERR_OK)  //接收到数据
				{
					memset(tcp_client_recvbuf,0,TCP_CLIENT_RX_BUFSIZE);  //数据接收缓冲区清零
					for(q=recvbuf->p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
					{
						//判断要拷贝到TCP_CLIENT_RX_BUFSIZE中的数据是否大于TCP_CLIENT_RX_BUFSIZE的剩余空间，如果大于
						//的话就只拷贝TCP_CLIENT_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
						if(q->len > (TCP_CLIENT_RX_BUFSIZE-data_len)) memcpy(tcp_client_recvbuf+data_len,q->payload,(TCP_CLIENT_RX_BUFSIZE-data_len));//拷贝数据
						else memcpy(tcp_client_recvbuf+data_len,q->payload,q->len);
						data_len += q->len;
						if(data_len > TCP_CLIENT_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出
					}

					debugprintf("%s\r\n",tcp_client_recvbuf);
				

					data_len = 0;
					
					netbuf_delete(recvbuf);
//					tcp_client_flag |= LWIP_SEND_DATA;
				}else if(recv_err == ERR_CLSD)  //关闭连接
				{
					netconn_close(tcp_clientconn);
					netconn_delete(tcp_clientconn);
					debugprintf("服务器%d.%d.%d.%d断开连接\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]);
					break;
				}
				vTaskDelay(1000);
			}
		}
			
	}
}














