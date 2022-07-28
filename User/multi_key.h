/*
 * @Description:
 * @Author:
 * @Change Logs:
 * @Date            Author        Notes:
 * 2018-3-27      the first version
 */

#ifndef __MULTI_KEY_H
#define __MULTI_KEY_H

#include "macro.h"


//keil配置向导工具
// <<< Use Configuration Wizard in Context Menu >>>\n
//==========================================================

// <o> KEYNUM - 按键数量
// <i> 设置按键数量
#define KEYNUM 2 //按键数量

// <o> KEY_FILTER_TIME - 滤波消抖次数
// <0-15>
// <i> 设置滤波消抖次数 最大为15次
#define KEY_FILTER_TIME 1 // (max:15)

// <o> KEY_LONG_TIME - 长按时间
// <i> 设置长按时间 最大为255
// <0-255>
#define KEY_LONG_TIME 50 // (max:255)

// <o> KEY_LONG_TIME - 重复触发时间
// <i> 设置重复触发时间 最大为15
// <0-15>
#define KEY_REPEAT_TIME 15 // (max:15)

// <o> KEY_MODE  - 按键模式
// <i> 多按键触发：同一时刻可以触发多个按键
// <i> 单按键触发：同一时刻只能触发一个按键
// <0=> 多按键触发
// <1=> 单按键触发
#define KEY_MODE 0 // 0:多按键同时触发 1：单独按键触发，按键按下不释放，其他按键不释放，长按除外

// <<< end of configuration section >>>

typedef enum
{
    DEBOUNCE = 0,    // 等待
    PressStatus,     // 有按键按下
    LongPressStatus, //长按状态
    RepeatStatus,    //重复状态
} Key_status_enum;   //定义了按键状态 用于按键扫描标记扫描步骤

typedef enum
{
    KEY_DOWN = 0,
    KEY_UP,
    KEY_PRESSED,
    KEY_LONG_PRESSED,
    KEY_REPEAT,
} key_status_t;

typedef struct KEY_EVENT
{
    uint8_t key_down : 1;         //按键按下
    uint8_t key_up : 1;           //按键松开
    uint8_t click_up : 1;         //点击 松手有效
    uint8_t click_down : 1;       //点击 按下有效
    uint8_t key_long_pressed : 1; //按键长按
    uint8_t key_repeat : 1;       //按键重复
    uint8_t valid : 1;            //有效
} KEY_EVENT_T;

typedef struct MULTI_KEY
{
    uint8_t filter_conut : 4; //消抖计数
    uint8_t scan_status : 3;  //扫描状态
    uint8_t is_key_down : 1;  //判断是否按下 在外部由用户处理
    uint8_t repeat_time : 4;  //重复触发判定时间
    uint8_t repeat_count : 4; //重复触发计数
    KEY_EVENT_T event;        //按键事件
    uint8_t long_count;       //长按计数
    uint8_t long_time;        //长按判定时间
} MULTI_KEY_T;

extern xdata MULTI_KEY_T mult_key_array[];
extern void multi_key_init(void);
extern void key_stop(MULTI_KEY_T *handle);
extern void key_scan(void);

#endif
