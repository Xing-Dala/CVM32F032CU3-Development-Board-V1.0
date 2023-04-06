#include "main.h"

static uint8_t __attribute__((aligned(4))) uart_tx_buf[256] = {0};
static uint8_t __attribute__((aligned(4))) uart_rx_buf[256] = {0};

#define CPR_BASE                0x40000000
#define UART0_BASE              0x40010000
#define UART1_BASE              0x40011000

#define CPR_UARTx_CLK_GRCTL(a)  ((volatile unsigned *)(CPR_BASE + 0x30 + (a * 0x08)))
#define CPR_UARTx_CLK_CTL(a)    ((volatile unsigned *)(CPR_BASE + 0x34 + (a * 0x08)))
#define UARTx_RBR(a)            ((volatile unsigned *)(UART0_BASE + 0x00 + (a * 0x1000)))
#define UARTx_THR(a)            ((volatile unsigned *)(UART0_BASE + 0x00 + (a * 0x1000)))
#define UARTx_DLL(a)            ((volatile unsigned *)(UART0_BASE + 0x00 + (a * 0x1000)))
#define UARTx_IER(a)            ((volatile unsigned *)(UART0_BASE + 0x04 + (a * 0x1000)))
#define UARTx_DLH(a)            ((volatile unsigned *)(UART0_BASE + 0x04 + (a * 0x1000)))
#define UARTx_IIR(a)            ((volatile unsigned *)(UART0_BASE + 0x08 + (a * 0x1000)))
#define UARTx_FCR(a)            ((volatile unsigned *)(UART0_BASE + 0x08 + (a * 0x1000)))
#define UARTx_TCR(a)            ((volatile unsigned *)(UART0_BASE + 0x0c + (a * 0x1000)))
#define UARTx_MCR(a)            ((volatile unsigned *)(UART0_BASE + 0x10 + (a * 0x1000)))
#define UARTx_TSR(a)            ((volatile unsigned *)(UART0_BASE + 0x14 + (a * 0x1000)))
#define UARTx_MSR(a)            ((volatile unsigned *)(UART0_BASE + 0x18 + (a * 0x1000)))
#define UARTx_USR(a)            ((volatile unsigned *)(UART0_BASE + 0x7c + (a * 0x1000)))

#define UART0_RBR               ((volatile unsigned *)(UART0_BASE + 0x00))
#define UART0_THR               ((volatile unsigned *)(UART0_BASE + 0x00))
#define UART0_DLL               ((volatile unsigned *)(UART0_BASE + 0x00))
#define UART0_IER               ((volatile unsigned *)(UART0_BASE + 0x04))
#define UART0_DLH               ((volatile unsigned *)(UART0_BASE + 0x04))
#define UART0_IIR               ((volatile unsigned *)(UART0_BASE + 0x08))
#define UART0_FCR               ((volatile unsigned *)(UART0_BASE + 0x08))
#define UART0_TCR               ((volatile unsigned *)(UART0_BASE + 0x0c))
#define UART0_MCR               ((volatile unsigned *)(UART0_BASE + 0x10))
#define UART0_TSR               ((volatile unsigned *)(UART0_BASE + 0x14))
#define UART0_MSR               ((volatile unsigned *)(UART0_BASE + 0x18))
#define UART0_USR               ((volatile unsigned *)(UART0_BASE + 0x7c))
#define UART1_RBR               ((volatile unsigned *)(UART1_BASE + 0x00))
#define UART1_THR               ((volatile unsigned *)(UART1_BASE + 0x00))
#define UART1_DLL               ((volatile unsigned *)(UART1_BASE + 0x00))
#define UART1_IER               ((volatile unsigned *)(UART1_BASE + 0x04))
#define UART1_DLH               ((volatile unsigned *)(UART1_BASE + 0x04))
#define UART1_IIR               ((volatile unsigned *)(UART1_BASE + 0x08))
#define UART1_FCR               ((volatile unsigned *)(UART1_BASE + 0x08))
#define UART1_TCR               ((volatile unsigned *)(UART1_BASE + 0x0c))
#define UART1_MCR               ((volatile unsigned *)(UART1_BASE + 0x10))
#define UART1_TSR               ((volatile unsigned *)(UART1_BASE + 0x14))
#define UART1_MSR               ((volatile unsigned *)(UART1_BASE + 0x18))
#define UART1_USR               ((volatile unsigned *)(UART1_BASE + 0x7c))

#define CPR_RSTCTL_CTLAPB_SW    ((volatile unsigned *)(CPR_BASE + 0x100))
#define CPR_RSTCTL_SUBRST_SW    ((volatile unsigned *)(CPR_BASE + 0x104))
#define CPR_RSTCTL_M0RST_SW     ((volatile unsigned *)(CPR_BASE + 0x108))
#define CPR_RSTCTL_M0RST_MASK   ((volatile unsigned *)(CPR_BASE + 0x10c))
#define CPR_RSTCTL_LOCKUP_STATE ((volatile unsigned *)(CPR_BASE + 0x110))
#define CPR_RSTCTL_WDTRST_MASK  ((volatile unsigned *)(CPR_BASE + 0x114))
#define CPR_LP_CTL              ((volatile unsigned *)(CPR_BASE + 0x118))
#define CPR_CTLAPBCLKEN_GRCTL   ((volatile unsigned *)(CPR_BASE + 0x070))

uHandler_callback uHandler_Callback[2] = {(uHandler_callback)0, (uHandler_callback)0};

/**
 * @brief UART初始化
 *
 * @param ch 初始化串口号(0:uart0 1:uart1);
 * @param baud 波特率
 */
extern void Uart_Init(uint32_t ch, uint32_t baud)
{
    uint32_t val;

    __write_hw_reg32(CPR_RSTCTL_SUBRST_SW, (1 << (16 + ch)));               // 复位uart模块
    __write_hw_reg32(CPR_RSTCTL_SUBRST_SW, ((1 << (16 + ch)) | (1 << ch))); // 解复位uart模块

    __read_hw_reg32(CPR_LP_CTL, val);
    val &= ~(1 << (1 - ch));
    __write_hw_reg32(CPR_LP_CTL, val); // 关闭uartx时钟保护

    val = (1 << (16 + ch + 4)) | (1 << (ch + 4));
    __write_hw_reg32(CPR_CTLAPBCLKEN_GRCTL, val); // 打开uartx pclk
    __write_hw_reg32(CPR_UARTx_CLK_GRCTL(ch), 0x110018);
    //__write_hw_reg32(CPR_UARTx_CLK_CTL(ch), 0x480271);
    __write_hw_reg32(CPR_UARTx_CLK_CTL(ch), baud >> 4);

    __write_hw_reg32(UARTx_TCR(ch), 0x80); // 使能DLAB位
    //__write_hw_reg32(UARTx_DLL(ch) , baud);
    __write_hw_reg32(UARTx_DLL(ch), baud & 0x0F);
    __write_hw_reg32(UARTx_DLH(ch), 0);

    __write_hw_reg32(UARTx_TCR(ch), 0x03); // 清零DLAB位,8个BIT位
    __write_hw_reg32(UARTx_FCR(ch), 0xb7); // 接收FIFO半满报中断, 并使能FIFO中断

#ifndef UART_DMA
    __write_hw_reg32(UARTx_IER(ch), 0x01); // 打开接收中断
    val = UART0_IRQn + ch;
    NVIC_EnableIRQ((IRQn_Type)val);
#else
    // if (ch == 1)
    // 	Uart_DMA_Send(1, "\nuse uart1 dma!\n", sizeof("\nuse uart1 dma!\n") - 1);
    // else
    // 	Uart_DMA_Send(0, "\nuse uart0 dma!\n", sizeof("\nuse uart0 dma!\n") - 1);
#endif
}

/**
 * @brief 为uart注册中断回调函数
 *
 * @param ch 串口号
 * @param callback 回调函数
 */
extern void Uart_Register_Callback(uint32_t ch, uHandler_callback callback)
{
    uHandler_Callback[ch] = callback;
}

/**
 * @brief UART0中断处理函数
 *
 */
extern void UART0_Handler(void)
{
    uint32_t iWK = 0;
    uint8_t tWK  = 0;

    __read_hw_reg32(UART0_IIR, iWK);
    iWK &= 0x0F;

    if ((iWK != 0x04) && (iWK != 0x0c))
        return;

    __read_hw_reg32(UART0_RBR, tWK);

    if (uHandler_Callback[0] != (uHandler_callback)0)
        (uHandler_Callback[0])(tWK);
}

/**
 * @brief UART1中断处理函数
 *
 */
extern void UART1_Handler(void)
{
    uint32_t iWK = 0;
    uint8_t tWK  = 0;

    __read_hw_reg32(UART1_IIR, iWK);

    iWK &= 0x0F;

    if ((iWK != 0x04) && (iWK != 0x0c))
        return;

    __read_hw_reg32(UART1_RBR, tWK);

    if (uHandler_Callback[1] != (uHandler_callback)0)
        (uHandler_Callback[1])(tWK);
}

/**
 * @brief Uart发送一个字符
 *
 * @param ch 串口号
 * @param c 要发送的字符
 */
extern void Uart_Send_Char(uint32_t ch, uint8_t c)
{
    unsigned int status;

    for (;;) {
        __read_hw_reg32(UARTx_TSR(ch), status);

        status &= 0x20;

        if (status == 0x20)
            break;
    }

    __write_hw_reg32(UARTx_THR(ch), c);
}

/**
 * @brief Uart发送一个字符串
 *
 * @param ch 串口号
 * @param s 要发送的字符串
 */
extern void Uart_Send_String(uint32_t ch, uint8_t *s)
{
    while (*s != '\0') {
        Uart_Send_Char(ch, *s++);
    }
}

/**
 * @brief Uart发送一个长度为length的uint8_t类型的数组
 *
 * @param ch 串口号
 * @param s 要发送的数组
 * @param length 数组长度
 */
extern void Uart_Send_Buf(uint32_t ch, uint8_t *s, uint32_t length)
{
    while (length--)
        Uart_Send_Char(ch, *s++);
}

/**
 * @brief Uart用DMA方式发送一个长度为length的uint8_t类型的数组  --注意DMA的只能访问SHRAM0（0x10010000 ~ 0x1001FFFF）
 *
 * @param ch 串口号
 * @param s 要发送的数组
 * @param length 数组长度
 */
extern void Uart_DMA_Send(uint8_t ch, uint8_t *s, uint32_t length)
{
    for (int i = 0; i < length; i++)
        uart_tx_buf[i] = s[i];

    uint32_t iWK = 0;

    //- TX Channel
    __write_hw_reg32(DMAS_CHx_SAR(ch), (uint32_t)uart_tx_buf);
    __write_hw_reg32(DMAS_CHx_DAR(ch), (ch * 0x1000 + 0x40010000)); // 发送地址写UART_THR
    __write_hw_reg32(DMAS_CHx_CTL1(ch), ((2 << 8)));                // 通道优先级2
    __write_hw_reg32(DMAS_CHx_CTL0(ch), length);
    __write_hw_reg32(DMAS_EN, ch);

    do {
        __read_hw_reg32(DMAS_INT_RAW, iWK);
    } while ((iWK & (1 + ch)) != (1 + ch));

    __write_hw_reg32(DMAS_INT_RAW, (1 + ch));
    __write_hw_reg32(DMAS_CLR, ch);
}

/**
 * @brief Uart轮询方式接收-使用此方式也是不需要开启中断接收的
 *
 * @param ch 串口号
 * @param s 接收的数组缓冲区
 * @param length 数组长度
 */
extern void Uart_Polling_Rev(uint8_t ch, uint32_t length, uint8_t *s)
{
    unsigned int status, i = 0;

    while (1) {
        __read_hw_reg32(UARTx_TSR(ch), status);

        status &= 0x01;

        if (status == 0x01) {
            uint8_t tWK = 0;

            __read_hw_reg32(UARTx_RBR(ch), tWK);

            s[i++] = tWK;
        }

        if (i == length)
            break;
    }
}

extern uint8_t Uart_Rev(uint8_t ch)
{
    unsigned int status, i = 0;

    while (1) {
        __read_hw_reg32(UARTx_TSR(ch), status);

        status &= 0x01;

        if (status == 0x01) {
            uint8_t tWK = 0;

            __read_hw_reg32(UARTx_RBR(ch), tWK);

            return tWK;

            break;
        }
    }
}