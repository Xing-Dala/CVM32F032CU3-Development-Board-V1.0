/**
 * @file main.c
 * @author XingDala
 * @brief SPI方式读写CV663，2秒读出16扇区，平均每扇区125毫秒
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

    //Buzzer_Init();

    SLED_Init();

    CV663_Init();

    PcdRc663_Lpcd_GetCfg(&I, &Q);

    PcdRc663_Cmd_Lpcd(I, Q, 300, CHECK_T);

    do {
        if (!CV663_IRQ_Read) {
            CV663_Read_Card();
            printf("12");
        }
    } while (1);
}