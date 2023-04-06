/**
 * @file main.h
 * @author XingDala
 * @brief
 * @version 0.1
 * @date 2023-01-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <string.h>

#include "xinc_m0.h"

#include "bsp_gpio.h"
#include "bsp_timer.h"
#include "bsp_uart.h"
#include "bsp_wdog.h"
#include "bsp_register_macro.h"
#include "bsp_com_spi.h"
#include "bsp_spi_master.h"

#include "ble.h"
#include "BT.h"
#include "btstack_defines.h"
#include "xinchip_ota.h"

#include "M0_Delay.h"

#include "CV663.h"
#include "Status.h"
#include "ISO14443.h"

#include "Buzzer.h"

#include "LED.h"

/*
*********************************************************************************************************
*            PLATFORM DEFINES
*********************************************************************************************************
*/
#define CORE_CLK      32000000ul /* Set Processor frequency */
#define TICKS_PER_SEC 100ul      /* Set the number of ticks in one second  */

/*
*********************************************************************************************************
*         	GLOBAL FUNCTION
*********************************************************************************************************
*/
#define AT_USE_UART 1
#define AT_BUFF_LEN 100

#define PIN_32_16

#define __ASSERT(a) while (!(a))

#endif
