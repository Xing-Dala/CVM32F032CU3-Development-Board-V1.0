#include "Platform.h"

/* ---------------------------------------------------------------------------------------------------
- 函数名称: xc_pwm_init
- 函数功能: pwm初始化
- 输入参数: pwm口（0或1）,占空比设置（范围0--99），周期计数器设置影响pwm信号输出频率(输出频率=pwm_clk/((period + 1)*100))
- 创建日期: 2019-04-22
- 作    者：陈俊伟
----------------------------------------------------------------------------------------------------*/
void xc_set_pwm(uint8_t ch, uint8_t ocpy_ratio, uint16_t period)
{
    __write_hw_reg32(PWMx_EN(ch), 0x0);          //暂停相应的 PWM 周期计数器和占空比计数器
    __write_hw_reg32(PWMx_P(ch), period);        // pwm周期设置寄存器--等于pwm_clk/((period + 1)*100)
    __write_hw_reg32(PWMx_OCPY(ch), ocpy_ratio); // pwm占空比设置寄存器
    __write_hw_reg32(PWMx_UP(ch), 0x1);          //更新周期寄存器和占空比寄存器。
    __write_hw_reg32(PWMx_EN(ch), 0x1);          //使能相应的 PWM 周期计数器和占空比计数器
}

/*
PWM 输出频率计算公式为
Fout= 160K/((div+1)*(period + 1))
Fout   : 输出频率
div    : PWM 时钟分频比(范围0-255)
period : PWM 周期设置  (范围0-255)
*/
void xc_pwm_init(uint8_t ch, uint8_t ocpy_ratio, uint16_t period)
{
    //注：管脚配置在Init_gpio函数中进行

    __write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL, 0x10001000); // PWM_PCLK 时钟使能
    __write_hw_reg32(CPR_PWM_CLK_CTL, 0x80000009);       // pwm_clk=mclk_in(32M)/(2*(9+1))=1.6M

    /*
   __write_hw_reg32(CPR_PWM_CLK_CTL,0x80000000); //pwm_clk=mclk_in(32M)/(2*(0 +1))= 16M ; max output HZ 160KHZ
   __write_hw_reg32(CPR_PWM_CLK_CTL,0x80000001); //pwm_clk=mclk_in(32M)/(2*(1 +1))=  8M ; max output HZ  80KHZ
   __write_hw_reg32(CPR_PWM_CLK_CTL,0x80000003); //pwm_clk=mclk_in(32M)/(2*(3 +1))=  4M ; max output HZ  40KHZ
   __write_hw_reg32(CPR_PWM_CLK_CTL,0x80000007); //pwm_clk=mclk_in(32M)/(2*(7 +1))=  2M ; max output HZ  20KHZ
   __write_hw_reg32(CPR_PWM_CLK_CTL,0x80000009); //pwm_clk=mclk_in(32M)/(2*(9 +1))=1.6M ; max output HZ  16KHZ
   __write_hw_reg32(CPR_PWM_CLK_CTL,0x8000000F); //pwm_clk=mclk_in(32M)/(2*(15+1))=  1M ; max output HZ  10KHZ
   __write_hw_reg32(CPR_PWM_CLK_CTL,0x8000004F); //pwm_clk=mclk_in(32M)/(2*(79+1))=0.2M ; max output HZ   2KHZ
   */
    xc_set_pwm(ch, ocpy_ratio, period);
}