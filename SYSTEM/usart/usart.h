#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 


#define CMD_GO		0x57	//'W'
#define CMD_BACK	0x53	//'S'
#define CMD_LEFT	0x41	//'A'
#define CMD_RIGHT	0x44	//'D'
	  	
extern uint16_t CMD;

void Uart_init(u32 bound);

#endif


