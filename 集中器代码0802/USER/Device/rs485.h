#ifndef _RS485_H
#define _RS485_H
#include "sys.h"

void rs485_config(u32 BaudRate);
void rs485_senddata(char* buff,u8 len);
#endif
