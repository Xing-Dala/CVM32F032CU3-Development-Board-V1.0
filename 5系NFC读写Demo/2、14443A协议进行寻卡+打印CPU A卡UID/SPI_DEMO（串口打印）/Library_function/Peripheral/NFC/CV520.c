/**
 * @file CV520.c
 * @author Yikui Zhang (xingdala@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-10-10
 *
 * @copyright Copyright (c) 2022
 *
 */

/**
 * @brief Analog SIP or High speed SPI
 *
 */
#define Analog_SIP

#include "main.h"

tpd_pcdpara g_sPcdPara;
tpd_pcdtrsv g_sPcdTrsv;

/**
 * @brief 读卡参数初始化
 *
 */
tpd_Card g_sCard; // Define Card Structure

uint8_t g_cBuffer[64][16]; // Card data buffer
uint16_t g_iLength;        // Card data buffer length

uint8_t g_cATS[32]; // Card serial number buffer
uint8_t g_cATSLen;  // Card serial number buffer length

uint8_t g_cM1KEY[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Card key 1
// uint8_t g_cM2KEY[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Card key 2

/**
 * @brief CV520_SPI initialization
 *
 */
void CV520_SPI_Init(void)
{
    gpio_mux_ctl(CV520_CLK_Pin, 0);
    gpio_fun_sel(CV520_CLK_Pin, GPIO_Dx);
    gpio_fun_inter(CV520_CLK_Pin, 0);
    gpio_direction_output(CV520_CLK_Pin); // SPI_CLK

    gpio_mux_ctl(CV520_CS_Pin, 0);
    gpio_fun_sel(CV520_CS_Pin, GPIO_Dx);
    gpio_fun_inter(CV520_CS_Pin, 0);
    gpio_direction_output(CV520_CS_Pin); // SPI_CS

    gpio_mux_ctl(CV520_MOSI_Pin, 0);
    gpio_fun_sel(CV520_MOSI_Pin, GPIO_Dx);
    gpio_fun_inter(CV520_MOSI_Pin, 0);
    gpio_direction_output(CV520_MOSI_Pin); // SPI_MOSI

    gpio_mux_ctl(CV520_MISO_Pin, 0);
    gpio_fun_sel(CV520_MISO_Pin, GPIO_Dx);
    gpio_fun_inter(CV520_MISO_Pin, 0);
    gpio_direction_input(CV520_MISO_Pin, 0); // SPI_MISO

    gpio_mux_ctl(CV520_RST_Pin, 0);
    gpio_fun_sel(CV520_RST_Pin, GPIO_Dx);
    gpio_fun_inter(CV520_RST_Pin, 0);
    gpio_direction_output(CV520_RST_Pin); // SPI_RST

    gpio_mux_ctl(CV520_IRQ_Pin, 0);
    gpio_fun_sel(CV520_IRQ_Pin, GPIO_Dx);
    gpio_fun_inter(CV520_IRQ_Pin, 0);
    gpio_direction_input(CV520_IRQ_Pin, 0); // SPI_IRQ
}

/**
 * @brief CV520 communication initialization
 *
 */
void CV520_Init(void)
{
    CV520_SPI_Init(); // CV520_SPI initialization

    uint8_t Status; // Task process state storage

    Status = PcdReset_CV520(); // Restart the antenna

    if (Status == ERR_SUCCESS)      // Determine whether the antenna reset is successful
        PcdConfig(PCD_TYPE_14443A); // The antenna is initialized successfully, and CV520 initial card reading parameters are configured

    printf("CV520 Init OK!\r\n");
}

/**
 * @brief Read the data cache at the address specified by CV520
 *
 * @param Address Address
 * @return uint8_t Data
 */
uint8_t Read_CV520(uint8_t Address)
{
    uint8_t cValue; // Task process state storage

    CV520_MOSI_Hight;

    Address <<= 1;
    Address |= 0x80;

    CV520_CLK_Low;
    CV520_CS_Low;

    for (uint8_t cCnt = 0; cCnt < 8; cCnt++) {
        if (Address & 0x80)
            CV520_MOSI_Hight;
        else
            CV520_MOSI_Low;

        CV520_CLK_Hight;

        Address <<= 1;

        CV520_CLK_Low;
    }

    CV520_MOSI_Low;

    for (uint8_t cCnt = 0; cCnt < 8; cCnt++) {
        cValue <<= 1;

        CV520_CLK_Hight;

        if (CV520_MISO_Read)
            cValue |= 0x01;

        CV520_CLK_Low;
    }

    CV520_CS_Hight;

    return cValue;
}

/**
 * @brief Write the data cache at the address specified by CV520
 *
 * @param Address Addrss
 * @param Data Data
 */
void Write_CV520(uint8_t Address, uint8_t Data)
{
    CV520_MISO_Low;

    Address <<= 1;
    Address &= 0x7E;

    CV520_CLK_Low;
    CV520_CS_Low;

    for (uint8_t cCnt = 0; cCnt < 8; cCnt++) {
        if (Address & 0x80)
            CV520_MOSI_Hight;
        else
            CV520_MOSI_Low;

        CV520_CLK_Hight;

        Address <<= 1;

        CV520_CLK_Low;
    }

    for (uint8_t cCnt = 0; cCnt < 8; cCnt++) {
        if (Data & 0x80)
            CV520_MOSI_Hight;
        else
            CV520_MOSI_Low;

        CV520_CLK_Hight;

        Data <<= 1;

        CV520_CLK_Low;
    }

    CV520_MOSI_Low;
    CV520_CS_Hight;

    return;
}

/**
 * @brief Restart the antenna
 *
 * @return uint8_t Status
 */
uint8_t PcdReset_CV520(void)
{
    uint8_t Status = MI_OK; // Task process state storage
    uint8_t CV520_Ready;
    uint16_t delay = 0xFFFF;

    CV520_RST_Low;

    Delay_ms(10);

    CV520_RST_Hight;

    do {
        CV520_Ready = Read_CV520(JREG_COMMAND);
        delay--;
    } while (delay != 0 && CV520_Ready != 0x20);

    if (delay == 0) {
        Status = MI_RESETERR;

        printf("CV520 Reset Error!\r\n");
    } else {
        g_sPcdPara.cType = 0x00;

        // Write_CV520(JREG_WATERLEVEL, 0x35);
        // CV520_Ready = Read_CV520(JREG_WATERLEVEL);
        // Write_CV520(JREG_WATERLEVEL, 0x1A);
        // CV520_Ready = Read_CV520(JREG_WATERLEVEL);
    }

    return Status;
}

/**
 * @brief Set the Bit Mask object
 *
 * @param Address Address
 * @param cMask
 */
void SetBitMask_CV520(uint8_t Address, uint8_t cMask)
{
    uint8_t cTemp;

    cTemp = Read_CV520(Address);

    Write_CV520(Address, (cTemp | cMask));

    cTemp = Read_CV520(Address);

    return;
}

/**
 * @brief Clean the Bit Mask object
 *
 * @param Address Address
 * @param cMask
 */
void ClearBitMask(uint8_t Address, uint8_t cMask)
{
    uint8_t cTemp;
    cTemp = Read_CV520(Address);
    Write_CV520(Address, (cTemp & ~cMask));
    return;
}

/**
 * @brief Set card reading mode
 *
 * @param cType "PCD_TYPE_14443A" or "PCD_TYPE_14443B";
 *
 * @test CV532: "PCD_TYPE_14443A"
 *
 * @return uint8_t Status
 */
uint8_t PcdConfig(uint8_t cType)
{
    uint8_t Status = ERR_FAILURE; // Task process state storage

    if (cType == PCD_TYPE_14443A && cType != g_sPcdPara.cType) {
        Write_CV520(JREG_COMMAND, 0x00);    // Starts and stops command execution.
        Write_CV520(JREG_COMMIEN, 0x00);    // Control bits to enable and disable the passing of interrupt requests.
        Write_CV520(JREG_DIVIEN, 0x00);     // Control bits to enable and disable the passing of interrupt requests.
        Write_CV520(JREG_COMMIRQ, 0x00);    // Interrupt request bits.
        Write_CV520(JREG_DIVIRQ, 0x00);     // Interrupt request bits.
        Write_CV520(JREG_STATUS2, 0x00);    // Contains status bits of the receiver, transmitter and data mode detector.
        Write_CV520(JREG_FIFOLEVEL, 0x80);  // Indicates the number of bytes stored in the FIFO.
        Write_CV520(JREG_WATERLEVEL, 0x01); // Defines the level for FIFO under- and overflow warning.
        Write_CV520(JREG_CONTROL, 0x10);    // Miscellaneous control bits.
        Write_CV520(JREG_BITFRAMING, 0x00); // Adjustments for bit-oriented frames.
        Write_CV520(JREG_COLL, 0x00);       // Defines the first bit-collision detected on the RF interface.

        Write_CV520(JREG_MODE, 0x01);       // Defines general mode settings for transmitting and receiving.
        Write_CV520(JREG_TXMODE, 0x00);     // Defines the data rate during transmission.
        Write_CV520(JREG_RXMODE, 0x00);     // Defines the data rate during reception.
        Write_CV520(JREG_TXCONTROL, 0x83);  // Controls the logical behavior of the antenna driver pins TX1 and TX2.
        Write_CV520(JREG_TX_ASK, 0x40);     // Controls transmit modulation settings.
        Write_CV520(JREG_TXSEL, 0x10);      // Selects the internal sources for the analog module.
        Write_CV520(JREG_RXSEL, 0x83);      // Selects internal receiver settings.
        Write_CV520(JREG_RXTRESHOLD, 0xAA); // Selects thresholds for the bit decoder.
        Write_CV520(JREG_DEMOD, 0x4D);      // Defines demodulator settings.
        Write_CV520(JREG_TYPE_B, 0x00);
        Write_CV520(JREG_SERIALSPEED, 0xEB);

        Write_CV520(JREG_MODWIDTH, 0x26); // Sets the modulation width.
        Write_CV520(JREG_RFCFG, 0x5F);    // Configures the receiver gain.
        Write_CV520(JREG_GSN, 0xFF);      // Defines the conductance of the antenna driver pins TX1 and TX2 for the n-driver when the driver is switched on.
        Write_CV520(JREG_CWGSP, 0x0F);    // Defines the conductance of the p-driver output during periods of no modulation.// 02为节电模式
        Write_CV520(JREG_MODGSP, 0x0F);   // Defines the conductance of the p-driver output during modulation.// 02为节电模式

        Status = ERR_SUCCESS;
    } else if (cType == g_sPcdPara.cType)
        return ERR_SUCCESS;

    if (Status == ERR_SUCCESS) {
        g_sPcdPara.cType = cType;

        for (uint16_t delay = 0; delay < 3000; delay++)
            ;
    } else
        g_sPcdPara.cType = 0x00;

    return ERR_SUCCESS;
}

/**
 * @brief Joiner command implement
 *
 * @param Command instructions
 * @return uint8_t Status
 */
uint8_t PcdTransceive(uint8_t Command)
{
    uint8_t Status; // Task process state storage
    uint8_t recebyte = 0;
    uint8_t waitFor  = 0x00;
    uint8_t n;
    uint32_t i = 0x0040;
    uint32_t j;

    switch (Command) {
        case JCMD_IDLE:
            waitFor = 0x00;
            break;
        case JCMD_AUTHENT:
            waitFor = 0x11;
            break;
        case JCMD_RECEIVE:
            waitFor  = 0x10;
            recebyte = 1;
            break;
        case JCMD_TRANSMIT:
            waitFor = 0x41;
            break;
        case JCMD_TRANSCEIVE:
            waitFor  = 0x21;
            recebyte = 1;
            i        = 0xFFFF;
            break;
        default:
            Status = MI_UNKNOWN_COMMAND;
            printf("PcdTransceive: unknown command!\r\n");
            break;
    }

    if (Status != MI_UNKNOWN_COMMAND) {
        Write_CV520(JREG_COMMAND, JCMD_IDLE);
        Write_CV520(JREG_COMMIEN, 0xFF);
        ClearBitMask(JREG_COMMIRQ, 0x80);
        SetBitMask_CV520(JREG_FIFOLEVEL, 0x80); // flush FIFO

        if (g_sPcdTrsv.iLength > _WATER_LEVEL_) // Write data to FIFO, and Send data
            n = _WATER_LEVEL_;
        else
            n = g_sPcdTrsv.iLength;

        for (i = 0; i < n; i++)
            Write_CV520(JREG_FIFODATA, g_sPcdTrsv.cData[i]);

        Write_CV520(JREG_COMMAND, Command);
        SetBitMask_CV520(JREG_BITFRAMING, 0x80);

        while (i < g_sPcdTrsv.iLength) {
            n = Read_CV520(JREG_FIFOLEVEL);

            if (n < _WATER_LEVEL_)
                Write_CV520(JREG_FIFODATA, g_sPcdTrsv.cData[i++]);
        }

        // Check PCD status
        j = 0xF000;
        i = 0x0000;

        do {
            n = Read_CV520(JREG_FIFOLEVEL);

            if (n > _WATER_LEVEL_)
                g_sPcdTrsv.cData[i++] = Read_CV520(JREG_FIFODATA);

            n = Read_CV520(JREG_COMMIRQ);

            j--;
        } while ((0 == (n & waitFor)) && (j != 0));

        if ((j != 0) && !(n & 0x01)) {
            n = Read_CV520(JREG_ERROR);

            if (!(n & 0x0F))
                Status = MI_OK;
            else if (n & 0x08)
                Status = MI_COLLERR;
            else if (n & 0x04)
                Status = MI_CRCERR;
            else if (n & 0x02)
                Status = MI_PARITYERR;
            else if (n & 0x01)
                Status = MI_FRAMINGERR;

            if (recebyte) {
                n = Read_CV520(JREG_FIFOLEVEL);

                if (Status == MI_OK)
                    if ((Read_CV520(JREG_CONTROL) & 0x07) != 0x00)
                        Status = MI_SERNRERR;

                if (n == 0)
                    n = 1;

                g_sPcdTrsv.iLength = (n + i);

                for (j = 0; j < n; j++)
                    g_sPcdTrsv.cData[i + j] = Read_CV520(JREG_FIFODATA);
            }
        } else if (n & 0x01)
            Status = MI_NOTAGERR;
        else
            Status = MI_TRANSERR;

        Write_CV520(JREG_COMMIEN, 0x00);
        Write_CV520(JREG_COMMIRQ, 0x00);
        SetBitMask_CV520(JREG_CONTROL, 0x80); //	stop timer now
    }
    return Status;
}

/**
 * @brief Card reading interval
 *
 * @param cTmo
 */
void PcdSetTmo_CV520(uint8_t cTmo)
{
    uint32_t iTime;

    if (cTmo < 10) {
        Write_CV520(JREG_TMODE, 0x90);
        Write_CV520(JREG_TPRESCALER, 0x11); // TPre = 17;

        iTime = (1 << cTmo) * 128; // 35 Clock
    } else if (cTmo < 15) {
        Write_CV520(JREG_TMODE, 0x94);
        Write_CV520(JREG_TPRESCALER, 0x46); // TPre = 70;

        iTime = (2 << cTmo) - (1 << (cTmo - 10));
    } else {
        Write_CV520(JREG_TMODE, 0x90);
        Write_CV520(JREG_TPRESCALER, 0x20); // TPre = 32;

        iTime = cTmo;
    }

    Write_CV520(JREG_TRELOADHI, (uint8_t)(iTime >> 8));
    Write_CV520(JREG_TRELOADLO, (uint8_t)iTime);

    return;
}

/**
 * @brief
 *
 * @param cFSDI
 */
void PcdSetFSD_CV520(uint8_t cFSDI)
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
 * @brief CV520 antenna switch
 *
 * @param cSta 0 / 1
 */
void PcdAntenna_CV520(uint8_t cSta)
{
    if (cSta)
        SetBitMask_CV520(JREG_TXCONTROL, 0x03);
    else
        ClearBitMask(JREG_TXCONTROL, 0x03);
}

void CV520_Read_Card(void)
{
    uint8_t cStatus;

    PiccReset();

    cStatus = PiccRequest(PICC_REQALL, g_sCard.cATQ);

    if (cStatus != ERR_SUCCESS)
        cStatus = PiccRequest(PICC_REQALL, g_sCard.cATQ);

    if (cStatus == ERR_SUCCESS) {
        printf("Find Card!\r\n");

        LED1(1);

        Buzzer_Beep(Buzzer_True);

        cStatus = PiccAnticollSelect(&g_sCard.cLen, g_sCard.cUID, &g_sCard.cSAK);

        cStatus = PiccHaltA();

        cStatus = PiccRequest(PICC_REQIDL, g_sCard.cATQ);

        cStatus = PiccRequest(PICC_REQALL, g_sCard.cATQ);

        if (cStatus == ERR_SUCCESS)
            cStatus = PiccAnticollSelect(&g_sCard.cLen, g_sCard.cUID, &g_sCard.cSAK);
    } else {
        Delay_ms(10);

        gpio_output_high(22); // 蓝
        gpio_output_high(23); // 绿
        gpio_output_high(24); // 橙
        gpio_output_high(25); // 红
    }

    if (cStatus == ERR_SUCCESS) {
        if (g_sCard.cSAK & 0x20) {
            printf("\r\nCPU A Card UID:\r\n");

            for (uint8_t i = 0; i < 4; i++)
                printf("0x%02X ", g_sCard.cUID[i]);

            printf("\r\n");

            cStatus = ISO14443A_Rst(&g_cATSLen, g_cATS);

            Delay_s(5);
        }
    }
}