/**
 * @file bsp_key.c
 * @author 华视微科技
 * @brief 按键读取函数
 * @version 0.1
 * @date 2022-09-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "bsp_gpio.h"

/**
 * @brief 按键初始化
 *
 * @param num 按键号
 */
void Key_Init(unsigned char num)
{
    switch (num)
    {
    case 1:
        IO_Init(0, 0, 2, 0, 1);
        break;
    case 2:
        IO_Init(1, 0, 2, 0, 1);
        break;
    default:
        break;
    }
}

/**
 * @brief 按键状态读取
 *
 * @param num 按键序号
 * @return 按键状态【1按下，0未按下】
 */
unsigned char Key_Read(unsigned char num)
{
    u8 val = 0;
    switch (num)
    {
    case 1:
    {
        if (!IO_Read(0))
        {
            for (unsigned int i = 0; i < 10000; i++)
                ;
            if (!IO_Read(0))
            {
                val = 1;
            }
        }
    }
    break;

    case 2:
    {
        if (!IO_Read(1))
        {
            for (unsigned int i = 0; i < 10000; i++)
                ;
            if (!IO_Read(1))
            {
                val = 1;
            }
        }
    }
    break;

    default:
        break;
    }
    return val;
}