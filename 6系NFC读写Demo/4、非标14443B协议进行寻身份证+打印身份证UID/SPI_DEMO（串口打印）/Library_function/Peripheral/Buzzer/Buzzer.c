/**
 * @file Buzzer.c
 * @author 张逸逵
 * @brief 蜂鸣器库函数
 * @version 0.1
 * @date 2022-12-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "main.h"

#define buzzerpin 11

/**
 * @brief 蜂鸣器初始化函数
 *
 */
void Buzzer_Init(void)
{
    gpio_mux_ctl(buzzerpin, 1);
    gpio_fun_sel(buzzerpin, 0);
    gpio_direction_output(buzzerpin);

    Buzzer_Beep(Buzzer_True);
}

/**
 * @brief 读卡提示音
 *
 */
void Buzzer_Beep(u8 I)
{
    if (I==1) {
        for (int i = 0; i < 250; i++) {
            gpio_output_high(buzzerpin);
            Delay_us(150);
            gpio_output_low(buzzerpin);
            Delay_us(150);
        }

        Delay_ms(200);

        for (int i = 0; i < 200; i++) {
            gpio_output_high(buzzerpin);
            Delay_us(100);
            gpio_output_low(buzzerpin);
            Delay_us(100);
        }
    } else if (I==0){
        for (int i = 0; i < 250; i++) {
            gpio_output_high(buzzerpin);
            Delay_us(100);
            gpio_output_low(buzzerpin);
            Delay_us(100);
        }

        Delay_ms(200);

        for (int i = 0; i < 150; i++) {
            gpio_output_high(buzzerpin);
            Delay_us(100);
            gpio_output_low(buzzerpin);
            Delay_us(100);
        }
    }
}