
#include "multi_tim.h"

#define TIM_500MS_TICK TICK_SEC / 2
xdata MULTI_TIM_T tim_head_handle[TIM_NUM] = {0}; //��ʱ��
bit tim_500ms_flag;								  //��ʱ��500ms flag

void muti_tim_init(void) //�ඨʱ����ʼ��
{
	uint8_t i;
	for (i = 0; i < TIM_NUM; i++)
	{
		tim_head_handle[i].run = 1;
	}
}
/************************************************************
** ��������: M U T I _ T I M _ D E C
** ��������: ��Ҫ1ms��ʱ��
** �������:
** �������: 
** �� �� ֵ:
** ��    ��:  
** ��    ��:  
** ��    ��:  1.0
** �޸�����  �汾��   �޸���  �޸�����
*
**************************************************************/
void muti_tim_dec(void) //�ඨʱ���Լ�
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
		tim_500ms_flag = 1; //��ʱ��500ms flag
	else
		tim_500ms_flag = 0; //��ʱ��500ms flag
}