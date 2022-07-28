/*
 * @Description:
 * @Author:
 * @Change Logs:
 * @Date            Author        Notes:
 * 2010-10-12
 */

#include "multi_key.h"

xdata MULTI_KEY_T mult_key_array[KEYNUM] = {0};

/************************************************************
** ��������: M U L T I _ K E Y _ I N I T
** ��������:
** �������:
** �������:
** �� �� ֵ:
** ��    ��:
** ��    ��:
** ��    ��:
** �޸�����  �汾��   �޸���  �޸�����
**************************************************************/
void multi_key_init(void)
{
    xdata uint8_t i;
    for (i = 0; i < KEYNUM; i++) //ɨ����ÿ������
    {
        // mult_key_array[i].is_key_down = 0;
        // mult_key_array[i].filter_conut = 0;
        mult_key_array[i].scan_status = DEBOUNCE;        //�ȴ�״̬
        mult_key_array[i].repeat_time = KEY_REPEAT_TIME; //�ظ������ж�ʱ��
        // mult_key_array[i].repeat_count = 0;
        // mult_key_array[i].long_count = 0;
        mult_key_array[i].long_time = KEY_LONG_TIME; //����ʱ��
        // mult_key_array[i].event.key_down = 0;
        mult_key_array[i].event.key_up = 1;
        // mult_key_array[i].event.click_down = 0; //��� ������Ч
        // mult_key_array[i].event.click_up = 0;   //��� ������Ч
        // mult_key_array[i].event.key_long_pressed = 0;
        // mult_key_array[i].event.key_repeat = 0;
        // mult_key_array[i].event.valid = 0; //��Ч
    }
}

/**
 * @description: ɨ�谴��
 * @param {type}
 * @return:
 */
void key_scan(void)
{
    xdata uint8_t i;
#if KEY_MODE
    static bit none_key_pressed_flag = FALSE;
    none_key_pressed_flag = check_has_key_press(); //�ж��Ƿ��а���up
#endif
    for (i = 0; i < KEYNUM; i++) //ɨ����ÿ������
    {
        switch (mult_key_array[i].scan_status)
        {
        case DEBOUNCE:                         // �ȴ�
            if (mult_key_array[i].is_key_down) //�жϵײ�������Ƿ��а������� �жϵ���key_value�е�����
            {
                if (mult_key_array[i].filter_conut++ >= KEY_FILTER_TIME) //�������¼���>=�����˲������������˲���
                {
                    mult_key_array[i].scan_status = PressStatus; //��ǽ��밴��״̬                    mult_key_array[i].filter_conut = 0;          //�������¼���=�趨���˲�����
                    mult_key_array[i].filter_conut = 0;          //�������¼���=�趨���˲�����
#if KEY_MODE                                                     // KEY_MODE=1������������ģʽ
                    if (none_key_pressed_flag)                   //�ж����а������ǲ������а������ɿ���
                    {
                        mult_key_array[i].event.valid = 1; //û�а������� ������߼��ж���Ч
                    }
                    else
                    {
                        mult_key_array[i].event.valid = 0; //�а������� ������߼��ж���Ч
                    }
#endif
                }
            }
            else
            {
                mult_key_array[i].filter_conut = 0;           //���㰴������ɨ���������
                mult_key_array[i].event.key_up = 1;           //��ǰ����ɿ����
                mult_key_array[i].event.key_down = 0;         //����������±��
                mult_key_array[i].event.click_down = 0;       //��� ������Ч
                mult_key_array[i].event.click_up = 0;         //��� ������Ч
                mult_key_array[i].event.key_long_pressed = 0; //�����ѹ���
                mult_key_array[i].event.key_repeat = 0;       //����ظ����
            }
            mult_key_array[i].long_count = 0;   //���㳤������
            mult_key_array[i].repeat_count = 0; //�����ظ�����
            break;

        case PressStatus:                      //����ѹ��״̬
            if (mult_key_array[i].is_key_down) //�жϵײ�������Ƿ��а�������
            {
                mult_key_array[i].event.key_up = 0; //������ֱ�־
#if KEY_MODE
                if (mult_key_array[i].event.valid) //�¼��Ƿ���Ч
                {
                    mult_key_array[i].event.key_down = 1;   //��ǰ��������¼���������
                    mult_key_array[i].event.click_down = 1; //��� ������Ч
                }
#else
                mult_key_array[i].event.key_down = 1;   //��ǰ��������¼���������
                mult_key_array[i].event.click_down = 1; //��� ������Ч
#endif
                mult_key_array[i].scan_status = LongPressStatus; //����ɨ��״̬Ϊ����״̬
            }
            else
            {
                mult_key_array[i].scan_status = DEBOUNCE; //��Ϊ�ȴ�״̬
            }
            break;

        case LongPressStatus:                       //����״̬
            mult_key_array[i].event.click_down = 0; //��� ������Ч
            if (mult_key_array[i].is_key_down)      //�жϵײ�������Ƿ��а�������
            {
                if (mult_key_array[i].long_time > 0) //������ʱ>0
                {
                    if (++mult_key_array[i].long_count >= mult_key_array[i].long_time) //������ʱ�Լ�ͬʱ�ж����趨ֵ�Ĵ�С
                    {
                        mult_key_array[i].event.key_long_pressed = 1; //��ǳ����¼�
                        mult_key_array[i].scan_status = RepeatStatus; //�����ظ�״̬
                    }
                }
            }
            else
            {
                mult_key_array[i].event.click_up = 1;     //��� ������Ч
                mult_key_array[i].scan_status = DEBOUNCE; //��ǽ���ȴ�״̬
            }
            break;

        case RepeatStatus:                     //�ظ�״̬
            if (mult_key_array[i].is_key_down) //�ж��Ƿ���
            {
                if (mult_key_array[i].repeat_time > 0) //�ظ���ʱ>0
                {
                    if (++mult_key_array[i].repeat_count >= mult_key_array[i].repeat_time) //�ظ�������ʱ�����趨ֵ
                    {
                        mult_key_array[i].repeat_count = 0; //�ظ�������ʱ����

                        mult_key_array[i].event.key_repeat = 1; //����ظ��¼�
                    }
                }
            }
            else
            {
                mult_key_array[i].scan_status = DEBOUNCE; //��Ϊ�ȴ�״̬
            }
            break;

        default:
            mult_key_array[i].scan_status = DEBOUNCE; //��Ϊ�ȴ�״̬
            break;
        }
    }
}
