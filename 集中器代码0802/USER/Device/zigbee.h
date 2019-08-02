#ifndef _ZIGBEE_H
#define _ZIGBEE_H
#include "sys.h"

void zigbee_init(u32 BaudRate);
int8_t zigbee_send_cmd(char *cmd,char *respond,u16 times);
#endif
