/**
 * @file CV663.c
 * @author Yikui Zhang (xingdala@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-10-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "main.h"

tpd_pcdpara g_sPcdPara;
tpd_pcdtrsv g_sPcdTrsv;

#define IIC_SDA_MODE_OUTPUT 1
#define IIC_SDA_MODE_INPUT 0

/**
 * @brief 读卡参数初始化
 *
 */
tpd_Card g_sCard; // Define Card Structure

unsigned char g_cBuffer[64][16]; // Card data buffer
uint16_t g_iLength;              // Card data buffer length

unsigned char g_cATS[32]; // Card serial number buffer
unsigned char g_cATSLen;  // Card serial number buffer length

unsigned char g_cM1KEY[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Card key 1
// unsigned char g_cM2KEY[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Card key 2

/**
 * @brief CV663模拟IIC初始化
 *
 */
void CV663_IIC_Init(void)
{
    gpio_mux_ctl(CV663_ADR1_Pin, 0);
    gpio_direction_output(CV663_ADR1_Pin); // IIC_ADR1

    gpio_mux_ctl(CV663_ADR2_Pin, 0);
    gpio_direction_output(CV663_ADR2_Pin); // IIC_ADR2

    gpio_output_low(CV663_ADR1_Pin);
    gpio_output_low(CV663_ADR2_Pin);

    gpio_mux_ctl(CV663_CLK_Pin, 0);
    gpio_direction_output(CV663_CLK_Pin); // IIC_CLK

    gpio_mux_ctl(CV663_SDA_Pin, 0);
    gpio_direction_output(CV663_SDA_Pin); // IIC_SDA

    CV663_SDA_Hight;
    CV663_CLK_Low;

    gpio_mux_ctl(CV663_RST_Pin, 0);
    gpio_direction_output(CV663_RST_Pin); // SPI_RST

    gpio_mux_ctl(CV663_IRQ_Pin, 0);
    gpio_direction_input(CV663_IRQ_Pin, 0); // SPI_IRQ

    gpio_mux_ctl(CV663_SL0_Pin, 0);
    gpio_direction_output(CV663_SL0_Pin); // SL0

    gpio_mux_ctl(CV663_SL1_Pin, 0);
    gpio_direction_output(CV663_SL1_Pin); // SL1

    gpio_output_high(CV663_SL0_Pin);
    gpio_output_low(CV663_SL1_Pin);
}

/**
 * @brief CV663 communication initialization
 *
 */
void CV663_Init(void)
{
    CV663_IIC_Init(); // CV663_SPI initialization

    unsigned char Status; // Task process state storage

    Status = PcdReset_CV663(); // Restart the antenna

    if (Status == ERR_SUCCESS)            // Determine whether the antenna reset is successful
        PcdConfig_CV663(PCD_TYPE_14443A); // The antenna is initialized successfully, and CV663 initial card reading parameters are configured

    printf("CV663 Init OK!\r\n");
}

/**
 * @brief IIC启动(OK)
 *
 */
void IIC_start(void)
{
    CV663_SDA_Hight;
    CV663_CLK_Hight;
    CV663_SDA_Low;
    CV663_CLK_Low;
}

/**
 * @brief IIC停止(OK)
 *
 */
void IIC_stop(void)
{
    CV663_SDA_Low;
    CV663_CLK_Hight;
    CV663_SDA_Hight;
    CV663_CLK_Low;
}

/**
 * @brief IIC主机不应答
 *
 */
void IIC_no_send_ack(void)
{
    CV663_CLK_Hight;
    CV663_CLK_Low;
}

/**
 * @brief 忽视IIC从机应答
 *
 */
void IIC_no_read_ack(void)
{
    CV663_CLK_Hight;
    CV663_CLK_Low;
}

/**
 * @brief IIC写入一个字节
 *
 * @param byte 字节
 */
void IIC_write_one_byte(unsigned char byte)
{
    for (unsigned char i = 0; i < 8; i++)
    {
        CV663_CLK_Low;
        if (byte & 0x80)
            CV663_SDA_Hight;
        else
            CV663_SDA_Low;
        CV663_CLK_Hight;
        byte <<= 1;
    }
    CV663_CLK_Low;
    CV663_SDA_Hight;
}

/**
 * @brief IIC读取一个字节
 *
 * @return unsigned char 返回一个字节
 */
unsigned char IIC_read_one_byte(void)
{
    unsigned char cValue = 0x00;

    gpio_mux_ctl(CV663_SDA_Pin, 0);
    gpio_direction_input(CV663_SDA_Pin, 0);

    for (unsigned char i = 0; i < 8; i++)
    {
        cValue <<= 1;

        CV663_CLK_Low;
        CV663_CLK_Hight;

        if (CV663_SDA_Read)
            cValue |= 0x01;
    }
    CV663_CLK_Low;

    gpio_mux_ctl(CV663_SDA_Pin, 0);
    gpio_direction_output(CV663_SDA_Pin);

    CV663_SDA_Hight;

    return cValue;
}

/**
 * @brief CV663写入一个字节
 *
 * @param Address 地址字节
 * @param Data 数据字节
 */
void Write_CV663(unsigned char Address, unsigned char Data)
{
    Delay_ms(2);

    IIC_start();

    IIC_write_one_byte(0x50); // slave_address_Write

    IIC_no_read_ack();

    IIC_write_one_byte(Address);

    IIC_no_read_ack();

    IIC_write_one_byte(Data);

    IIC_no_read_ack();

    IIC_stop();
}

/**
 * @brief CV663读取一个字节
 *
 * @param Address 地址字节
 * @return unsigned char 返回字节
 */
unsigned char Read_CV663(unsigned char Address)
{
    Delay_ms(2);

    unsigned char cValue = 0x00;

    IIC_start();

    IIC_write_one_byte(0x50); // slave_address_Write

    IIC_no_read_ack();

    IIC_write_one_byte(Address);

    IIC_no_read_ack();

    IIC_stop();

    IIC_start();

    IIC_write_one_byte(0x51); // slave_address_Read

    IIC_no_read_ack();

    cValue = IIC_read_one_byte();

    IIC_no_send_ack();

    IIC_stop();

    return cValue;
}

/**
 * @brief 复位并初始化CV663
 *
 * @return unsigned char Status 状态位
 */
unsigned char PcdReset_CV663(void)
{
    unsigned char Status = MI_OK;
    unsigned char n;
    unsigned int Delay = 0xFFFF;

    CV663_RST_Hight;

    Delay_ms(10);

    CV663_RST_Low;

    Delay_ms(10);

    do
    {
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

    cClock1 = (1 << cFWI) + 12 + cTpcd;

    Write_CV663(PHHAL_HW_CV663_REG_T0RELOADHI, (unsigned char)(cClock0 >> 8));
    Write_CV663(PHHAL_HW_CV663_REG_T0RELOADLO, (unsigned char)cClock0);
    Write_CV663(PHHAL_HW_CV663_REG_T1RELOADHI, (unsigned char)(cClock1 >> 8));
    Write_CV663(PHHAL_HW_CV663_REG_T1RELOADLO, (unsigned char)cClock1);
    //	Write_CV663(PHHAL_HW_CV663_REG_T0CONTROL, 0x98);
    //	Write_CV663(PHHAL_HW_CV663_REG_T1CONTROL, 0x9A);
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
    g_sPcdTrsv.iLength = 2;

    cStatus = PcdTransceive_CV663(PHHAL_HW_CV663_CMD_LOADPROTOCOL);

    return cStatus;
}

/**
 * @brief
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

        if (cStatus != 0x00)
            cStatus = phhalHw_CV663_Cmd_LoadProtocol(0x00, 0x00);

        // Apply RegisterSet
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
    }
    else if (cType == PCD_TYPE_14443B) // Config Type B
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
    }
    else
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
    unsigned char cStatus = 0x01;
    unsigned char irq0En = 0x00;
    unsigned char irq1En = 0x00;
    unsigned char cIrq0waitFor;
    unsigned char cIrq1waitFor;
    unsigned char n1, n2, j;
    unsigned short i;
    unsigned char cError;

    n2 = 0x00;
    cIrq0waitFor = 0x00;
    cIrq1waitFor = 0x00;

    switch (cCommand)
    {
    case PHHAL_HW_CV663_CMD_WRITEE2:
        irq0En = 0x10;
        irq1En = 0x40;
        cIrq0waitFor = 0x00;
        cIrq1waitFor = 0x40;
        break;
    case PHHAL_HW_CV663_CMD_READE2:
        irq0En = 0x10;
        irq1En = 0x40;
        cIrq0waitFor = 0x00;
        cIrq1waitFor = 0x40;
        recebyte = 1;
        break;
    case PHHAL_HW_CV663_CMD_LOADPROTOCOL:
        irq0En = 0x10;
        irq1En = 0x42;
        cIrq0waitFor = 0x10;
        cIrq1waitFor = 0x02;
        break;
    case PHHAL_HW_CV663_CMD_LOADKEY:
        irq0En = 0x18;
        irq1En = 0x42;
        cIrq0waitFor = 0x00;
        cIrq1waitFor = 0x40;
        break;
    case PHHAL_HW_CV663_CMD_MFAUTHENT:
        irq0En = 0x18; // 0x14;
        irq1En = 0x42;
        cIrq0waitFor = 0x00;
        cIrq1waitFor = 0x40;
        break;
    case PHHAL_HW_CV663_CMD_TRANSMIT:
        irq0En = 0x18; // 0x0A;
        irq1En = 0x42;
        cIrq0waitFor = 0x08;
        cIrq1waitFor = 0x42;
        break;
    case PHHAL_HW_CV663_CMD_TRANSCEIVE:
        irq0En = 0x14;
        irq1En = 0x42;
        cIrq0waitFor = 0x10;
        cIrq1waitFor = 0x02;
        recebyte = 1;
        break;
    default:
        cCommand = MI_UNKNOWN_COMMAND;
        break;
    }
    if (cCommand != MI_UNKNOWN_COMMAND)
    {
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

        do
        {
            n1 = Read_CV663(PHHAL_HW_CV663_REG_IRQ0);
            n2 = Read_CV663(PHHAL_HW_CV663_REG_IRQ1);
            i--;
        } while ((i != 0) && !(n1 & cIrq0waitFor) && !(n2 & cIrq1waitFor));

        if ((i != 0) && !(n2 & 0x02))
        {
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

            if (recebyte)
            {
                n1 = Read_CV663(PHHAL_HW_CV663_REG_FIFOLENGTH);
                n2 = Read_CV663(PHHAL_HW_CV663_REG_RXBITCTRL) & 0x07;

                if (n2 != 0)
                    g_sPcdTrsv.iLength = (n1 - 1) * 8 + n2;
                else
                    g_sPcdTrsv.iLength = n1;

                for (j = 0; j < n1; j++)
                    g_sPcdTrsv.cData[j] = Read_CV663(PHHAL_HW_CV663_REG_FIFODATA);
            }
        }
        else if (n2 & 0x02)
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
    if (iTime != 0)
    {
        ClearBitMask_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0x08);

        Delay_ms(iTime); // Delay for 1 ms

        SetBitMask_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0x08);
    }
    else
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
    switch (cFSDI)
    {
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

void CV663_Read_Card(void)
{
    uint8_t cStatus;

    PiccReset();

    cStatus = PiccRequest(PICC_REQALL, g_sCard.cATQ);

    if (cStatus != ERR_SUCCESS)
        cStatus = PiccRequest(PICC_REQALL, g_sCard.cATQ);

    if (cStatus == ERR_SUCCESS)
    {
        printf("Find Card!\r\n");

        LED1(1);

        Buzzer_Beep(Buzzer_True);

        cStatus = PiccAnticollSelect(&g_sCard.cLen, g_sCard.cUID, &g_sCard.cSAK);

        cStatus = PiccHaltA();

        cStatus = PiccRequest(PICC_REQIDL, g_sCard.cATQ);

        cStatus = PiccRequest(PICC_REQALL, g_sCard.cATQ);

        if (cStatus == ERR_SUCCESS)
            cStatus = PiccAnticollSelect(&g_sCard.cLen, g_sCard.cUID, &g_sCard.cSAK);
    }
    else
        Delay_ms(10);

    if (cStatus == ERR_SUCCESS)
    {
        uint8_t tu_cBuffer[256];

        printf("Mifare Card!\r\n");

        for (uint8_t x = 0; x < 16; x++)
        {
            cStatus = PiccAuthState(x, PICC_AUTHENT1A, g_cM1KEY);
            for (uint8_t y = 0; y < 4; y++)
                PiccRead(x, y, g_cBuffer[x * 4 + y]);
        }
        printf("Read OK!\r\n");

        for (uint8_t x = 0; x < 16; x++)
        {
            printf("data%d\r\n", x);
            for (uint8_t y = 0; y < 4; y++)
            {
                for (uint8_t i = 0; i < 16; i++)
                    printf("%02X  ", g_cBuffer[x * 4 + y][i]);
                printf("\r\n");
            }
        }

        Delay_s(2);
    }
    else
    {
        gpio_output_high(22); // 蓝
        gpio_output_high(23); // 绿
        gpio_output_high(24); // 橙
        gpio_output_high(25); // 红
    }
}