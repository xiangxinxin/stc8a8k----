
#include "uart.h" // 串行通信函数头文件
#include "delay.h"
#include "soft_uart.h"
#include "key.h"
#include "multi_key.h"
#include "multi_tim.h"

#include "STC8.H"

#define T_100US 1                                 //定时时间常量，单位100us
#define T1TMS (T_100US * MAIN_Fosc / 10000)       // 1T模式下
#define T12TMS (T_100US * MAIN_Fosc / 10000 / 12) // 12T模式下

/********************************************
引脚别名定义
*********************************************/
sbit led = P2 ^ 6;
uint8_t time_cnt_inc = 0; //系统架构用的变量

void Timer0Init(void)
{
    AUXR |= 0x80;               //定时器0为1T模式
    TMOD &= 0xF8;               //定时器0设置为定时方式，工作模式为16位自动重装模式
    TMOD &= 0xF7;               //定时器0门控位GATE设置为0
    TL0 = (65536 - T1TMS);      // 1T模式下初始装载值
    TH0 = (65536 - T1TMS) >> 8; // 1T模式下初始装载值
    TR0 = 1;                    //定时器0开始计时
    ET0 = 1;                    //使能定时器0的溢出中断允许位
}

void system_loop(void) // 1ms
{
    static unsigned char loop_cnt = 0x00;

    muti_tim_dec();           //多定时器自减
    if ((loop_cnt % 20) == 0) // 20ms
    {
        Sys_Scan();    //获取按键标志位
        key_scan();    //按键处理
        key_process(); //按键事件
    }
    if (loop_cnt == 5)
    {
        if (key1_tim.alarm) //按键1的定时时间到达
        {
            key1_tim.alarm = 0;
            soft_uart_tx_pin_init(); //配置tx引脚
            printf("key1_tim alarm\n");
            soft_uart_rx_pin_init(); //配置rx引脚
        }
        if (key2_tim.alarm) //按键2的定时时间到达
        {
            key2_tim.alarm = 0;
            soft_uart_tx_pin_init(); //配置tx引脚
            printf("key2_tim alarm\n");
            soft_uart_rx_pin_init(); //配置rx引脚
        }
    }
    loop_cnt++;
    if (loop_cnt >= 100) // 100ms
    {
        loop_cnt = 0; //表示计数到最大数为
    }
}

/**************************************
功能描述：定时器0中断服务程序
入口参数：无
返回值：无
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
    soft_uart_isr();  //软串口模块 此函数需要在中断中执行
    if (soft_uart.RI) //接收完成标志
    {
        if (soft_uart.rx_SBUF == 'A')
            led = 1;
        else if (soft_uart.rx_SBUF == 'B')
            led = 0;
        soft_uart.RI = 0;
    }
}

/***************************************************************************
 * 描  述 : 主函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************/
int main()
{
    P0M0 = 0x00; //准双向口
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;

    Timer0Init();            //定时器0初始化
    ES = 1;                  // 串口1中断打开
    EA = 1;                  // 总中断打开
    multi_key_init();        //按键库初始化
    soft_uart_rx_pin_init(); //配置rx引脚
    muti_tim_init();         //多定时器初始化
    while (1)                // 主循环
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
