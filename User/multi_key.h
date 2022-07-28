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


//keil�����򵼹���
// <<< Use Configuration Wizard in Context Menu >>>\n
//==========================================================

// <o> KEYNUM - ��������
// <i> ���ð�������
#define KEYNUM 2 //��������

// <o> KEY_FILTER_TIME - �˲���������
// <0-15>
// <i> �����˲��������� ���Ϊ15��
#define KEY_FILTER_TIME 1 // (max:15)

// <o> KEY_LONG_TIME - ����ʱ��
// <i> ���ó���ʱ�� ���Ϊ255
// <0-255>
#define KEY_LONG_TIME 50 // (max:255)

// <o> KEY_LONG_TIME - �ظ�����ʱ��
// <i> �����ظ�����ʱ�� ���Ϊ15
// <0-15>
#define KEY_REPEAT_TIME 15 // (max:15)

// <o> KEY_MODE  - ����ģʽ
// <i> �ఴ��������ͬһʱ�̿��Դ����������
// <i> ������������ͬһʱ��ֻ�ܴ���һ������
// <0=> �ఴ������
// <1=> ����������
#define KEY_MODE 0 // 0:�ఴ��ͬʱ���� 1�����������������������²��ͷţ������������ͷţ���������

// <<< end of configuration section >>>

typedef enum
{
    DEBOUNCE = 0,    // �ȴ�
    PressStatus,     // �а�������
    LongPressStatus, //����״̬
    RepeatStatus,    //�ظ�״̬
} Key_status_enum;   //�����˰���״̬ ���ڰ���ɨ����ɨ�貽��

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
    uint8_t key_down : 1;         //��������
    uint8_t key_up : 1;           //�����ɿ�
    uint8_t click_up : 1;         //��� ������Ч
    uint8_t click_down : 1;       //��� ������Ч
    uint8_t key_long_pressed : 1; //��������
    uint8_t key_repeat : 1;       //�����ظ�
    uint8_t valid : 1;            //��Ч
} KEY_EVENT_T;

typedef struct MULTI_KEY
{
    uint8_t filter_conut : 4; //��������
    uint8_t scan_status : 3;  //ɨ��״̬
    uint8_t is_key_down : 1;  //�ж��Ƿ��� ���ⲿ���û�����
    uint8_t repeat_time : 4;  //�ظ������ж�ʱ��
    uint8_t repeat_count : 4; //�ظ���������
    KEY_EVENT_T event;        //�����¼�
    uint8_t long_count;       //��������
    uint8_t long_time;        //�����ж�ʱ��
} MULTI_KEY_T;

extern xdata MULTI_KEY_T mult_key_array[];
extern void multi_key_init(void);
extern void key_stop(MULTI_KEY_T *handle);
extern void key_scan(void);

#endif
