#include "main.h"
#include "bsp_register_macro.h"

///**
// * @brief 中断函数
// *
// * @param timer_num 定时器
// * @param us 多少秒一次中断
// */
//void xc_timer_init(uint8_t timer_num, uint32_t us)
//{
//	__write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL, 0x80008);	  // TIMER_PCLK 时钟使能
//	__write_hw_reg32(CPR_TIMER_CLK_CTL(timer_num), 0x0F); // TIMERx_CLK 时钟控制寄存器 mclk_in(32MHz)/2*(0x0F + 0x1)
//	__write_hw_reg32(TIMERx_TCR(timer_num), 0x0);		  //不屏蔽定时器中断，不使能定时器timer_num，
//	__write_hw_reg32(TIMERx_TCR(timer_num), 0x2);		  //设置定时器工作在用户定义计数模式
//	__write_hw_reg32(TIMERx_TLC(timer_num), us);		  //载入计数器计数初值(32bits),该值应大于等于 0x4
//	__write_hw_reg32(TIMERx_TCR(timer_num), 0x3);		  //使能定时器timer_num
//	NVIC_EnableIRQ((IRQn_Type)(TIMER0_IRQn + timer_num));
//}

///**
// * @brief 定时器2中断服务函数
// *
// */
//void TIMER2_Handler(void)
//{
//	u32 val = 0;
//	__read_hw_reg32(TIMER2_TIC, val);
//	//需要处理的定时器任务

//	printf("TIMER2_Handler\n");

//	//__write_hw_reg32(TIMERx_TCR(2), 0x0);
//}

///**
// * @brief 定时器3中断服务函数
// *
// */
//void TIMER3_Handler(void)
//{
//	u32 val = 0;
//	__read_hw_reg32(TIMER3_TIC, val);
//	//需要处理的定时器任务
//	printf("TIMER3_Handler\n");

//	extern void xc_gpadc_handle(void);
//	xc_gpadc_handle();
//}
