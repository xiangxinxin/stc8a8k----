

#ifndef __MULTI_TIM_H
#define __MULTI_TIM_H

#include "macro.h"

//keil配置向导工具
// <<< Use Configuration Wizard in Context Menu >>>\n
//==========================================================

// <o> TIM_NUM - 定时器数量
// <0-255>
// <i> 定时器数量 max:255
#define TIM_NUM 2

// <o> TICK_SEC - 1S钟计数次数
// <0-65535>
// <i> 1S钟计数次数，需要根据实际情况进行微调 max:65535
#define TICK_SEC 1023

// <<< end of configuration section >>>

typedef struct
{
    uint8_t run : 1;   //停止
    uint8_t alarm : 1; //闹铃
    uint16_t cnt;      //计时
} MULTI_TIM_T;

extern xdata MULTI_TIM_T tim_head_handle[]; //计时器
extern bit tim_500ms_flag;                  //定时器500ms flag

void muti_tim_init(void); //多定时器初始化
void muti_tim_dec(void);  //多定时器自减
#endif
