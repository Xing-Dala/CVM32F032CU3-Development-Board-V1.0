/**
 * @file main.c
 * @author XingDala
 * @brief SPI方式读写CV663
 * @version 0.1
 * @date 2023-01-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"
#include "PCD.h"

int main(void)
{
    Delay_s(1);

    printf("CV627 Demo!\r\n");

    //Buzzer_Init();

    SLED_Init();

    CV663_Init();

    PcdConfig_CV663(PCD_TYPE_15693);

    do
    {
        CV663_Read_Card();
    } while (1);
}