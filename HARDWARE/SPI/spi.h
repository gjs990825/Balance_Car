#ifndef _SPI_H_
#define _SPI_H_

#include "sys.h"


void 	Init(void);
uint8_t RW(u8 dat);
void 	CE_H(void);
void 	CE_L(void);
void 	CSN_H(void);
void 	CSN_L(void);
		 
#endif //_SPI_H_

