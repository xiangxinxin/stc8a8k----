#ifndef __DELAY_H_
#define __DELAY_H_

#define MAIN_Fosc		24000000L	//定义主时钟
#include	"STC8.H"
#include <stdio.h>

#define  uint16   unsigned int  
#define  uint8    unsigned char

void DelayUS(uint16 t);
void DelayMS(uint16 t);

#endif