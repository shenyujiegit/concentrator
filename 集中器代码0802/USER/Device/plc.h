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
//���ز���ָ��
#define	CMD_MODULE_RST		0x01
#define	CMD_GET_VERSION		0x02
#define	CMD_GET_MAC			0x03
#define	CMD_GET_ADDR		0x0B
#define	CMD_SET_MAC			0x0C
#define	CMD_GET_PARA		0x0D
#define	CMD_SET_PARA		0x0E
//�ŵ�����ָ��
#define	CMD_SEND_DATA		0x14//��������
#define	CMD_RECV_DATA		0x15//��������
#define	CMD_SEARCH_DEV		0x17//�����豸
#define	CMD_SEARCH_DEV_OFF		0x18//ֹͣ�����豸
#define	CMD_SEARCH_DEV_UP		0x19//�ϱ��������
#define	CMD_SEARCH_DEV_TELL		0x1A//֪ͨ�豸����
#define	CMD_SEARCH_DEV_RESP		0x1B//��Ӧ�豸����
#define	CMD_SET_NETPARA			0x1C//�����������
#define	CMD_SET_NETPARA_TELL	0x1D//֪ͨ�����������
#define	CMD_SET_NETPARA_UP		0x1F//��������������ý��
#define	CMD_RGET_VERSION		0x52//Զ�̶�ȡ�汾��Ϣ
#define	CMD_RGET_MAC			0x03//Զ�̶�ȡMAC
#define	CMD_RGET_PARA			0x0D//Զ�̶�ȡ�������

void plc_config(u32 BaudRate);
u8 at_csum(char *buf,u8 len);
u8 at_cxor(char *buf,u8 len);
void plc_send_cmd(u8 cmd,u8 *buff,u8 len);


#endif
