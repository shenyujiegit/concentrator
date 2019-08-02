#ifndef _PLC_H
#define _PLC_H
#include "sys.h"

typedef struct
{
u8 fh;
u16  len;
u8  ctrl;
u8  cmd;
u8  data[128];
u8	csum;
u8	cxor;
}plc_at_info;

extern plc_at_info ps1642_cmd;

/*********************** ES1642 CMD **************************/
//本地操作指令
#define	CMD_MODULE_RST		0x01
#define	CMD_GET_VERSION		0x02
#define	CMD_GET_MAC			0x03
#define	CMD_GET_ADDR		0x0B
#define	CMD_SET_MAC			0x0C
#define	CMD_GET_PARA		0x0D
#define	CMD_SET_PARA		0x0E
//信道操作指令
#define	CMD_SEND_DATA		0x14//发送数据
#define	CMD_RECV_DATA		0x15//接收数据
#define	CMD_SEARCH_DEV		0x17//搜索设备
#define	CMD_SEARCH_DEV_OFF		0x18//停止搜索设备
#define	CMD_SEARCH_DEV_UP		0x19//上报搜索结果
#define	CMD_SEARCH_DEV_TELL		0x1A//通知设备搜索
#define	CMD_SEARCH_DEV_RESP		0x1B//响应设备搜索
#define	CMD_SET_NETPARA			0x1C//设置网络口令
#define	CMD_SET_NETPARA_TELL	0x1D//通知网络口令设置
#define	CMD_SET_NETPARA_UP		0x1F//设置网络口令设置结果
#define	CMD_RGET_VERSION		0x52//远程读取版本信息
#define	CMD_RGET_MAC			0x03//远程读取MAC
#define	CMD_RGET_PARA			0x0D//远程读取网络参数

void plc_config(u32 BaudRate);
u8 at_csum(char *buf,u8 len);
u8 at_cxor(char *buf,u8 len);
void plc_send_cmd(u8 cmd,u8 *buff,u8 len);


#endif
