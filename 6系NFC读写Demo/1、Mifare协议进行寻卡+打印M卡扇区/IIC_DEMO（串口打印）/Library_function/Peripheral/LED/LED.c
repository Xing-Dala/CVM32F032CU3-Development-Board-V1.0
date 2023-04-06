/**
 * @file key.c
 * @author 张逸逵 (xingdala@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-01-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"

void SLED_Init(void)
{
    gpio_fun_sel(22, GPIO_Dx);
    gpio_direction_output(22);

    gpio_fun_sel(23, GPIO_Dx);
    gpio_direction_output(23);

    gpio_fun_sel(24, GPIO_Dx);
    gpio_direction_output(24);

    gpio_fun_sel(25, GPIO_Dx);
    gpio_direction_output(25);

    gpio_output_low(22);
    gpio_output_low(23);
    gpio_output_low(24);
    gpio_output_low(25);

    Delay_ms(300);
    gpio_output_high(22);
    gpio_output_high(23);
    gpio_output_high(24);
    gpio_output_high(25);
}

/**
 * @brief 蓝灯
 *
 * @param i
 */
void LED1(uint8_t i)
{
    if (i)
        gpio_output_low(22);
    else
        gpio_output_high(22);
}

/**
 * @brief 绿灯
 *
 * @param i
 */
void LED2(uint8_t i)
{
    if (i)
        gpio_output_low(23);
    else
        gpio_output_high(23);
}

/**
 * @brief 黄灯
 *
 * @param i
 */
void LED3(uint8_t i)
{
    if (i)
        gpio_output_low(24);
    else
        gpio_output_high(24);
}

/**
 * @brief 红灯
 *
 * @param i
 */
void LED4(uint8_t i)
{
    if (i)
        gpio_output_low(25);
    else
        gpio_output_high(25);
}