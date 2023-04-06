/**
 * @file CV663.c
 * @author Yikui Zhang (xingdala@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-03-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"
#include "PCD.h"

tpd_pcdpara g_sPcdPara;
tpd_pcdtrsv g_sPcdTrsv;

/**
 * @brief 读卡参数初始化
 *
 */
uint8_t g_cATS[32]; // Card serial number buffer
uint8_t g_cATSLen;  // Card serial number buffer length

/**
 * @brief CV663模拟SPI初始化
 *
 */
void CV663_SPI_Init(void)
{

    gpio_mux_ctl(CV663_CLK_Pin, 0);
    gpio_direction_output(CV663_CLK_Pin); // SPI_CLK

    gpio_mux_ctl(CV663_CS_Pin, 0);
    gpio_direction_output(CV663_CS_Pin); // SPI_CS

    gpio_mux_ctl(CV663_MOSI_Pin, 0);
    gpio_direction_output(CV663_MOSI_Pin); // SPI_MOSI

    gpio_mux_ctl(CV663_MISO_Pin, 0);
    gpio_direction_input(CV663_MISO_Pin, 0); // SPI_MISO

    gpio_mux_ctl(CV663_RST_Pin, 0);
    gpio_direction_output(CV663_RST_Pin); // SPI_RST

    gpio_mux_ctl(CV663_IRQ_Pin, 0);
    gpio_direction_input(CV663_IRQ_Pin, 0); // SPI_IRQ

    gpio_mux_ctl(CV663_SL0_Pin, 0);
    gpio_direction_output(CV663_SL0_Pin); // SL0

    gpio_mux_ctl(CV663_SL1_Pin, 0);
    gpio_direction_output(CV663_SL1_Pin); // SL1

    gpio_output_low(CV663_SL0_Pin);
    gpio_output_high(CV663_SL1_Pin);
}

/**
 * @brief CV663 communication initialization
 *
 */
void CV663_Init(void)
{
    CV663_SPI_Init(); // CV663_SPI initialization

    uint8_t Status; // Task process state storage

    Status = PcdReset_CV663(); // Restart the antenna

    if (Status == ERR_SUCCESS)            // Determine whether the antenna reset is successful
        PcdConfig_CV663(PCD_TYPE_14443A); // The antenna is initialized successfully, and CV663 initial card reading parameters are configured
}

/**
 * @brief Reset and initialize CV663
 *
 * @return unsigned char Status
 */
unsigned char PcdReset_CV663(void)
{
    unsigned char Status = MI_OK;
    unsigned char n;
    unsigned int Delay = 0xFFFF;

    CV663_RST_Hight;

    Delay_ms(5);

    CV663_RST_Low;

    do {
        n = Read_CV663(0x00);
        Delay--;
    } while (Delay != 0 && n != 0x40);

    if (Delay == 0)
        Status = MI_RESETERR;
    else
        n = Read_CV663(PHHAL_HW_CV663_REG_VERSION);

    return Status;
}

/**
 * @brief Read the register at the specified address of CV663 and return the data stored at that address.
 *
 * @param Address
 * @return unsigned char iTemp
 */
unsigned char Read_CV663(unsigned char Address)
{
    unsigned char cValue = 0; // Task process state storage

    CV663_MOSI_Hight;

    Address <<= 1;
    Address |= 0x01;

    CV663_CLK_Low;
    CV663_CS_Low;

    for (unsigned char cCnt = 0; cCnt < 8; cCnt++) {
        if (Address & 0x80)
            CV663_MOSI_Hight;
        else
            CV663_MOSI_Low;

        CV663_CLK_Hight;

        Address <<= 1;

        CV663_CLK_Low;
    }

    CV663_MOSI_Hight;

    for (unsigned char cCnt = 0; cCnt < 8; cCnt++) {
        cValue <<= 1;

        CV663_CLK_Hight;

        if (CV663_MISO_Read)
            cValue |= 0x01;

        CV663_CLK_Low;
    }

    CV663_CLK_Low;

    CV663_CS_Hight;

    return cValue;
}

/**
 * @brief Write data to the specified address of CV663 register.
 *
 * @param Address
 * @param Data
 */
void Write_CV663(unsigned char Address, unsigned char Data)
{
    CV663_MISO_Low;

    Address <<= 1;
    Address &= 0x7E;
    Data &= 0xFF;

    CV663_CLK_Low;
    CV663_CS_Low;

    for (uint8_t cCnt = 0; cCnt < 8; cCnt++) {
        if (Address & 0x80)
            CV663_MOSI_Hight;
        else
            CV663_MOSI_Low;

        CV663_CLK_Hight;

        Address <<= 1;

        CV663_CLK_Low;
    }

    for (uint8_t cCnt = 0; cCnt < 8; cCnt++) {
        if (Data & 0x80)
            CV663_MOSI_Hight;
        else
            CV663_MOSI_Low;

        CV663_CLK_Hight;

        Data <<= 1;

        CV663_CLK_Low;
    }

    CV663_MOSI_Low;
    CV663_CS_Hight;

    return;
}

/**
 * @brief
 *
 * @param reg
 * @param mask
 */
void SetBitMask_CV663(unsigned char reg, unsigned char mask)
{
    unsigned char tmp = Read_CV663(reg);

    Write_CV663(reg, tmp | mask); // set bit mask

    return;
}

/**
 * @brief
 *
 * @param reg
 * @param mask
 */
void ClearBitMask_CV663(unsigned char reg, unsigned char mask)
{
    unsigned char tmp = Read_CV663(reg);

    Write_CV663(reg, tmp & ~mask); // clear bit mask

    return;
}

/**
 * @brief
 *
 * @param cFWI
 */
void PcdSetTmo_CV663(unsigned char cFWI)
{
    unsigned int cClock0 = 4096;
    unsigned int cClock1;
    unsigned char cTpcd = 2;

    cClock1 = (1 << cFWI) + 12 + cTpcd; // 计算方法

    Write_CV663(PHHAL_HW_CV663_REG_T0RELOADHI, (unsigned char)(cClock0 >> 8));
    Write_CV663(PHHAL_HW_CV663_REG_T0RELOADLO, (unsigned char)cClock0);
    Write_CV663(PHHAL_HW_CV663_REG_T1RELOADHI, (unsigned char)(cClock1 >> 8));
    Write_CV663(PHHAL_HW_CV663_REG_T1RELOADLO, (unsigned char)cClock1);
    //	Write_CV663(PHHAL_HW_CV663_REG_T0CONTROL, 0x98); // 时钟源为13.56
    //	Write_CV663(PHHAL_HW_CV663_REG_T1CONTROL, 0x9A); // 时钟源为Timer0
    Write_CV663(PHHAL_HW_CV663_REG_TCONTROL, 0x33);

    return;
}

/**
 * @brief
 *
 * @param RxPtl
 * @param TxPtl
 * @return unsigned char
 */
unsigned char phhalHw_CV663_Cmd_LoadProtocol(unsigned char RxPtl, unsigned char TxPtl)
{
    unsigned char cStatus = MI_OK;

    PcdSetTmo_CV663(0);

    g_sPcdTrsv.cData[0] = RxPtl;
    g_sPcdTrsv.cData[1] = TxPtl;
    g_sPcdTrsv.iLength  = 2;

    cStatus = PcdTransceive_CV663(PHHAL_HW_CV663_CMD_LOADPROTOCOL);

    return cStatus;
}

/**
 * @brief 配置基站操作卡类型
 *
 * @param cType
 * @return unsigned char
 */
unsigned char PcdConfig_CV663(unsigned char cType)
{
    unsigned char cStatus = ERR_FAILURE;

    if (cType == PCD_TYPE_14443A) // Config Type A
    {
        // Write_CV663(PHHAL_HW_CV663_REG_TCONTROL,0x30);
        // Write_CV663(PHHAL_HW_CV663_REG_FIFOCONTROL, 0X10);

        // LoadProtocol(bTxProtocol=0, bRxProtocol=0)
        cStatus = phhalHw_CV663_Cmd_LoadProtocol(0x00, 0x00); // 570us

        if (cStatus != 0x00) // 当前次的寻卡失败后，协议配置可能失败，在此重新配置，此配置有效，但是会返回失败。B卡相同。
            cStatus = phhalHw_CV663_Cmd_LoadProtocol(0x00, 0x00);

        // Apply RegisterSet
        // 写一个寄存器时间3.3us
        Write_CV663(PHHAL_HW_CV663_REG_T0CONTROL, 0X98); // Input clock is 13.56MHz
        Write_CV663(PHHAL_HW_CV663_REG_T1CONTROL, 0X92); // Input clock is Timer0
        // Write_CV663(PHHAL_HW_CV663_REG_T2CONTROL, 0X20);
        // Write_CV663(PHHAL_HW_CV663_REG_T2RELOADHI, 0X03);
        // Write_CV663(PHHAL_HW_CV663_REG_T2RELOADLO, 0XFF);

        Write_CV663(PHHAL_HW_CV663_REG_FIFOCONTROL, 0x10);         // confirmed
        Write_CV663(PHHAL_HW_CV663_REG_WATERLEVEL, _WATER_LEVEL_); // confirmed
        Write_CV663(PHHAL_HW_CV663_REG_RXBITCTRL, 0x88);           // confirmed
        Write_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0X8D);
        Write_CV663(PHHAL_HW_CV663_REG_TXAMP, 0X80); //
        Write_CV663(PHHAL_HW_CV663_REG_DRVCON, 0X01);
        Write_CV663(PHHAL_HW_CV663_REG_TXI, 0x05);
        Write_CV663(PHHAL_HW_CV663_REG_RXSOFD, 0X00);
        Write_CV663(PHHAL_HW_CV663_REG_RCV, 0X12);

        Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x18);
        Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x18);
        Write_CV663(PHHAL_HW_CV663_REG_TXDATANUM, 0x08);
        Write_CV663(PHHAL_HW_CV663_REG_TXMODWIDTH, 0x27);
        Write_CV663(PHHAL_HW_CV663_REG_TXSYM10BURSTLEN, 0x00);
        Write_CV663(PHHAL_HW_CV663_REG_TXWAITCTRL, 0xc0);
        Write_CV663(PHHAL_HW_CV663_REG_TXWAITLO, 0x12);
        Write_CV663(PHHAL_HW_CV663_REG_FRAMECON, 0xCF);
        Write_CV663(PHHAL_HW_CV663_REG_RXCTRL, 0x04);
        Write_CV663(PHHAL_HW_CV663_REG_RXTHRESHOLD, 0x7F); // tested value: 0x7F; recommanded value: 0x32
        Write_CV663(PHHAL_HW_CV663_REG_RXANA, 0x0F);       // tested value: 0x0F; recommanded value: 0x0A
        Write_CV663(PHHAL_HW_CV663_REG_RXWAIT, 0x90);

        // Write_CV663(PHHAL_HW_CV663_REG_SIGOUT, 0x19);

        // Write_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0x82);
        // Write_CV663(PHHAL_HW_CV663_REG_STATUS, 0x00);
        // Write_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0x8A); // FieldOn
    } else if (cType == PCD_TYPE_14443B) // Config Type B
    {
        // LoadProtocol( bTxProtocol=04, bRxProtocol=04)
        cStatus = phhalHw_CV663_Cmd_LoadProtocol(0x04, 0x04);

        if (cStatus != 0x00)
            cStatus = phhalHw_CV663_Cmd_LoadProtocol(0x04, 0x04);

        // Apply RegisterSet
        Write_CV663(PHHAL_HW_CV663_REG_T0CONTROL, 0X98); // Input clock is 13.56MHz
        Write_CV663(PHHAL_HW_CV663_REG_T1CONTROL, 0X92); // Input clock is Timer0
        // Write_CV663(PHHAL_HW_CV663_REG_T2CONTROL, 0X20);
        // Write_CV663(PHHAL_HW_CV663_REG_T2RELOADHI, 0X03);
        // Write_CV663(PHHAL_HW_CV663_REG_T2RELOADLO, 0XFF);

        Write_CV663(PHHAL_HW_CV663_REG_FIFOCONTROL, 0X10);
        Write_CV663(PHHAL_HW_CV663_REG_WATERLEVEL, _WATER_LEVEL_);
        Write_CV663(PHHAL_HW_CV663_REG_RXBITCTRL, 0X80);
        Write_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0X8F);
        Write_CV663(PHHAL_HW_CV663_REG_TXAMP, 0X8F); //  0xCF:	0CM 13,8  4CM 11.2
        Write_CV663(PHHAL_HW_CV663_REG_DRVCON, 0X01);
        Write_CV663(PHHAL_HW_CV663_REG_TXI, 0X05);
        Write_CV663(PHHAL_HW_CV663_REG_RXSOFD, 0X00);
        Write_CV663(PHHAL_HW_CV663_REG_RCV, 0X12);

        Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x7B);
        Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x7B);
        Write_CV663(PHHAL_HW_CV663_REG_TXDATANUM, 0x08);
        Write_CV663(PHHAL_HW_CV663_REG_TXMODWIDTH, 0x00);
        Write_CV663(PHHAL_HW_CV663_REG_TXSYM10BURSTLEN, 0x00);
        Write_CV663(PHHAL_HW_CV663_REG_TXWAITCTRL, 0x01);
        // Write_CV663(PHHAL_HW_CV663_REG_TXWAITLO, 0x0B);
        Write_CV663(PHHAL_HW_CV663_REG_FRAMECON, 0x05);
        Write_CV663(PHHAL_HW_CV663_REG_RXSOFD, 0x00);
        Write_CV663(PHHAL_HW_CV663_REG_RXCTRL, 0x34);
        Write_CV663(PHHAL_HW_CV663_REG_RXTHRESHOLD, 0x7F); // tested value: 0x7F; recommanded value: 0x3F
        Write_CV663(PHHAL_HW_CV663_REG_RXANA, 0x06);       // tested value: 0x06; recommanded value: 0x0A
        Write_CV663(PHHAL_HW_CV663_REG_RXWAIT, 0x9F);

        // Write_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0x8F); 	//FieldOn
    } else if (cType == PCD_TYPE_15693) {
        cStatus = phhalHw_CV663_Cmd_LoadProtocol(PHHAL_HW_CV663_REG_ERROR, 0x0A);
        // cStatus = phhalHw_CV663_Cmd_LoadProtocol(PHHAL_HW_CV663_REG_STATUS, 0x0B);

        Write_CV663(PHHAL_HW_CV663_REG_T0CONTROL, 0X98);  // 定时器在接收前 4 位后将立即停止。定时器用于有下溢时的 LFO 微调（在 PsoEdge 侧启动/停止）。定时器输入时钟在 13.56 MHz
        Write_CV663(PHHAL_HW_CV663_REG_T1CONTROL, 0X9A);  // 定时器在接收前 4 位后将立即停止。定时器用于有下溢时的 LFO 微调（在 PsoEdge 侧启动/停止）。定时器输入时钟为定时器 0 的下溢
        Write_CV663(PHHAL_HW_CV663_REG_T2CONTROL, 0X20);  // 关闭T2定时器
        Write_CV663(PHHAL_HW_CV663_REG_T2RELOADHI, 0X03); // 定义定时器 2 预设值的高字节03
        Write_CV663(PHHAL_HW_CV663_REG_T2RELOADLO, 0XFF); // 定义定时器 2 预设值的低字节FF

        Write_CV663(PHHAL_HW_CV663_REG_FIFOCONTROL, 0XB0);         // FIFO 大小为 255 字节。LoAlert =1 if FIFOLength <= WaterLevel。清空 FIFO 缓冲器
        Write_CV663(PHHAL_HW_CV663_REG_WATERLEVEL, _WATER_LEVEL_); // FIFO 48字节为溢出
        Write_CV663(PHHAL_HW_CV663_REG_RXBITCTRL, 0X80);           // ISO/IEC 14443 防碰撞。接收位的 LSB 存储在第 0 位，第二个接收的位存储在第 1 位位置。通信中接收到的最后数据字节整个字节都有效
        Write_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0X8F);             // 在 TX2 引脚反转发射机 2 的信号，两个传送器引脚皆被使能，推挽操作，时钟极性由 invtx 定义；设置为10 ％调制
        Write_CV663(PHHAL_HW_CV663_REG_TXAMP, 0XCF);               // TVDD -500 mV，剩余载波78%，调制指数12.4%
        Write_CV663(PHHAL_HW_CV663_REG_DRVCON, 0X01);              // 定义TxEnvelope信号为调制源
        Write_CV663(PHHAL_HW_CV663_REG_TXI, 0X05);                 // 工厂微调值，设置预期 Tx 的负载电流
        Write_CV663(PHHAL_HW_CV663_REG_RXSOFD, 0X00);
        Write_CV663(PHHAL_HW_CV663_REG_RCV, 0X12); // 单一 RXP 输入引脚模式为全差分，定义模数转换器（ADC）的操作模式为ADC 的正常接收模式，定义用于信号处理单元的输入为内部模拟模块信号（RX），碰撞具有至少 1/2 的信号强度

        Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x7B);        // 指定 CRC8 的传输预设值FF。定义计算CRC16。得到的 CRC 会被反转，且附加到数据帧。CRC 将被附加到数据流
        Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x7B);        // 指定 CRC8 的传输预设值FF。定义计算CRC16。得到的 CRC 会被反转，且附加到数据帧。CRC 将被附加到数据流
        Write_CV663(PHHAL_HW_CV663_REG_TXDATANUM, 0x08);       // 连续传输开始之间的时间为 ETU 的倍数，可以发送单一符号模式，最后一个数据字节的所有位都会被发送
        Write_CV663(PHHAL_HW_CV663_REG_TXMODWIDTH, 0x00);      // 定义在脉冲调制时发送数据的脉冲长度
        Write_CV663(PHHAL_HW_CV663_REG_TXSYM10BURSTLEN, 0x00); // 指定突发符号 0 和 1 所需的位的数量为8
        Write_CV663(PHHAL_HW_CV663_REG_FRAMECON, 0x0F);        // 定义Symbol3将被当作开始和停止符号发送
        Write_CV663(PHHAL_HW_CV663_REG_RXCTRL, 0x02);          // 预期会收到在 RxEOFSymbolReg 中定义的结束符号。定义接收信号的传输速度为26kBd
        Write_CV663(PHHAL_HW_CV663_REG_RXTHRESHOLD, 0x44);     // 定义接收的 MinLeve为4，定义相移检测器单元的 MinLevel 为4
        Write_CV663(PHHAL_HW_CV663_REG_RXANA, 0x06);           // fl=157kHz，fu=2,9MHz，gain（dB20）=41，Bandwith=2,7MHz
        // Write_CV663(PHHAL_HW_CV663_REG_RXWAIT, 0x10);
        Write_CV663(PHHAL_HW_CV663_REG_TXWAITCTRL, 0x88); // TxWait 的时间在数据接收结束时（RX）开始。TxWait 的时间为 TxWait x 16/13.56 MHz。无停止位，无 EGT（EGT=停止位+额外保护时间EGT），仅使用于 ISO/IEC 14443 Type B
        Write_CV663(PHHAL_HW_CV663_REG_TXWAITLO, 0xA9);   // 定义接收和发送之间或 2 个发送数据流之间的最短时间。
        Write_CV663(PHHAL_HW_CV663_REG_RXSOFD, 0x00);
        Write_CV663(PHHAL_HW_CV663_REG_RCV, 0x12); // 单一 RXP 输入引脚模式为全差分，定义模数转换器（ADC）的操作模式为ADC 的正常接收模式，定义用于信号处理单元的输入为内部模拟模块信号（RX），碰撞具有至少 1/2 的信号强度
    } else
        return 0xFF;

    Delay_ms(3);

    return cStatus;
}

/**
 * @brief CV663执行命令
 *
 * @param cCommand 命令
 * @return unsigned char 命令执行状态
 */
unsigned char PcdTransceive_CV663(unsigned char cCommand)
{
    unsigned char recebyte = 0;
    unsigned char cStatus  = 0x01;
    unsigned char irq0En   = 0x00;
    unsigned char irq1En   = 0x00;
    unsigned char cIrq0waitFor;
    unsigned char cIrq1waitFor;
    unsigned char n1, n2, j;
    unsigned short i;
    unsigned char cError;

    n2           = 0x00;
    cIrq0waitFor = 0x00;
    cIrq1waitFor = 0x00;

    switch (cCommand) {
        case PHHAL_HW_CV663_CMD_WRITEE2:
            irq0En       = 0x10;
            irq1En       = 0x40;
            cIrq0waitFor = 0x00;
            cIrq1waitFor = 0x40;
            break;
        case PHHAL_HW_CV663_CMD_READE2:
            irq0En       = 0x10;
            irq1En       = 0x40;
            cIrq0waitFor = 0x00;
            cIrq1waitFor = 0x40;
            recebyte     = 1;
            break;
        case PHHAL_HW_CV663_CMD_LOADPROTOCOL:
            irq0En       = 0x10;
            irq1En       = 0x42;
            cIrq0waitFor = 0x10;
            cIrq1waitFor = 0x02;
            break;
        case PHHAL_HW_CV663_CMD_LOADKEY:
            irq0En       = 0x18;
            irq1En       = 0x42;
            cIrq0waitFor = 0x00;
            cIrq1waitFor = 0x40;
            break;
        case PHHAL_HW_CV663_CMD_MFAUTHENT:
            irq0En       = 0x18; // 0x14;
            irq1En       = 0x42;
            cIrq0waitFor = 0x00;
            cIrq1waitFor = 0x40;
            break;
        case PHHAL_HW_CV663_CMD_TRANSMIT:
            irq0En       = 0x18; // 0x0A;
            irq1En       = 0x42;
            cIrq0waitFor = 0x08;
            cIrq1waitFor = 0x42;
            break;
        case PHHAL_HW_CV663_CMD_TRANSCEIVE:
            irq0En       = 0x14;
            irq1En       = 0x42;
            cIrq0waitFor = 0x10;
            cIrq1waitFor = 0x02;
            recebyte     = 1;
            break;
        default:
            cCommand = MI_UNKNOWN_COMMAND;
            break;
    }
    if (cCommand != MI_UNKNOWN_COMMAND) {
        Write_CV663(PHHAL_HW_CV663_REG_COMMAND, PHHAL_HW_CV663_CMD_IDLE);
        Write_CV663(PHHAL_HW_CV663_REG_IRQ0, 0x7F);   // clear all IRQ0 flags
        Write_CV663(PHHAL_HW_CV663_REG_IRQ1, 0x7F);   // clear all IRQ1 flags
        Write_CV663(PHHAL_HW_CV663_REG_IRQ0EN, 0x00); // clear all IRQ0 source
        Write_CV663(PHHAL_HW_CV663_REG_IRQ1EN, 0x00); // clear all IRQ1 source
        Write_CV663(PHHAL_HW_CV663_REG_IRQ0EN, irq0En);
        Write_CV663(PHHAL_HW_CV663_REG_IRQ1EN, irq1En);
        SetBitMask_CV663(PHHAL_HW_CV663_REG_FIFOCONTROL, 0x10); // FlushFIFO();

        for (i = 0; i < g_sPcdTrsv.iLength; i++)
            Write_CV663(PHHAL_HW_CV663_REG_FIFODATA, g_sPcdTrsv.cData[i]);

        Write_CV663(PHHAL_HW_CV663_REG_COMMAND, cCommand);

        i = 50000;

        do {
            n1 = Read_CV663(PHHAL_HW_CV663_REG_IRQ0);
            n2 = Read_CV663(PHHAL_HW_CV663_REG_IRQ1);
            i--;
        } while ((i != 0) && !(n1 & cIrq0waitFor) && !(n2 & cIrq1waitFor));

        if ((i != 0) && !(n2 & 0x02)) {
            cError = Read_CV663(PHHAL_HW_CV663_REG_ERROR);

            if (0 == cError)
                cStatus = MI_OK;
            else if (cError & 0x02)
                cStatus = MI_FRAMINGERR;
            else if (cError & 0x04)
                cStatus = MI_COLLERR;
            else if (cError & 0x01)
                cStatus = MI_CRCERR;
            else
                cStatus = MI_FATAL_ERR;

            if (recebyte) {
                n1 = Read_CV663(PHHAL_HW_CV663_REG_FIFOLENGTH);
                n2 = Read_CV663(PHHAL_HW_CV663_REG_RXBITCTRL) & 0x07;

                if (n2 != 0)
                    g_sPcdTrsv.iLength = (n1 - 1) * 8 + n2;
                else
                    g_sPcdTrsv.iLength = n1;

                for (j = 0; j < n1; j++)
                    g_sPcdTrsv.cData[j] = Read_CV663(PHHAL_HW_CV663_REG_FIFODATA);
            }
        } else if (n2 & 0x02)
            cStatus = MI_NOTAGERR;
        else
            cStatus = MI_COM_ERR;
    }

    Write_CV663(PHHAL_HW_CV663_REG_COMMAND, PHHAL_HW_CV663_CMD_IDLE);
    Write_CV663(PHHAL_HW_CV663_REG_IRQ0, 0x7F);           // clear all IRQ0 flags
    Write_CV663(PHHAL_HW_CV663_REG_IRQ1, 0x7F);           // clear all IRQ1 flags
    Write_CV663(PHHAL_HW_CV663_REG_IRQ0EN, 0x00);         // clear all IRQ0 source
    Write_CV663(PHHAL_HW_CV663_REG_IRQ1EN, 0x00);         // clear all IRQ1 source
    Write_CV663(PHHAL_HW_CV663_REG_TCONTROL, 0x33);       // stop timer now
    SetBitMask_CV663(PHHAL_HW_CV663_REG_T0CONTROL, 0x80); // set timer auto stop
    SetBitMask_CV663(PHHAL_HW_CV663_REG_T1CONTROL, 0x80); // set timer auto stop

    Delay_us(500); // FDT PCD,MIN

    return cStatus;
}

/**
 * @brief
 *
 * @param iTime
 */
void M500PcdRfReset_CV663(unsigned short iTime)
{
    if (iTime != 0) {
        ClearBitMask_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0x08);

        Delay_ms(iTime); // Delay for 1 ms

        SetBitMask_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0x08);
    } else
        ClearBitMask_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0x08);

    return;
}

/**
 * @brief
 *
 * @param cFSDI
 */
void PcdSetFSD_CV663(unsigned char cFSDI)
{
    switch (cFSDI) {
        case 0: // 14
        case 1: // 22
        case 2: // 30
        case 3: // 38
        case 4: // 46
            g_sPcdPara.cFSD = 14 + cFSDI * 8;
            break;
        case 5:
            g_sPcdPara.cFSD = 62;
            break;
        case 6:
            g_sPcdPara.cFSD = 94;
            break;
        case 7:
            g_sPcdPara.cFSD = 126;
            break;
        default:
            g_sPcdPara.cFSD = 254;
            break;
    }

    return;
}

/**
 * @brief
 *
 * @param cSta
 */
void PcdAntenna_CV663(unsigned char cSta)
{
    unsigned char cTemp = Read_CV663(PHHAL_HW_CV663_REG_DRVMODE);

    if (cSta)
        Write_CV663(PHHAL_HW_CV663_REG_DRVMODE, cTemp | 0x08);
    else
        Write_CV663(PHHAL_HW_CV663_REG_DRVMODE, cTemp & (~0x08));
}

uint8_t UUID[8];
uint8_t TagSNR[512];

void CV663_Read_Card(void)
{
    uint8_t cStatus = ISO15693_MultiTagAutoInventory(0x00, 0x00, 0x00);

    if (g_sPcdTrsv.cData[2] != 0xF0 && g_sPcdTrsv.cData[2] != 0x00) {

        printf("one 15693 card\r\n");

        for (uint8_t x = 0; x < 8; x++)
            UUID[x] = g_sPcdTrsv.cData[x + 2];

        memset(g_sPcdTrsv.cData, 0, sizeof(g_sPcdTrsv.cData));
    } else if (g_sPcdTrsv.cData[2] == 0xF0) {
        printf("more 15693 card\r\n");
        for (uint8_t i = 0; i < 0x0F; i++) {

            if (!i) {
                g_sPcdTrsv.cData[0] = 0x06;               // Flag
                g_sPcdTrsv.cData[1] = ISO15693_INVENTORY; // Command
                g_sPcdTrsv.cData[2] = 0x00;               // Mask长度

                g_sPcdTrsv.cData[3] = 0x00; // Mask值
                g_sPcdTrsv.cData[4] = 0x00; // Mask值
                g_sPcdTrsv.cData[5] = 0x00; // Mask值
                g_sPcdTrsv.cData[6] = 0x00; // Mask值

                g_sPcdTrsv.iLength = 4;

                SetTimeOut();

                PcdTransceive_CV663(PCD_TRANSCEIVE);
            } else {
                g_sPcdTrsv.cData[0] = 0xFF;

                g_sPcdTrsv.iLength = 0;

                PcdTransceive_CV663(PCD_TRANSCEIVE);
            }

            Delay_us(1);

            if (!g_sPcdTrsv.cData[0]) {
                for (uint16_t x = 0; x < 512; x++)
                    if (g_sPcdTrsv.cData[x])
                        printf("g_sPcdTrsv.cData[%d] = 0x%02X;\r\n", x, g_sPcdTrsv.cData[x]);

                for (uint8_t i = 0; i < 8; i++)
                    UUID[i] = g_sPcdTrsv.cData[i + 2];

                printf("\r\n");
            }

            Delay_ms(50);
        }
    }

    if (UUID[7] == 0xE0) {
        printf("UID: ");
        for (uint8_t x = 0; x < 8; x++)
            printf("0x%02X ", g_sPcdTrsv.cData[x + 2]);

        printf("\r\n\r\n");

        printf("Read one 15693 Card # # # # # # #\r\n");

        unsigned char Length    = 0;
        unsigned char Data[256] = 0;

        for (uint8_t kk = 0; kk < 5; kk++) {
            ISO15693_Select(0x02, UUID);
            ISO15693_ReadBlock(0x02, UUID, kk * 16, 32, &Length, Data);
            for (uint8_t i = 0; i < 16; i++) {
                printf("Data %X:", kk * 16 + i);
                for (uint8_t x = 0; x < 4; x++)
                    printf("0x%02X ", g_sPcdTrsv.cData[i * 4 + x + 1]);
            }
        }

        printf("\r\n\r\n");

        Delay_s(1);

        printf("Write one 15693 Card # # # # # # #\r\n\r\n");

        unsigned char writedata1[4] = {0xFF, 0xFF, 0xFF, 0xFF};

        ISO15693_Write(0x02, UUID, 1, writedata1);

        printf("Read one 15693 Card # # # # # # #\r\n");

        for (uint8_t kk = 0; kk < 5; kk++) {
            ISO15693_Select(0x02, UUID);
            ISO15693_ReadBlock(0x02, UUID, kk * 16, 32, &Length, Data);
            for (uint8_t i = 0; i < 16; i++) {
                printf("Data %X:", kk * 16 + i);
                for (uint8_t x = 0; x < 4; x++)
                    printf("0x%02X ", g_sPcdTrsv.cData[i * 4 + x + 1]);
            }
        }

        printf("\r\n\r\n");

        Delay_s(1);

        printf("Clearn one 15693 Card # # # # # # #\r\n\r\n");

        unsigned char writedata2[4] = 0x00;
        for (uint8_t i = 0; i < 64; i++)
            ISO15693_Write(0x02, UUID, i, writedata2);

        printf("Read one 15693 Card # # # # # # #\r\n");

        for (uint8_t kk = 0; kk < 5; kk++) {
            ISO15693_Select(0x02, UUID);
            ISO15693_ReadBlock(0x02, UUID, kk * 16, 32, &Length, Data);
            for (uint8_t i = 0; i < 16; i++) {
                printf("Data %X:", kk * 16 + i);
                for (uint8_t x = 0; x < 4; x++)
                    printf("0x%02X ", g_sPcdTrsv.cData[i * 4 + x + 1]);
            }
        }

        memset(UUID, 0, sizeof(UUID));
        printf("\r\n\r\n");

        Delay_s(3);
    }

    if (cStatus == ERR_SUCCESS) {
        LED1(1);
    } else {
        Delay_ms(10);

        LED1(0); // 蓝
        LED2(0); // 绿
        LED3(0); // 橙
        LED4(0); // 红
    }
}