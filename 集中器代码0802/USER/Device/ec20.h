#ifndef _EC20_H
#define _EC20_H
#include "sys.h"


void ec20_config(u32 BaudRate);
u8 ec20_pwr_on(void);
void ec20_senddata(char* buff,u8 len);
#endif
