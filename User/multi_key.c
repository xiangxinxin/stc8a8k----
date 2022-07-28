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
** 函数名称: M U L T I _ K E Y _ I N I T
** 功能描述:
** 输入参数:
** 输出参数:
** 返 回 值:
** 作    者:
** 日    期:
** 版    本:
** 修改日期  版本号   修改人  修改内容
**************************************************************/
void multi_key_init(void)
{
    xdata uint8_t i;
    for (i = 0; i < KEYNUM; i++) //扫描中每个按键
    {
        // mult_key_array[i].is_key_down = 0;
        // mult_key_array[i].filter_conut = 0;
        mult_key_array[i].scan_status = DEBOUNCE;        //等待状态
        mult_key_array[i].repeat_time = KEY_REPEAT_TIME; //重复触发判定时间
        // mult_key_array[i].repeat_count = 0;
        // mult_key_array[i].long_count = 0;
        mult_key_array[i].long_time = KEY_LONG_TIME; //长按时间
        // mult_key_array[i].event.key_down = 0;
        mult_key_array[i].event.key_up = 1;
        // mult_key_array[i].event.click_down = 0; //点击 按下有效
        // mult_key_array[i].event.click_up = 0;   //点击 按下有效
        // mult_key_array[i].event.key_long_pressed = 0;
        // mult_key_array[i].event.key_repeat = 0;
        // mult_key_array[i].event.valid = 0; //无效
    }
}

/**
 * @description: 扫描按键
 * @param {type}
 * @return:
 */
void key_scan(void)
{
    xdata uint8_t i;
#if KEY_MODE
    static bit none_key_pressed_flag = FALSE;
    none_key_pressed_flag = check_has_key_press(); //判断是否有按键up
#endif
    for (i = 0; i < KEYNUM; i++) //扫描中每个按键
    {
        switch (mult_key_array[i].scan_status)
        {
        case DEBOUNCE:                         // 等待
            if (mult_key_array[i].is_key_down) //判断底层物理层是否有按键按下 判断的是key_value中的数据
            {
                if (mult_key_array[i].filter_conut++ >= KEY_FILTER_TIME) //按键按下计数>=按键滤波变量（用于滤波）
                {
                    mult_key_array[i].scan_status = PressStatus; //标记进入按下状态                    mult_key_array[i].filter_conut = 0;          //按键按下计数=设定的滤波变量
                    mult_key_array[i].filter_conut = 0;          //按键按下计数=设定的滤波变量
#if KEY_MODE                                                     // KEY_MODE=1：单按键触发模式
                    if (none_key_pressed_flag)                   //判断所有按键中是不是所有按键都松开了
                    {
                        mult_key_array[i].event.valid = 1; //没有按键按下 后面的逻辑判断有效
                    }
                    else
                    {
                        mult_key_array[i].event.valid = 0; //有按键按下 后面的逻辑判断无效
                    }
#endif
                }
            }
            else
            {
                mult_key_array[i].filter_conut = 0;           //清零按键按下扫描次数计数
                mult_key_array[i].event.key_up = 1;           //标记按键松开标记
                mult_key_array[i].event.key_down = 0;         //清除按键按下标记
                mult_key_array[i].event.click_down = 0;       //点击 按下有效
                mult_key_array[i].event.click_up = 0;         //点击 松手有效
                mult_key_array[i].event.key_long_pressed = 0; //清除长压标记
                mult_key_array[i].event.key_repeat = 0;       //清除重复标记
            }
            mult_key_array[i].long_count = 0;   //清零长按计数
            mult_key_array[i].repeat_count = 0; //清零重复计数
            break;

        case PressStatus:                      //按键压下状态
            if (mult_key_array[i].is_key_down) //判断底层物理层是否有按键按下
            {
                mult_key_array[i].event.key_up = 0; //清除松手标志
#if KEY_MODE
                if (mult_key_array[i].event.valid) //事件是否有效
                {
                    mult_key_array[i].event.key_down = 1;   //标记按键按下事件（触发）
                    mult_key_array[i].event.click_down = 1; //点击 按下有效
                }
#else
                mult_key_array[i].event.key_down = 1;   //标记按键按下事件（触发）
                mult_key_array[i].event.click_down = 1; //点击 按下有效
#endif
                mult_key_array[i].scan_status = LongPressStatus; //设置扫描状态为长按状态
            }
            else
            {
                mult_key_array[i].scan_status = DEBOUNCE; //设为等待状态
            }
            break;

        case LongPressStatus:                       //长按状态
            mult_key_array[i].event.click_down = 0; //点击 按下有效
            if (mult_key_array[i].is_key_down)      //判断底层物理层是否有按键按下
            {
                if (mult_key_array[i].long_time > 0) //长按计时>0
                {
                    if (++mult_key_array[i].long_count >= mult_key_array[i].long_time) //长按计时自加同时判断与设定值的大小
                    {
                        mult_key_array[i].event.key_long_pressed = 1; //标记长按事件
                        mult_key_array[i].scan_status = RepeatStatus; //进入重复状态
                    }
                }
            }
            else
            {
                mult_key_array[i].event.click_up = 1;     //点击 松手有效
                mult_key_array[i].scan_status = DEBOUNCE; //标记进入等待状态
            }
            break;

        case RepeatStatus:                     //重复状态
            if (mult_key_array[i].is_key_down) //判断是否按下
            {
                if (mult_key_array[i].repeat_time > 0) //重复计时>0
                {
                    if (++mult_key_array[i].repeat_count >= mult_key_array[i].repeat_time) //重复触发计时大于设定值
                    {
                        mult_key_array[i].repeat_count = 0; //重复触发计时清零

                        mult_key_array[i].event.key_repeat = 1; //标记重复事件
                    }
                }
            }
            else
            {
                mult_key_array[i].scan_status = DEBOUNCE; //设为等待状态
            }
            break;

        default:
            mult_key_array[i].scan_status = DEBOUNCE; //设为等待状态
            break;
        }
    }
}
