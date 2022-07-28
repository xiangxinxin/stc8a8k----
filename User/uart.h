#ifndef __UART_H_
#define __UART_H_


#include "intrins.h"
#include "delay.h"
#include <string.h>           // 加入此头文件后,可使用strstr库函数

#define S4RI  0x01             
#define S4TI  0x02             
          
void U1SendData(uint8 ch);
void U1SendString(uint8 *s);
void UartInit(void);
void U4SendData(uint8 ch);
void U4SendString(uint8 *s);
void CLR_Buf(void);				   
bit  Hand(uint8 *a); 

#endif
