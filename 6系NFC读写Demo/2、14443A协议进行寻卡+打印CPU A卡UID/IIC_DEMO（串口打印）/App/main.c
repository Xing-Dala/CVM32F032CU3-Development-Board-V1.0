/**
 * @file main.c
 * @author XingDala
 * @brief I2C方式读写RFID_6，6秒一次性读取16扇区，平均每个扇区375毫秒
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

    RFID_6_Init();

    do
    {
        RFID_6_Read_Card();
    } while (1);
}