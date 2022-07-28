/*
 * @File       :
 * @Change Logs:
 * @Date            Author        Notes:
 */
#include "key.h"
#include "STC8.H"
#include "soft_uart.h"
#include "multi_tim.h"

/************************************************************
** ��������: K E Y _ P R O C E S S
** ��������:
** �������:
** �������:
** �� �� ֵ:
** ��    ��:
** ��    ��:
** ��    ��:
** �޸�����  �汾��   �޸���  �޸�����
**************************************************************/

void key_process(void)
{
    static int key1_cnt = 0;
    static int key2_cnt = 0;
    if (key1.event.click_up)
    {
        key1_cnt++;
        key1_tim.cnt = 3;        //����һ��3S�ӵĶ�ʱ
        soft_uart_tx_pin_init(); //����tx����
        printf("key1_cnt:%d\n", key1_cnt);
        soft_uart_rx_pin_init(); //����rx����
    }
    if (key2.event.click_up)
    {
        key2_cnt++;
        key2_tim.cnt = 3;        //����һ��3S�ӵĶ�ʱ
        soft_uart_tx_pin_init(); //����tx����
        printf("key2_cnt:%d\n", key2_cnt);
        soft_uart_rx_pin_init(); //����rx����
    }
}

/**************************************************
 *�������ƣ�void  Sys_Scan(void)
 *�������ܣ�ɨ��TK����ʾ
 *��ڲ�����void
 *���ڲ�����void
 **************************************************/
void Sys_Scan(void)
{
    key1.is_key_down = (P23 == 0) ? 1 : 0;
    key2.is_key_down = (P24 == 0) ? 1 : 0;
}
