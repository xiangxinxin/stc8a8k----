
#include "soft_uart.h"
#include "STC8.H"

SOFT_UART_T soft_uart = {0};

void soft_uart_clear_rx_parameter(void);                           //������ղ���
void soft_uart_clear_tx_parameter(void);                           //������Ͳ���
void soft_uart_rx_pin_init(void);                                  //����rx����
unsigned char soft_uart_get_rx_pin_state(void);                    //��ȡrx���ŵ�ƽ״̬
void soft_uart_tx_pin_init(void);                                  //����tx����
void soft_uart_set_tx_pin_high_level(void);                        //���÷�����������ߵ�ƽ
void soft_uart_set_tx_pin_low_level(void);                         //���÷�����������͵�ƽ
void soft_uart_Transmit_Byte(unsigned char ch);                    //���ڷ��͵�������
void soft_uart_Transmit_nByte(unsigned char *ch, unsigned char n); //���ڷ���n������

//����ģ�� �˺�����Ҫ���ж���ִ��
void soft_uart_isr(void)
{
#if SIGNGLE_LINE_MODE //����ģʽ
    if (soft_uart.signgle_line_work_state == TX_STATE)
    {
#endif
#if TX_EN                           //����ʹ��
        switch (soft_uart.tx_state) //����״̬
        {
        case UART_IDLE: //����״̬
        {
            if (soft_uart.tx_send_start) //���Ϳ�ʼ��־����Ҫ���͵����ݷ���tx_SBUF�󣬽���λ��1
            {
                soft_uart.tx_state = UART_START; //����״̬
            }
        }
        break;
        case UART_START: //��ʼλ
        {
            soft_uart_set_tx_pin_low_level();
            soft_uart.tx_send_cnt++;
            if (soft_uart.tx_send_cnt >= 10)
            {
                soft_uart.tx_send_cnt = -1;
                soft_uart.tx_bit_cnt = 0;
                soft_uart.tx_state = UART_DATA; //����״̬
            }
        }
        break;
        case UART_DATA: //����
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
#if (SOFT_UART_PARITY == NO_PARITY)                 //��У��
                    soft_uart.tx_state = UART_STOP; //����״̬
                    break;
#else
                    soft_uart.tx_state = UART_PARITY; //����״̬
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
        case UART_PARITY: //��żУ��λ
        {
            soft_uart.tx_send_cnt++;
            if (soft_uart.tx_send_cnt >= 10)
            {
                soft_uart.tx_send_cnt = 0;
                soft_uart.tx_state = UART_STOP; //����״̬
            }
        }
        break;
        case UART_STOP: //����λ
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
                        soft_uart.TI = 1;               //������ɱ�־
                        soft_uart.tx_state = UART_IDLE; //����״̬
                    }
        }
        break;
        default:
            break;
        }
#endif

#if SIGNGLE_LINE_MODE //����ģʽ
    }
    if (soft_uart.signgle_line_work_state == RX_STATE)
    {
#endif
#if RX_EN                           //����ʹ��
        switch (soft_uart.rx_state) //����״̬
        {
        case UART_IDLE: //����״̬
        {
            if (!soft_uart_get_rx_pin_state())
            {
                soft_uart.rx_state = UART_START; //����״̬
                soft_uart.rx_send_cnt = 0;
            }
        }
        break;
        case UART_START: //��ʼλ
        {
            soft_uart.rx_send_cnt++;
            if (soft_uart.rx_send_cnt >= 10)
            {
                soft_uart.rx_send_cnt = 0;
                soft_uart.rx_bit_cnt = 0;
                soft_uart.rx_state = UART_DATA; //����״̬
            }
        }
        break;
        case UART_DATA: //����
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
#if (SOFT_UART_PARITY == NO_PARITY)                 //��У��
                    soft_uart.rx_state = UART_STOP; //����״̬
#else
                    soft_uart.rx_state = UART_PARITY; //����״̬
#endif
                }
            }
        }
        break;
        case UART_PARITY: //��żУ��λ
        {
            soft_uart.rx_send_cnt++;
            if (soft_uart.rx_send_cnt >= 10)
            {
                soft_uart.rx_send_cnt = 0;
                soft_uart.rx_state = UART_STOP; //����״̬
            }
        }
        break;
        case UART_STOP: //����λ
        {
            soft_uart.rx_send_cnt++;
            if (soft_uart.rx_send_cnt >= 5)
            {
                if (soft_uart_get_rx_pin_state())
                {
                    soft_uart.rx_send_cnt = 0;
                    soft_uart.RI = 1;               //������ɱ�־
                    soft_uart.rx_state = UART_IDLE; //����״̬
                }
            }
        }
        break;
        default:
            break;
        }
#endif
#if SIGNGLE_LINE_MODE //����ģʽ
    }
#endif
}

#if TRANSMIT_NBYTE_EN || PRINTF_EN
//���ڷ��͵�������
void soft_uart_Transmit_Byte(unsigned char ch)
{
    while ((soft_uart.TI == 0) || (soft_uart.tx_state != UART_IDLE))
        ;
    soft_uart.TI = 0;
    soft_uart.tx_SBUF = ch;
    soft_uart.tx_send_start = 1; //���Ϳ�ʼ��־����Ҫ���͵����ݷ���tx_SBUF�󣬽���λ��1
    while (soft_uart.TI == 0)
        ;
}
#endif

#if TRANSMIT_NBYTE_EN
//���ڷ���n������
void soft_uart_Transmit_nByte(unsigned char *ch, unsigned char n)
{
    while (n--)
    {
        soft_uart_Transmit_Byte(*ch++);
    }
}
#endif

#if PRINTF_EN
//֧��printf��ӡ  ϵͳ�е�putchar�ض���
char putchar(char c)
{
    soft_uart_Transmit_Byte(c);
    return (c);
}
#endif

//������ղ���
void soft_uart_clear_rx_parameter(void)
{
    soft_uart.RI = 0;
    soft_uart.rx_bit_cnt = 0;
    soft_uart.rx_send_cnt = 0;
    soft_uart.rx_SBUF = 0;          //���ݻ�����
    soft_uart.rx_state = UART_IDLE; //����״̬
    soft_uart.signgle_line_work_state = RX_STATE;
}

//������Ͳ���
void soft_uart_clear_tx_parameter(void)
{
    soft_uart.TI = 1; //������ɱ�־
    soft_uart.tx_send_start = 0;
    soft_uart.tx_bit_cnt = 0;
    soft_uart.tx_send_cnt = 0;
    soft_uart.tx_SBUF = 0;          //���ݻ�����
    soft_uart.tx_state = UART_IDLE; //����״̬
    soft_uart.signgle_line_work_state = TX_STATE;
}

//����rx����
void soft_uart_rx_pin_init(void)
{
    soft_uart_clear_rx_parameter(); //������ղ���
    P0M0 = 0x00;
    P0M1 = 0x00; //׼˫���
}

//��ȡrx���ŵ�ƽ״̬
unsigned char soft_uart_get_rx_pin_state(void)
{
    return P00;
}

//����tx����
void soft_uart_tx_pin_init(void)
{
    soft_uart_clear_tx_parameter(); //������Ͳ���
    P0M0 = 0x00;
    P0M1 = 0x00; //׼˫���
}

//���÷�����������ߵ�ƽ
void soft_uart_set_tx_pin_high_level(void)
{
    P00 = 1;
}

//���÷�����������͵�ƽ
void soft_uart_set_tx_pin_low_level(void)
{
    P00 = 0;
}
