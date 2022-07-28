
#include "multi_tim.h"

#define TIM_500MS_TICK TICK_SEC / 2
xdata MULTI_TIM_T tim_head_handle[TIM_NUM] = {0}; //计时器
bit tim_500ms_flag;								  //定时器500ms flag

void muti_tim_init(void) //多定时器初始化
{
	uint8_t i;
	for (i = 0; i < TIM_NUM; i++)
	{
		tim_head_handle[i].run = 1;
	}
}
/************************************************************
** 函数名称: M U T I _ T I M _ D E C
** 功能描述: 需要1ms的时基
** 输入参数:
** 输出参数: 
** 返 回 值:
** 作    者:  
** 日    期:  
** 版    本:  1.0
** 修改日期  版本号   修改人  修改内容
*
**************************************************************/
void muti_tim_dec(void) //多定时器自减
{
	static uint16_t ms_tick = 0;
	uint8_t i;
	if (ms_tick++ >= TICK_SEC)
	{
		ms_tick = 0;
		for (i = 0; i < TIM_NUM; i++)
		{
			if (tim_head_handle[i].run)
			{
				if (tim_head_handle[i].cnt)
				{
					tim_head_handle[i].cnt--;
					if (!tim_head_handle[i].cnt)
						tim_head_handle[i].alarm = 1;
				}
			}
		}
	}
	if (ms_tick >= TIM_500MS_TICK)
		tim_500ms_flag = 1; //定时器500ms flag
	else
		tim_500ms_flag = 0; //定时器500ms flag
}