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
** 函数名称: K E Y _ P R O C E S S
** 功能描述:
** 输入参数:
** 输出参数:
** 返 回 值:
** 作    者:
** 日    期:
** 版    本:
** 修改日期  版本号   修改人  修改内容
**************************************************************/

void key_process(void)
{
    static int key1_cnt = 0;
    static int key2_cnt = 0;
    if (key1.event.click_up)
    {
        key1_cnt++;
        key1_tim.cnt = 3;        //设置一个3S钟的定时
        soft_uart_tx_pin_init(); //配置tx引脚
        printf("key1_cnt:%d\n", key1_cnt);
        soft_uart_rx_pin_init(); //配置rx引脚
    }
    if (key2.event.click_up)
    {
        key2_cnt++;
        key2_tim.cnt = 3;        //设置一个3S钟的定时
        soft_uart_tx_pin_init(); //配置tx引脚
        printf("key2_cnt:%d\n", key2_cnt);
        soft_uart_rx_pin_init(); //配置rx引脚
    }
}

/**************************************************
 *函数名称：void  Sys_Scan(void)
 *函数功能：扫描TK和显示
 *入口参数：void
 *出口参数：void
 **************************************************/
void Sys_Scan(void)
{
    key1.is_key_down = (P23 == 0) ? 1 : 0;
    key2.is_key_down = (P24 == 0) ? 1 : 0;
}
