
#include "uart.h" // ����ͨ�ź���ͷ�ļ�
#include "delay.h"
#include "soft_uart.h"
#include "key.h"
#include "multi_key.h"
#include "multi_tim.h"

#include "STC8.H"

#define T_100US 1                                 //��ʱʱ�䳣������λ100us
#define T1TMS (T_100US * MAIN_Fosc / 10000)       // 1Tģʽ��
#define T12TMS (T_100US * MAIN_Fosc / 10000 / 12) // 12Tģʽ��

/********************************************
���ű�������
*********************************************/
sbit led = P2 ^ 6;
uint8_t time_cnt_inc = 0; //ϵͳ�ܹ��õı���

void Timer0Init(void)
{
    AUXR |= 0x80;               //��ʱ��0Ϊ1Tģʽ
    TMOD &= 0xF8;               //��ʱ��0����Ϊ��ʱ��ʽ������ģʽΪ16λ�Զ���װģʽ
    TMOD &= 0xF7;               //��ʱ��0�ſ�λGATE����Ϊ0
    TL0 = (65536 - T1TMS);      // 1Tģʽ�³�ʼװ��ֵ
    TH0 = (65536 - T1TMS) >> 8; // 1Tģʽ�³�ʼװ��ֵ
    TR0 = 1;                    //��ʱ��0��ʼ��ʱ
    ET0 = 1;                    //ʹ�ܶ�ʱ��0������ж�����λ
}

void system_loop(void) // 1ms
{
    static unsigned char loop_cnt = 0x00;

    muti_tim_dec();           //�ඨʱ���Լ�
    if ((loop_cnt % 20) == 0) // 20ms
    {
        Sys_Scan();    //��ȡ������־λ
        key_scan();    //��������
        key_process(); //�����¼�
    }
    if (loop_cnt == 5)
    {
        if (key1_tim.alarm) //����1�Ķ�ʱʱ�䵽��
        {
            key1_tim.alarm = 0;
            soft_uart_tx_pin_init(); //����tx����
            printf("key1_tim alarm\n");
            soft_uart_rx_pin_init(); //����rx����
        }
        if (key2_tim.alarm) //����2�Ķ�ʱʱ�䵽��
        {
            key2_tim.alarm = 0;
            soft_uart_tx_pin_init(); //����tx����
            printf("key2_tim alarm\n");
            soft_uart_rx_pin_init(); //����rx����
        }
    }
    loop_cnt++;
    if (loop_cnt >= 100) // 100ms
    {
        loop_cnt = 0; //��ʾ�����������Ϊ
    }
}

/**************************************
������������ʱ��0�жϷ������
��ڲ�������
����ֵ����
***************************************/
void timer0_int(void) interrupt 1
{
    static unsigned char _1ms_cnt = 0;
    if (++_1ms_cnt >= 10)
    {
        _1ms_cnt = 0;
        if (time_cnt_inc < 0xFF)
            time_cnt_inc++;
    }
    soft_uart_isr();  //����ģ�� �˺�����Ҫ���ж���ִ��
    if (soft_uart.RI) //������ɱ�־
    {
        if (soft_uart.rx_SBUF == 'A')
            led = 1;
        else if (soft_uart.rx_SBUF == 'B')
            led = 0;
        soft_uart.RI = 0;
    }
}

/***************************************************************************
 * ��  �� : ������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************/
int main()
{
    P0M0 = 0x00; //׼˫���
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;

    Timer0Init();            //��ʱ��0��ʼ��
    ES = 1;                  // ����1�жϴ�
    EA = 1;                  // ���жϴ�
    multi_key_init();        //�������ʼ��
    soft_uart_rx_pin_init(); //����rx����
    muti_tim_init();         //�ඨʱ����ʼ��
    while (1)                // ��ѭ��
    {
        if (time_cnt_inc > 0) // 1ms
        {
            if (time_cnt_inc >= 250)
                time_cnt_inc = 250;
            time_cnt_inc--;
            system_loop();
        }
    }
}
