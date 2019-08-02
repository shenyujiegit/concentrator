#ifndef _EEPROM_H
#define _EEPROM_H
#include "sys.h"

//÷∏¡Ó±Ì
#define EEPROM_WRITE_EN		0x06
#define EEPROM_WRITE_DIS	0x04
#define EEPROM_READ_SR		0x05
#define EEPROM_WRITE_SR		0x01
#define EEPROM_READ_MMY		0x03
#define EEPROM_WRITE_MMY	0x02

#define PAGE_SZ         	64


void CAT25X_Init(void);
void CAT25X_Write_Enable(void);
void CAT25X_Write_Disenable(void);
void CAT25X_Write_SR(u8 stat);
u8 CAT25X_Read_SR(void);
void CAT25X_Read(char* Buff,u16 ReadAddr,u16 Num);
void CAT25X_Write(char* Buff,u16 ReadAddr,u16 Num);
//void CAT25X_Write (unsigned long adr, unsigned long sz, unsigned char *buf);
//void CAT25X_Read (unsigned long adr, unsigned long sz, unsigned char *buf);
#endif
