

#ifndef __KEY_H
#define __KEY_H

#include "multi_key.h"
#include "multi_tim.h"

#define key1 mult_key_array[0]
#define key2 mult_key_array[1]
#define key1_tim tim_head_handle[0]
#define key2_tim tim_head_handle[1]




void key_process(void);
void Sys_Scan(void);
#endif 
