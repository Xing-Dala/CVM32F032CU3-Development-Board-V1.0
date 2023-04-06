/**
 * @file main.c
 * @author XingDala
 * @brief UART方式读写CV663，最快26秒读出一张M1卡的16个扇区，平均每个扇区1.6秒
 * @version 0.1
 * @date 2023-01-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"

int main(void)
{
    Delay_s(1);

    printf("CV627 Demo!\r\n");

    Buzzer_Init();

    SLED_Init();

    CV663_Init();

    do {
    CV663_Read_Card();
    } while (1);
}