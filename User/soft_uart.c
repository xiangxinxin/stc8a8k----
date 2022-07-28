
#include "soft_uart.h"
#include "STC8.H"

SOFT_UART_T soft_uart = {0};

void soft_uart_clear_rx_parameter(void);                           //清除接收参数
void soft_uart_clear_tx_parameter(void);                           //清除发送参数
void soft_uart_rx_pin_init(void);                                  //配置rx引脚
unsigned char soft_uart_get_rx_pin_state(void);                    //获取rx引脚电平状态
void soft_uart_tx_pin_init(void);                                  //配置tx引脚
void soft_uart_set_tx_pin_high_level(void);                        //设置发送引脚输出高电平
void soft_uart_set_tx_pin_low_level(void);                         //设置发送引脚输出低电平
void soft_uart_Transmit_Byte(unsigned char ch);                    //软串口发送单个数据
void soft_uart_Transmit_nByte(unsigned char *ch, unsigned char n); //软串口发送n个数据

//软串口模块 此函数需要在中断中执行
void soft_uart_isr(void)
{
#if SIGNGLE_LINE_MODE //单线模式
    if (soft_uart.signgle_line_work_state == TX_STATE)
    {
#endif
#if TX_EN                           //发送使能
        switch (soft_uart.tx_state) //串口状态
        {
        case UART_IDLE: //空闲状态
        {
            if (soft_uart.tx_send_start) //发送开始标志，将要发送的数据放入tx_SBUF后，将此位置1
            {
                soft_uart.tx_state = UART_START; //串口状态
            }
        }
        break;
        case UART_START: //起始位
        {
            soft_uart_set_tx_pin_low_level();
            soft_uart.tx_send_cnt++;
            if (soft_uart.tx_send_cnt >= 10)
            {
                soft_uart.tx_send_cnt = -1;
                soft_uart.tx_bit_cnt = 0;
                soft_uart.tx_state = UART_DATA; //串口状态
            }
        }
        break;
        case UART_DATA: //数据
        {
            soft_uart.tx_send_cnt++;
            if (soft_uart.tx_send_cnt >= 10)
            {
                soft_uart.tx_send_cnt = 0;
                soft_uart.tx_SBUF >>= 1;
                soft_uart.tx_bit_cnt++;
                if (soft_uart.tx_bit_cnt >= 8)
                {
                    soft_uart.tx_bit_cnt = 0;
#if (SOFT_UART_PARITY == NO_PARITY)                 //奇校验
                    soft_uart.tx_state = UART_STOP; //串口状态
                    break;
#else
                    soft_uart.tx_state = UART_PARITY; //串口状态
                    break;
#endif
                }
            }
            if (soft_uart.tx_SBUF & 0x01)
            {
                soft_uart_set_tx_pin_high_level();
            }
            else
            {
                soft_uart_set_tx_pin_low_level();
            }
        }
        break;
        case UART_PARITY: //奇偶校验位
        {
            soft_uart.tx_send_cnt++;
            if (soft_uart.tx_send_cnt >= 10)
            {
                soft_uart.tx_send_cnt = 0;
                soft_uart.tx_state = UART_STOP; //串口状态
            }
        }
        break;
        case UART_STOP: //结束位
        {
            soft_uart.tx_send_cnt++;
            soft_uart_set_tx_pin_high_level();
#if (SOFT_UART_STOP == _1_STOP)
            if (soft_uart.tx_send_cnt >= 10)
#endif
#if (SOFT_UART_STOP == _1_5_STOP)
                if (soft_uart.tx_send_cnt > 15)
#endif
#if (SOFT_UART_STOP == _2_STOP)
                    if (soft_uart.tx_send_cnt > 20)
#endif
                    {
                        soft_uart.tx_send_cnt = 0;
                        soft_uart.tx_send_start = 0;
                        soft_uart.TI = 1;               //发送完成标志
                        soft_uart.tx_state = UART_IDLE; //串口状态
                    }
        }
        break;
        default:
            break;
        }
#endif

#if SIGNGLE_LINE_MODE //单线模式
    }
    if (soft_uart.signgle_line_work_state == RX_STATE)
    {
#endif
#if RX_EN                           //接收使能
        switch (soft_uart.rx_state) //串口状态
        {
        case UART_IDLE: //空闲状态
        {
            if (!soft_uart_get_rx_pin_state())
            {
                soft_uart.rx_state = UART_START; //串口状态
                soft_uart.rx_send_cnt = 0;
            }
        }
        break;
        case UART_START: //起始位
        {
            soft_uart.rx_send_cnt++;
            if (soft_uart.rx_send_cnt >= 10)
            {
                soft_uart.rx_send_cnt = 0;
                soft_uart.rx_bit_cnt = 0;
                soft_uart.rx_state = UART_DATA; //串口状态
            }
        }
        break;
        case UART_DATA: //数据
        {
            soft_uart.rx_send_cnt++;

            if (soft_uart.rx_send_cnt == 5)
            {
                soft_uart.rx_SBUF >>= 1;
                if (soft_uart_get_rx_pin_state())
                {
                    soft_uart.rx_SBUF |= 0x80;
                }
                else
                {
                    soft_uart.rx_SBUF &= ~0x80;
                }
                soft_uart.rx_bit_cnt++;
            }
            if (soft_uart.rx_send_cnt >= 10)
            {
                soft_uart.rx_send_cnt = 0;
                if (soft_uart.rx_bit_cnt >= 8)
                {
#if (SOFT_UART_PARITY == NO_PARITY)                 //奇校验
                    soft_uart.rx_state = UART_STOP; //串口状态
#else
                    soft_uart.rx_state = UART_PARITY; //串口状态
#endif
                }
            }
        }
        break;
        case UART_PARITY: //奇偶校验位
        {
            soft_uart.rx_send_cnt++;
            if (soft_uart.rx_send_cnt >= 10)
            {
                soft_uart.rx_send_cnt = 0;
                soft_uart.rx_state = UART_STOP; //串口状态
            }
        }
        break;
        case UART_STOP: //结束位
        {
            soft_uart.rx_send_cnt++;
            if (soft_uart.rx_send_cnt >= 5)
            {
                if (soft_uart_get_rx_pin_state())
                {
                    soft_uart.rx_send_cnt = 0;
                    soft_uart.RI = 1;               //接收完成标志
                    soft_uart.rx_state = UART_IDLE; //串口状态
                }
            }
        }
        break;
        default:
            break;
        }
#endif
#if SIGNGLE_LINE_MODE //单线模式
    }
#endif
}

#if TRANSMIT_NBYTE_EN || PRINTF_EN
//软串口发送单个数据
void soft_uart_Transmit_Byte(unsigned char ch)
{
    while ((soft_uart.TI == 0) || (soft_uart.tx_state != UART_IDLE))
        ;
    soft_uart.TI = 0;
    soft_uart.tx_SBUF = ch;
    soft_uart.tx_send_start = 1; //发送开始标志，将要发送的数据放入tx_SBUF后，将此位置1
    while (soft_uart.TI == 0)
        ;
}
#endif

#if TRANSMIT_NBYTE_EN
//软串口发送n个数据
void soft_uart_Transmit_nByte(unsigned char *ch, unsigned char n)
{
    while (n--)
    {
        soft_uart_Transmit_Byte(*ch++);
    }
}
#endif

#if PRINTF_EN
//支持printf打印  系统中的putchar重定向
char putchar(char c)
{
    soft_uart_Transmit_Byte(c);
    return (c);
}
#endif

//清除接收参数
void soft_uart_clear_rx_parameter(void)
{
    soft_uart.RI = 0;
    soft_uart.rx_bit_cnt = 0;
    soft_uart.rx_send_cnt = 0;
    soft_uart.rx_SBUF = 0;          //数据缓冲区
    soft_uart.rx_state = UART_IDLE; //空闲状态
    soft_uart.signgle_line_work_state = RX_STATE;
}

//清除发送参数
void soft_uart_clear_tx_parameter(void)
{
    soft_uart.TI = 1; //发送完成标志
    soft_uart.tx_send_start = 0;
    soft_uart.tx_bit_cnt = 0;
    soft_uart.tx_send_cnt = 0;
    soft_uart.tx_SBUF = 0;          //数据缓冲区
    soft_uart.tx_state = UART_IDLE; //空闲状态
    soft_uart.signgle_line_work_state = TX_STATE;
}

//配置rx引脚
void soft_uart_rx_pin_init(void)
{
    soft_uart_clear_rx_parameter(); //清除接收参数
    P0M0 = 0x00;
    P0M1 = 0x00; //准双向口
}

//获取rx引脚电平状态
unsigned char soft_uart_get_rx_pin_state(void)
{
    return P00;
}

//配置tx引脚
void soft_uart_tx_pin_init(void)
{
    soft_uart_clear_tx_parameter(); //清除发送参数
    P0M0 = 0x00;
    P0M1 = 0x00; //准双向口
}

//设置发送引脚输出高电平
void soft_uart_set_tx_pin_high_level(void)
{
    P00 = 1;
}

//设置发送引脚输出低电平
void soft_uart_set_tx_pin_low_level(void)
{
    P00 = 0;
}
