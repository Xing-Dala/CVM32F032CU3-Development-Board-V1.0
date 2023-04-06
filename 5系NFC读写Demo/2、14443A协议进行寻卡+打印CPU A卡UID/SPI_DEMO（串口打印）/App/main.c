/**
 * @file main.c
 * @author XingDala
 * @brief SPI方式读写CV520，2秒读出16扇区，平均每扇区125毫秒
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

    printf("CV520 Demo!\r\n");

    Buzzer_Init();

    SLED_Init();

    CV520_Init();

    do
    {
        CV520_Read_Card();
    } while (1);
}