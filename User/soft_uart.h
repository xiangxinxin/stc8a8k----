#ifndef _SOFT_UART_H_
#define _SOFT_UART_H_

#include <string.h>
#include <stdio.h>

#define NO_PARITY 0    //无校验
#define ODD_PARITY 1   //奇校验
#define EVEN_PARITY 2  //偶校验
#define MARK_PARITY 3  //校验位始终位1
#define SPACE_PARITY 4 //校验位始终位0

#define _1_STOP 0   // 1位停止位
#define _1_5_STOP 1 // 1.5位停止位
#define _2_STOP 2   // 2位停止位

//keil配置向导工具
// <<< Use Configuration Wizard in Context Menu >>>\n
//==========================================================

// <q> SIGNGLE_LINE_MODE  - 单线模式
#define SIGNGLE_LINE_MODE 1 //单线模式

// <q> TX_EN  - 发送使能
#define TX_EN 1 //发送使能

// <q> RX_EN  - 接收使能
#define RX_EN 1 //接收使能

// <o> SOFT_UART_PARITY  - 配置校验位
// <i> 配置校验位
// <0=> 无校验
// <1=> 奇校验
// <2=> 偶校验
// <3=> 校验位始终位1
// <4=> 校验位始终位0
#define SOFT_UART_PARITY 0 //配置校验位

// <o> SOFT_UART_STOP  - 配置停止位
// <i> 配置停止位
// <0=> 1位停止位
// <1=> 1.5位停止位
// <2=> 2位停止位
#define SOFT_UART_STOP 0 //配置停止位


// <<< end of configuration section >>>

typedef enum
{
    UART_IDLE = 0,   //空闲状态
    UART_START,      //起始位
    UART_DATA,       //数据
    UART_PARITY,     //奇偶校验位
    UART_STOP,       //结束位
} SOFT_UART_STATE_T; //软件串口状态枚举

typedef enum
{
    RX_STATE = 0,                      //发送状态
    TX_STATE,                          //接收状态
} SOFT_UART_SIGNGLE_LINE_WORK_STATE_T; //软件串口单线工作状态

typedef struct
{
    unsigned char TI : 1;            //发送完成标志
    unsigned char RI : 1;            //接收完成标志
    unsigned char tx_send_start : 1; //发送开始标志，将要发送的数据放入tx_SBUF后，将此位置1
    unsigned char tx_SBUF;           //数据缓冲区
    unsigned char tx_send_cnt;       //
    unsigned char tx_bit_cnt;        //
    SOFT_UART_STATE_T tx_state;      //串口状态

    unsigned char rx_SBUF;      //数据缓冲区
    unsigned char rx_send_cnt;  //
    unsigned char rx_bit_cnt;   //
    SOFT_UART_STATE_T rx_state; //串口状态

    SOFT_UART_SIGNGLE_LINE_WORK_STATE_T signgle_line_work_state; //软件串口单线工作状态

} SOFT_UART_T;

extern SOFT_UART_T soft_uart;

void soft_uart_isr(void);                                          //软串口模块 此函数需要在中断中执行
void soft_uart_clear_rx_parameter(void);                           //清除接收参数
void soft_uart_clear_tx_parameter(void);                           //清除发送参数
void soft_uart_rx_pin_init(void);                                  //配置rx引脚
void soft_uart_tx_pin_init(void);                                  //配置tx引脚
void soft_uart_Transmit_Byte(unsigned char ch);                    //软串口发送单个数据
void soft_uart_Transmit_nByte(unsigned char *ch, unsigned char n); //软串口发送n个数据

#endif
