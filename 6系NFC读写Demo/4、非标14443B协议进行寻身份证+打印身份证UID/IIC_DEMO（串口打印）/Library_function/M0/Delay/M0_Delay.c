/**
 * @file M0_Delay.c
 * @author Yikui Zhang (xingdala@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-10-12
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "main.h"

void Delay_us(u16 time)
{
    time *= 4.6;
    for (time; time > 0; time--)
        ;
}

void Delay_ms(u16 time)
{
    for (time; time > 0; time--)
        for (u16 i = 0; i < 4600; i++)
            ;
}

void Delay_s(u16 time)
{
    for (time; time > 0; time--)
        for (u32 i = 0; i < 4600000; i++)
            ;
}