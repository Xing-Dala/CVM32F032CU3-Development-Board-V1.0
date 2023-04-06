#ifndef _BSP_WDOG_H
#define _BSP_WDOG_H

#include "main.h"

void xc_wdog_init(uint32_t counter_value); //看门狗初始化
void xc_wdog_feed(void);                   //喂狗

#endif
