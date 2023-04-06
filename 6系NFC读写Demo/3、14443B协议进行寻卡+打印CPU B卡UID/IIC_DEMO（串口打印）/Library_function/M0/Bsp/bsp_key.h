/**
 * @file bsp_key.h
 * @author 华视微科技
 * @brief 按键读取函数
 * @version 0.1
 * @date 2022-09-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __BSP_KEY_H_
#define __BSP_KEY_H_

#include "main.h"

extern void Key_Init(unsigned char num);
extern unsigned char Key_Read(unsigned char num);

#endif