#ifndef _SOFT_UART_H_
#define _SOFT_UART_H_

#include <string.h>
#include <stdio.h>

#define NO_PARITY 0    //��У��
#define ODD_PARITY 1   //��У��
#define EVEN_PARITY 2  //żУ��
#define MARK_PARITY 3  //У��λʼ��λ1
#define SPACE_PARITY 4 //У��λʼ��λ0

#define _1_STOP 0   // 1λֹͣλ
#define _1_5_STOP 1 // 1.5λֹͣλ
#define _2_STOP 2   // 2λֹͣλ

//keil�����򵼹���
// <<< Use Configuration Wizard in Context Menu >>>\n
//==========================================================

// <q> SIGNGLE_LINE_MODE  - ����ģʽ
#define SIGNGLE_LINE_MODE 1 //����ģʽ

// <q> TX_EN  - ����ʹ��
#define TX_EN 1 //����ʹ��

// <q> RX_EN  - ����ʹ��
#define RX_EN 1 //����ʹ��

// <o> SOFT_UART_PARITY  - ����У��λ
// <i> ����У��λ
// <0=> ��У��
// <1=> ��У��
// <2=> żУ��
// <3=> У��λʼ��λ1
// <4=> У��λʼ��λ0
#define SOFT_UART_PARITY 0 //����У��λ

// <o> SOFT_UART_STOP  - ����ֹͣλ
// <i> ����ֹͣλ
// <0=> 1λֹͣλ
// <1=> 1.5λֹͣλ
// <2=> 2λֹͣλ
#define SOFT_UART_STOP 0 //����ֹͣλ


// <<< end of configuration section >>>

typedef enum
{
    UART_IDLE = 0,   //����״̬
    UART_START,      //��ʼλ
    UART_DATA,       //����
    UART_PARITY,     //��żУ��λ
    UART_STOP,       //����λ
} SOFT_UART_STATE_T; //�������״̬ö��

typedef enum
{
    RX_STATE = 0,                      //����״̬
    TX_STATE,                          //����״̬
} SOFT_UART_SIGNGLE_LINE_WORK_STATE_T; //������ڵ��߹���״̬

typedef struct
{
    unsigned char TI : 1;            //������ɱ�־
    unsigned char RI : 1;            //������ɱ�־
    unsigned char tx_send_start : 1; //���Ϳ�ʼ��־����Ҫ���͵����ݷ���tx_SBUF�󣬽���λ��1
    unsigned char tx_SBUF;           //���ݻ�����
    unsigned char tx_send_cnt;       //
    unsigned char tx_bit_cnt;        //
    SOFT_UART_STATE_T tx_state;      //����״̬

    unsigned char rx_SBUF;      //���ݻ�����
    unsigned char rx_send_cnt;  //
    unsigned char rx_bit_cnt;   //
    SOFT_UART_STATE_T rx_state; //����״̬

    SOFT_UART_SIGNGLE_LINE_WORK_STATE_T signgle_line_work_state; //������ڵ��߹���״̬

} SOFT_UART_T;

extern SOFT_UART_T soft_uart;

void soft_uart_isr(void);                                          //����ģ�� �˺�����Ҫ���ж���ִ��
void soft_uart_clear_rx_parameter(void);                           //������ղ���
void soft_uart_clear_tx_parameter(void);                           //������Ͳ���
void soft_uart_rx_pin_init(void);                                  //����rx����
void soft_uart_tx_pin_init(void);                                  //����tx����
void soft_uart_Transmit_Byte(unsigned char ch);                    //���ڷ��͵�������
void soft_uart_Transmit_nByte(unsigned char *ch, unsigned char n); //���ڷ���n������

#endif
