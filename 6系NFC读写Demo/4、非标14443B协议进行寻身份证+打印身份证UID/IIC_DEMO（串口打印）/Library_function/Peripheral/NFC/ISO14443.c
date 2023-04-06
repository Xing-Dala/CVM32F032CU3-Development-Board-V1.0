/**
 * @file ISO14443.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-11-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "main.h"

#include "ISO14443.h"
#include "String.h"

extern tpd_Card g_sCard;
extern tpd_pcdpara g_sPcdPara;
extern tpd_pcdtrsv g_sPcdTrsv;

/**
 * @brief 卡片复位
 *
 */
void PiccReset(void)
{
    PcdAntenna_CV663(OFF);
    Delay_ms(20);

    PcdAntenna_CV663(ON);
    Delay_ms(20);
}

/**
 * @brief 寻卡
 *
 * @param cReq 0x26=PICC_REQIDL; 0x52=PICC_REQALL;
 * @param cATQA Answer To Request
 * @return unsigned char
 */
unsigned char PiccRequest(unsigned char cReq, unsigned char *cATQA)
{
    unsigned char cStatus;

    PcdConfig_CV663(PCD_TYPE_14443A);

    PcdSetTmo_CV663(0);

    SetBitMask_CV663(PHHAL_HW_CV663_REG_DRVMODE, 0x08); // Enable Transmitter PIN
    Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x18);     // Disable TX CRC
    Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x18);     // Disable RX CRC
    Write_CV663(PHHAL_HW_CV663_REG_TXDATANUM, 0x0F);    // Sent last data is 7 bit; Need to reset.
    Write_CV663(PHHAL_HW_CV663_REG_RXBITCTRL, 0x00);

    SetBitMask_CV663(PHHAL_HW_CV663_REG_FRAMECON, 0xC0);
    ClearBitMask_CV663(PHHAL_HW_CV663_REG_STATUS, 0x20);

    g_sPcdTrsv.cData[0] = cReq;
    g_sPcdTrsv.iLength  = 1;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

    if (cStatus == ERR_SUCCESS)
        if (g_sPcdTrsv.iLength == 2)
            memcpy(cATQA, g_sPcdTrsv.cData, 2);
        else
            cStatus = MI_BITCOUNTERR;

    return cStatus;
}

/**
 * @brief 防碰撞
 *
 * @param cLev PICC_ANTICOLL1：标准选择代码；PICC_ANTICOLL2：级联级1；PICC_ANTICOLL3：级联级2；
 * @param cUID 4字节序列号和1字节BCC
 * @return unsigned char
 */
unsigned char PiccAnticoll(unsigned char cLev, unsigned char *cUID)
{
    unsigned char cStatus;

    PcdSetTmo_CV663(0);

    Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x18);
    Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x18);
    Write_CV663(PHHAL_HW_CV663_REG_TXDATANUM, 0x08);
    Write_CV663(PHHAL_HW_CV663_REG_RXBITCTRL, 0x00);

    g_sPcdTrsv.iLength  = 0x02;
    g_sPcdTrsv.cData[0] = cLev;
    g_sPcdTrsv.cData[1] = 0x20;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

    if (cStatus == ERR_SUCCESS)
        memcpy(cUID, g_sPcdTrsv.cData, g_sPcdTrsv.iLength);

    return cStatus;
}

/**
 * @brief 选定一张卡
 *
 * @param cLev
 * @param cUID 4字节序列号
 * @param cSAK 1字节选择确认
 * @return unsigned char
 */
unsigned char PiccSelect(unsigned char cLev, unsigned char *cUID, unsigned char *cSAK)
{
    unsigned char cStatus;

    PcdSetTmo_CV663(4);

    Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x19);
    Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x19);
    Write_CV663(PHHAL_HW_CV663_REG_RXBITCTRL, 0x00);

    g_sPcdTrsv.iLength  = 0x07;
    g_sPcdTrsv.cData[0] = cLev;
    g_sPcdTrsv.cData[1] = 0x70;
    memcpy(&g_sPcdTrsv.cData[2], cUID, 5);

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

    if (cStatus == ERR_SUCCESS) {
        if (g_sPcdTrsv.iLength != 0x01)
            cStatus = MI_BITCOUNTERR;

        *cSAK = g_sPcdTrsv.cData[0];
    }

    return cStatus;
}

/**
 * @brief 卡休眠
 *
 * @return unsigned char
 */
unsigned char PiccHaltA(void)
{
    unsigned char cStatus;

    g_sPcdTrsv.cData[0] = PICC_HALT;
    g_sPcdTrsv.cData[1] = 0x00;
    g_sPcdTrsv.iLength  = 0x02;
    cStatus             = PcdTransceive_CV663(PCD_CMD_TRANSMIT);
    if (cStatus == MI_NOTAGERR) {
        cStatus = ERR_SUCCESS;
    }
    // PcdIdle();

    return cStatus;
}

/**
 * @brief 三级防碰撞
 *
 * @param cLen
 * @param cUID
 * @param cSAK
 * @return unsigned char
 */
unsigned char PiccAnticollSelect(unsigned char *cLen, unsigned char *cUID, unsigned char *cSAK)
{
    unsigned char cStatus;

    // 一级防碰撞处理
    cStatus = PiccAnticoll(PICC_ANTICOLL1, cUID);

    if (cStatus == ERR_SUCCESS)
        cStatus = PiccSelect(PICC_ANTICOLL1, cUID, cSAK);

    if (cStatus == ERR_SUCCESS) {
        *cLen = 0x04;

        // 二级防碰撞处理
        if (*cSAK & 0x04) {
            memcpy(cUID, &cUID[1], 3);

            cStatus = PiccAnticoll(PICC_ANTICOLL2, &cUID[3]);

            if (cStatus == ERR_SUCCESS)
                cStatus = PiccSelect(PICC_ANTICOLL2, &cUID[3], cSAK);

            if (cStatus == ERR_SUCCESS) {
                *cLen = 0x07;

                // 三级防碰撞处理
                if (*cSAK & 0x04) {
                    memcpy(&cUID[3], &cUID[4], 3);

                    cStatus = PiccAnticoll(PICC_ANTICOLL3, &cUID[6]);

                    if (cStatus == ERR_SUCCESS)
                        cStatus = PiccSelect(PICC_ANTICOLL3, &cUID[6], cSAK);

                    if (cStatus == ERR_SUCCESS)
                        *cLen = 0x0A;
                }
            }
        }
    }

    return cStatus;
}

/**
 * @brief TypeA卡片复位功能
 *
 * @param cpLen
 * @param cpData
 * @return unsigned char
 */
unsigned char PiccTypeA_Rst(unsigned char *cpLen, unsigned char *cpData)
{
    unsigned char cStatus;
    PcdSetTmo_CV663(5);

    g_sPcdTrsv.cData[0] = PICC_RESET;
    g_sPcdTrsv.cData[1] = g_sPcdPara.cFSD;
    g_sPcdTrsv.iLength  = 2;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);
    if (cStatus == ERR_SUCCESS) {
        memcpy(cpData, g_sPcdTrsv.cData, g_sPcdTrsv.iLength);
        *cpLen = g_sPcdTrsv.iLength;
    }
    return cStatus;
}

/**
 * @brief FSCI转换为 Max INF Length
 *
 * @param cFSCI
 */
void PiccCaluINF(unsigned char cFSCI)
{
    unsigned char cCount = 0x00;

    if (g_sPcdPara.cCID == 0x08)
        cCount++;

    if (g_sPcdPara.cNAD == 0x04)
        cCount++;

    switch (cFSCI) {
        case 0: // 16 - PCB(1) - EDC(2) = 13
        case 1: // 24
        case 2: // 32
        case 3: // 40
        case 4: // 48
            g_sPcdPara.cMaxINF = cFSCI * 8 + 13 - cCount;
            break;
        case 5: // 64 - PCB(1) - EDC(2) = 61
            g_sPcdPara.cMaxINF = 61 - cCount;
            break;
        case 6: // 96 - PCB(1) - EDC(2) = 93
            g_sPcdPara.cMaxINF = 93 - cCount;
            break;
        case 7: // 128 - PCB(1) - EDC(2) = 125
            g_sPcdPara.cMaxINF = 125 - cCount;
            break;
        default: // 256 - PCB(1) - EDC(2) = 253
            g_sPcdPara.cMaxINF = 253 - cCount;
            break;
    }
    return;
}

/**
 * @brief
 *
 * @param cTimeOut
 * @param cWTX
 */
void PiccWaitWTX(unsigned char cTimeOut, unsigned char cWTX)
{
    if (cWTX == 0 || cWTX > 59)
        PcdSetTmo_CV663(cTimeOut);
    // else
    // Write_CV663(JREG_TRELOADLO, 0x04 * cWTX + 1);

    return;
}

/**
 * @brief
 *
 * @param cpLen
 * @param cpRATS
 * @return unsigned char
 */
unsigned char ISO14443A_Rst(unsigned char *cpLen, unsigned char *cpRATS)
{
    unsigned char cStatus;
    unsigned char i;

    // 初始化默认参数
    g_sPcdPara.cFWI = 0x0A; // 设置默认
    g_sPcdPara.cFSD = 0x40; // 设置默认读卡器能力
    g_sPcdPara.cCID = 0x00;
    g_sPcdPara.cNAD = 0x00;
    g_sPcdPara.cPCB = 0x00;

    cStatus = PiccTypeA_Rst(cpLen, cpRATS);

    if (cStatus == ERR_SUCCESS && cpRATS[0] > 0x01) {
        i = 2;

        if (cpRATS[1] & 0x10) // TA1
        {
            // cTemp = cpRATS[i] & (cpRATS[i] >> 4) & 0x07;
            i++;
        }

        if (cpRATS[1] & 0x20) // TB1
        {
            g_sPcdPara.cFWI = cpRATS[i] >> 4;
            i++;
        }

        if (cpRATS[1] & 0x40) // TC1
        {
            if (cpRATS[i] & 0x02)
                g_sPcdPara.cCID = 0x08;

            if (cpRATS[i] & 0x01)
                g_sPcdPara.cNAD = 0x04;
        }

        PiccCaluINF(cpRATS[1] & 0x0F);
    }

    return cStatus;
}

#define PCB_I_BLOCK 0x02
#define PCB_R_BLOCK 0xA2
#define PCB_S_BLOCK 0xC2

/**
 * @brief TypeA卡片写入
 *
 * @param cpLen
 * @param cpAPDU
 * @return unsigned char
 */
unsigned char ISO14443_COS(unsigned short *cpLen, unsigned char *cpAPDU)
{
    unsigned char cStatus;
    // unsigned char cErr;
    unsigned char cCurIndex;
    unsigned char cSendLenNow = 0x00;
    unsigned char cRecvLenNow = 0x00;
    unsigned int iSendIndex   = 0x00;
    unsigned int iRecvIndex   = 0x00;

    PcdSetTmo_CV663(g_sPcdPara.cFWI);

    g_sPcdTrsv.cData[0] = 0xA2; // 设置R(ACK)初值，可进入I-Block装配流程

    do {
        if ((g_sPcdTrsv.cData[0] & 0xF2) == 0xA2) // 收到R(ACK)应答   发送I(n)数据
        {
            cCurIndex = 0x00;

            if ((iSendIndex + g_sPcdPara.cMaxINF) < *cpLen) {
                // 发送数据长度大于卡片接收能力，分包发送。
                g_sPcdTrsv.cData[cCurIndex++] = PCB_I_BLOCK | 0x10 | g_sPcdPara.cCID | g_sPcdPara.cNAD | g_sPcdPara.cPCB; // I-Block, Chaining bit, CID, NAD, Block Number

                if (g_sPcdPara.cCID == 0x08)
                    g_sPcdTrsv.cData[cCurIndex++] = 0x00;

                if (g_sPcdPara.cNAD == 0x04)
                    g_sPcdTrsv.cData[cCurIndex++] = 0x00;

                cSendLenNow = g_sPcdPara.cMaxINF;
            } else {
                // 发送数据长度小于卡片接收能力，单包发送。
                g_sPcdTrsv.cData[cCurIndex++] = PCB_I_BLOCK | g_sPcdPara.cCID | g_sPcdPara.cNAD | g_sPcdPara.cPCB;

                if (g_sPcdPara.cCID == 0x08)
                    g_sPcdTrsv.cData[cCurIndex++] = 0x00;

                if (g_sPcdPara.cNAD == 0x04)
                    g_sPcdTrsv.cData[cCurIndex++] = 0x00;

                cSendLenNow = (unsigned char)(*cpLen - iSendIndex);
            }

            memcpy(&g_sPcdTrsv.cData[cCurIndex], &cpAPDU[iSendIndex], cSendLenNow);

            g_sPcdTrsv.iLength = cCurIndex + cSendLenNow;

            // 更新参数
            g_sPcdPara.cPCB ^= 0x01;                     // 更新PCB Block Number
            iSendIndex += cSendLenNow;                   // 更新发送完成索引
        } else if ((g_sPcdTrsv.cData[0] & 0xF0) == 0x10) // 收到I(1)数据   发送R(ACK)应答
        {
            // 接收数据，输出到cpAPDU
            cCurIndex = 1;
            if (g_sPcdTrsv.cData[0] & 0x08)
                cCurIndex++;

            if (g_sPcdTrsv.cData[0] & 0x04)
                cCurIndex++;

            cRecvLenNow = g_sPcdTrsv.iLength - cCurIndex;

            memcpy(&cpAPDU[iRecvIndex], &g_sPcdTrsv.cData[cCurIndex], cRecvLenNow);

            iRecvIndex += cRecvLenNow;

            // 配置 R(ACK)
            cCurIndex = 1;

            g_sPcdTrsv.cData[0] = PCB_R_BLOCK | g_sPcdPara.cCID | g_sPcdPara.cPCB;

            if (g_sPcdPara.cCID)
                g_sPcdTrsv.cData[cCurIndex++] = 0x00;

            g_sPcdTrsv.iLength = cCurIndex;

            // 更新参数
            g_sPcdPara.cPCB ^= 0x01;                     // 更新PCB Block Number
        } else if ((g_sPcdTrsv.cData[0] & 0xF2) == 0x02) // 收到I(0)数据   结束
        {
            // 接收数据，输出到cpAPDU
            cCurIndex = 1;
            if (g_sPcdTrsv.cData[0] & 0x08)
                cCurIndex++;

            if (g_sPcdTrsv.cData[0] & 0x04)
                cCurIndex++;

            cRecvLenNow = g_sPcdTrsv.iLength - cCurIndex;

            memcpy(&cpAPDU[iRecvIndex], &g_sPcdTrsv.cData[cCurIndex], cRecvLenNow);

            *cpLen = iRecvIndex + cRecvLenNow;

            // 数据接收完成，结束循环

            break;
        } else if ((g_sPcdTrsv.cData[0] & 0xF2) == 0xF2) // 收到S(WTX)   发送S(WTX)
        {
            cCurIndex = 1;

            if (g_sPcdTrsv.cData[0] & 0x08)
                cCurIndex = 2;

            // 等待超时
            PiccWaitWTX(g_sPcdPara.cFWI, g_sPcdTrsv.cData[cCurIndex]);
        } else {
            cStatus = ERR_FAILURE;
            break; // 未知PCB命令，结束循环
        }

        // 数据块收发机制
        // for (cErr=0; cErr<3; cErr++)
        //{
        cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE); // 收发正常，继续循环

        if (cStatus == ERR_SUCCESS)
            break;
        //}

    } while (cStatus == ERR_SUCCESS);

    return cStatus;
}

/**
 *  @brief ISO14443-3 Type B Function
 *  @struct: PiccRequestB
 *           PiccAttrib
 *           PiccHltb
 */

/**
 * @brief Type B 寻卡
 *
 * @param cReq 0:REQB; 1:WUPB
 * @param AFI
 * @param N = 0~7 Number of slots
 * @param cpATQB // 至少需要16字节的缓存
 * @return unsigned char
 */
unsigned char PiccRequestB(unsigned char cReq, unsigned char AFI, unsigned char N, unsigned char *cpATQB)
{
    unsigned char cStatus;

    PcdConfig_CV663(PCD_TYPE_14443B);

    PcdSetTmo_CV663(g_sPcdPara.cFWI);

    g_sPcdTrsv.cData[0] = 0x05;                              // APf code
    g_sPcdTrsv.cData[1] = AFI;                               // AFI
    g_sPcdTrsv.cData[2] = ((cReq << 3) & 0x08) | (N & 0x07); // PARAM
    g_sPcdTrsv.iLength  = 0x03;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

    if (cStatus == ERR_SUCCESS && g_sPcdTrsv.iLength == 0x0C) {
        memcpy(cpATQB, g_sPcdTrsv.cData, g_sPcdTrsv.iLength);
        // PcdSetTmo_CV663(cpATQB[11]>>4); // set FWT
    } else
        cStatus = ERR_FAILURE;

    return cStatus;
}

/**
 * @brief Type B 防碰撞
 *
 * @param PUPI
 * @param PARAM3
 * @param cFSDI
 * @param answer
 * @return unsigned char
 */
unsigned char PiccAttrib(unsigned char *PUPI, unsigned char PARAM3, unsigned char cFSDI, unsigned char *answer)
{
    unsigned char cStatus;

    PcdSetTmo_CV663(g_sPcdPara.cFWI);

    g_sPcdTrsv.cData[0] = 0x1D;

    memcpy(&g_sPcdTrsv.cData[1], PUPI, 4);

    g_sPcdTrsv.cData[5] = 0x00;   // EOF/SOF required, default TR0/TR1
    g_sPcdTrsv.cData[6] = cFSDI;  // Max frame 64
    g_sPcdTrsv.cData[7] = PARAM3; // Param3, ISO/IEC 14443-4 compliant?
    g_sPcdTrsv.cData[8] = 0x00;   // CID

    g_sPcdTrsv.iLength = 0x09;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

    *answer = g_sPcdTrsv.cData[0];

    return cStatus;
}

// /**
//  * @brief
//  *
//  * @param PUPI
//  * @return unsigned char
//  */
// unsigned char PiccHaltB(unsigned char *PUPI)
// {
// 	unsigned char cStatus;

// 	g_sPcdTrsv.cData[0] = 0x50;
// 	memcpy(&g_sPcdTrsv.cData[1], PUPI, 4);
// 	g_sPcdTrsv.iLength = 0x05;

// 	cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

// 	return cStatus;
// }

/**
 * @brief
 *
 * @param cReq bReq meaning 0x00 = REQB; 0x01 =  WUPB;
 * @param pLen
 * @param pATQB
 * @return unsigned char
 */
unsigned char ISO14443B_Rst(unsigned char cReq, unsigned char *pLen, unsigned char *pATQB)
{
    unsigned char cStatus;
    unsigned char cFSCI;
    // unsigned char cTemp;

    g_sPcdPara.cFWI = 0x04; // 设置默认
    g_sPcdPara.cFSD = 0x40; // 设置默认读卡器能力
    g_sPcdPara.cCID = 0x08; // 身份证设置为0x08，普通B卡设置为0x00。
    g_sPcdPara.cNAD = 0x00;
    g_sPcdPara.cPCB = 0x00;

    cStatus = PiccRequestB(cReq, 0, 0, pATQB);
    if (cStatus == ERR_SUCCESS) {
        cFSCI = pATQB[10] >> 4;

        g_sPcdPara.cFWI = pATQB[11] >> 4;

        if (pATQB[11] & 0x01)
            g_sPcdPara.cCID = 0x08;

        if (pATQB[11] & 0x02)
            g_sPcdPara.cNAD = 0x04;

        PiccCaluINF(cFSCI);

        // cStatus = PiccAttrib(&pATQB[1], pATQB[10] & 0x0F, cFSCI, &pATQB[12]); // 普通B卡参数设置
        cStatus = PiccAttrib(&pATQB[1], 0x01, 0x08, &pATQB[12]); // 身份证参数设置

        if (cStatus == ERR_SUCCESS)
            *pLen = 12;
    }

    return cStatus;
}

/**
 * @brief Mifare卡验证
 *
 * @param cSector 扇区(0-15)
 * @param cMode PICC_AUTHENT1A; PICC_AUTHENT1B
 * @return uint8_t Status
 */
uint8_t PiccAuthState(uint8_t cSector, uint8_t cMode, uint8_t* key)
{
    unsigned char cStatus;

    PcdSetTmo_CV663(5);

    memcpy(&g_sPcdTrsv.cData, key, 6);

    g_sPcdTrsv.iLength = 6;

    cStatus = PcdTransceive_CV663(PCD_CMD_LOADKEY);

    if (cStatus == ERR_SUCCESS) {
        if (cMode & 0x80)
            g_sPcdTrsv.cData[0] = PICC_AUTHENT1B;
        else
            g_sPcdTrsv.cData[0] = PICC_AUTHENT1A;

        g_sPcdTrsv.cData[1] = cSector * 4 + 3;

        memcpy(&g_sPcdTrsv.cData[2], g_sCard.cUID, 4);

        g_sPcdTrsv.iLength = 6;

        cStatus = PcdTransceive_CV663(PCD_CMD_MFAUTHENT);
    }

    if (cStatus == ERR_SUCCESS) {
        // cStatus = Read_CV663(JREG_STATUS2); // WAIT ADD

        if (Read_CV663(PHHAL_HW_CV663_REG_STATUS) & 0x20)
            cStatus = ERR_SUCCESS;
        else
            cStatus = MI_AUTHERR;
    }

    return cStatus;
}

/**
 * @brief Mifare卡读取一个扇区的数据
 *
 * @param cSector 扇区，cAddr = 0--15: Mifare S50; cAddr = 0--63: Mifare S70;
 * @param CBlock 区块，0-3
 * @param Data 数据
 * @return uint8_t
 */
uint8_t PiccRead(uint8_t cSector, uint8_t CBlock, uint8_t *Data)
{
    unsigned char cStatus;

    PcdSetTmo_CV663(5);

    Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x19);
    Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x19);
    Write_CV663(PHHAL_HW_CV663_REG_TXDATANUM, 0x08);
    Write_CV663(PHHAL_HW_CV663_REG_RXBITCTRL, 0x00);

    g_sPcdTrsv.cData[1] = cSector * 4 + CBlock; // 必须先引用
    g_sPcdTrsv.cData[0] = PICC_READ;
    g_sPcdTrsv.iLength  = 2;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

    if (cStatus == ERR_SUCCESS) {
        if (g_sPcdTrsv.iLength != 0x10)
            cStatus = MI_BYTECOUNTERR;
        else
            memcpy(Data, g_sPcdTrsv.cData, 0x10);
    }

    return cStatus;
}

/**
 * @brief 给Mifare卡的一个区块写入16字节
 *
 * @param cSector 扇区，cAddr = 0--15: Mifare S50; cAddr = 0--63: Mifare S70;
 * @param CBlock 区块，0-3
 * @param Data 数据
 * @return uint8_t Status
 */
uint8_t PiccWrite(uint8_t cSector, uint8_t CBlock, uint8_t *Data)
{
    unsigned char cStatus;

    PcdSetTmo_CV663(5);

    Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x19);
    Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x18);

    g_sPcdTrsv.cData[0] = PICC_WRITE;
    g_sPcdTrsv.cData[1] = cSector * 4 + CBlock;
    g_sPcdTrsv.iLength  = 2;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

    if (cStatus == MI_SERNRERR) {
        g_sPcdTrsv.cData[0] &= 0x0F;

        if (g_sPcdTrsv.cData[0] == 0x0A)
            cStatus = ERR_SUCCESS;
        else
            cStatus = MI_WRITEERR;
    }

    if (cStatus == ERR_SUCCESS) {
        PcdSetTmo_CV663(5);

        memcpy(g_sPcdTrsv.cData, Data, 0x10);

        g_sPcdTrsv.iLength = 0x10;

        cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

        if (cStatus == MI_SERNRERR) {
            g_sPcdTrsv.cData[0] &= 0x0F;

            if (g_sPcdTrsv.cData[0] == 0x0A)
                cStatus = ERR_SUCCESS;
            else
                cStatus = MI_WRITEERR;
        }
    }

    return cStatus;
}

/**
 * @brief 钱包功能函数
 *
 * @param cFunc 0xC1: 加钱; 0xC0: 减钱;
 * @param cBlock 扇区区块
 * @param cpValue 4字节
 * @return unsigned char
 */
unsigned char PiccValue(unsigned char cFunc, unsigned char cBlock, unsigned char *cpValue)
{
    unsigned char cStatus;

    PcdSetTmo_CV663(5);

    Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x18);
    Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x19);

    g_sPcdTrsv.cData[0] = cFunc;
    g_sPcdTrsv.cData[1] = cBlock;
    g_sPcdTrsv.iLength  = 2;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

    if (cStatus == MI_SERNRERR) {
        g_sPcdTrsv.cData[0] &= 0x0F;

        if (g_sPcdTrsv.iLength == 0x01 && g_sPcdTrsv.cData[0] == 0x0A) {
            PcdSetTmo_CV663(5);

            memcpy(g_sPcdTrsv.cData, cpValue, 0x04);

            g_sPcdTrsv.iLength = 0x04;

            cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

            if (cStatus == ERR_SUCCESS || cStatus == MI_NOTAGERR)
                cStatus = PiccTransfer(cBlock);
        }
    }

    return cStatus;
}

/**
 * @brief 扇区备份
 *
 * @param cBlock 扇区区块
 * @return unsigned char
 */
unsigned char PiccRestore(unsigned char cBlock)
{
    unsigned char cStatus;

    PcdSetTmo_CV663(5);

    Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x18);
    Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x19);

    g_sPcdTrsv.cData[0] = PICC_RESTORE;
    g_sPcdTrsv.cData[1] = cBlock;
    g_sPcdTrsv.iLength  = 2;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

    if (cStatus == MI_SERNRERR) {
        g_sPcdTrsv.cData[0] &= 0x0F;

        if (g_sPcdTrsv.iLength == 0x01 && g_sPcdTrsv.cData[0] == 0x0A) {
            PcdSetTmo_CV663(5);

            g_sPcdTrsv.cData[0] = 0x00;
            g_sPcdTrsv.cData[1] = 0x00;
            g_sPcdTrsv.cData[2] = 0x00;
            g_sPcdTrsv.cData[3] = 0x00;
            g_sPcdTrsv.iLength  = 0x04;

            cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);

            if (cStatus == MI_NOTAGERR)
                cStatus = ERR_SUCCESS;
        }
    }

    return cStatus;
}

/**
 * @brief 执行钱包功能操作
 *
 * @param cBlock
 * @return unsigned char
 */
unsigned char PiccTransfer(unsigned char cBlock)
{
    unsigned char cStatus;

    PcdSetTmo_CV663(5);

    Write_CV663(PHHAL_HW_CV663_REG_RXCRCCON, 0x18);
    Write_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x19);

    g_sPcdTrsv.cData[0] = PICC_TRANSFER;
    g_sPcdTrsv.cData[1] = cBlock;
    g_sPcdTrsv.iLength  = 2;

    cStatus = PcdTransceive_CV663(PCD_CMD_TRANSCEIVE);
    if (cStatus == MI_SERNRERR) {
        g_sPcdTrsv.cData[0] &= 0x0F;

        if (g_sPcdTrsv.iLength == 0x01 && g_sPcdTrsv.cData[0] == 0x0A)
            cStatus = ERR_SUCCESS;
    }

    return cStatus;
}
