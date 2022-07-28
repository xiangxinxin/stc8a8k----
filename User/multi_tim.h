

#ifndef __MULTI_TIM_H
#define __MULTI_TIM_H

#include "macro.h"

//keil�����򵼹���
// <<< Use Configuration Wizard in Context Menu >>>\n
//==========================================================

// <o> TIM_NUM - ��ʱ������
// <0-255>
// <i> ��ʱ������ max:255
#define TIM_NUM 2

// <o> TICK_SEC - 1S�Ӽ�������
// <0-65535>
// <i> 1S�Ӽ�����������Ҫ����ʵ���������΢�� max:65535
#define TICK_SEC 1023

// <<< end of configuration section >>>

typedef struct
{
    uint8_t run : 1;   //ֹͣ
    uint8_t alarm : 1; //����
    uint16_t cnt;      //��ʱ
} MULTI_TIM_T;

extern xdata MULTI_TIM_T tim_head_handle[]; //��ʱ��
extern bit tim_500ms_flag;                  //��ʱ��500ms flag

void muti_tim_init(void); //�ඨʱ����ʼ��
void muti_tim_dec(void);  //�ඨʱ���Լ�
#endif
