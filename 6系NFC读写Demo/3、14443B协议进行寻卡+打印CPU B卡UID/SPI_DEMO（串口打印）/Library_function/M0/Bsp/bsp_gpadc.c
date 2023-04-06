#include "Platform.h"

/**
 * @brief ADC引脚配置
 *
 * @param channels 16bits 0---8bit位代表0--8通道，需要配置就对相应的bit位置1
 */
void adc_gpio_config(uint16_t channels)
{
	if (channels == 0) //通道0--GADC_AIN0--GPIO21
	{
		gpio_mux_ctl(21, 0);
		gpio_fun_sel(21, 0);
		gpio_fun_inter(21, 0);
		gpio_direction_input(21, 3);
	}
	else if (channels == 1) //通道1--GADC_AIN0--GPIO20
	{
		gpio_mux_ctl(20, 0);
		gpio_fun_sel(20, 0);
		gpio_fun_inter(20, 0);
		gpio_direction_input(20, 3);
	}
	else if (channels == 2) //通道2--GADC_AIN0--GPIO19
	{
		gpio_mux_ctl(19, 0);
		gpio_fun_sel(19, 0);
		gpio_fun_inter(19, 0);
		gpio_direction_input(19, 3);
	}
	else if (channels == 3) //通道3--GADC_AIN0--GPIO18
	{
		gpio_mux_ctl(18, 0);
		gpio_fun_sel(18, 0);
		gpio_fun_inter(18, 0);
		gpio_direction_input(18, 3);
	}
	else if (channels == 4) //通道4--GADC_AIN0--GPIO0
	{
		gpio_mux_ctl(0, 0);
		gpio_fun_sel(0, 0);
		gpio_fun_inter(0, 0);
		gpio_direction_input(0, 3);
	}
	else if (channels == 5) //通道5--GADC_AIN0--GPIO1
	{
		gpio_mux_ctl(1, 0);
		gpio_fun_sel(1, 0);
		gpio_fun_inter(1, 0);
		gpio_direction_input(1, 3);
	}
	else if (channels == 6) //通道6--GADC_AIN0--GPIO4
	{
		gpio_mux_ctl(4, 0);
		gpio_fun_sel(4, 0);
		gpio_fun_inter(4, 0);
		gpio_direction_input(4, 3);
	}
	else if (channels == 7) //通道7--GADC_AIN0--GPIO5
	{
		gpio_mux_ctl(5, 0);
		gpio_fun_sel(5, 0);
		gpio_fun_inter(5, 0);
		gpio_direction_input(5, 3);
	}
	else if (channels == 8) //芯片供电电压的2/3 例如芯片供电为3.3V那么通道8采集电压为2.2V
	{
	}
}

/**
 * @brief ADC初始化
 *
 */
extern void Init_adc(void)
{
	__write_hw_reg32(CPR_RSTCTL_CTLAPB_SW, 0x10000000); /*先 使GPADC_RSTN=0，再使 GPADC_RSTN=1，软复位 GPADC 模块*/
	__write_hw_reg32(CPR_RSTCTL_CTLAPB_SW, 0x10001000); /*先 使GPADC_RSTN=0，再使 GPADC_RSTN=1，软复位 GPADC 模块*/

	__write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL, 0x20002000); //使能GPADC_PCLK_EN 的GPADC_PCLK时钟

	__write_hw_reg32(GPADC_FIFO_CTL, 0x10); //对 FIFO 进 行 一 次 清 空 操 作
	__write_hw_reg32(GPADC_FIFO_CTL, 0x00); //

	//__write_hw_reg32(GPADC_RF_CTL , 0x812);// bit1为gadc_vref_sel选择位。(0：选择2.47V基准(参考)电压; 1:选择AVDD(3.3V)作为基准(参考)电压)
	__write_hw_reg32(GPADC_RF_CTL, 0x810);	// 配置寄存器 GPADC_RF_CTL   GPADC_PCLK/(gpadc_clkdiv*2)=16M/16=1M ,select 2.4V vref
	__write_hw_reg32(GPADC_MAIN_CTL, 0x09); // 配置寄存器 GPADC_MAIN_CTL 打开GPADC模块，，数据采集上升沿
}
