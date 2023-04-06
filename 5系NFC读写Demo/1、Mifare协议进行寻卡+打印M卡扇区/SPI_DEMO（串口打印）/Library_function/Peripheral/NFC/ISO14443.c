/**
 * @file ISO14443.c
 * @author Yikui Zhang (xingdala@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-10-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ISO14443.h"
#include "String.h"
#include "stdio.h"
#include "Status.h"
#include "CV520.h"
#include "RC52x.h"
#include "stdint.h"

#include "main.h"

extern tpd_Card g_sCard;
extern tpd_pcdpara g_sPcdPara;
extern tpd_pcdtrsv g_sPcdTrsv;

/**
 * @brief Restart the antenna
 *
 */
void PiccReset(void)
{
    PcdAntenna_CV520(0); // Turn off the antenna

    Delay_ms(10);

    PcdAntenna_CV520(1); // Turn on the antenna

    Delay_ms(10);

    return;
}

/**
 * @brief
 *
 * @param cReq 0x26=PICC_REQIDL; 0x52=PICC_REQALL;
 * @param cATQA Answer To Request
 * @return uint8_t Status
 */
uint8_t PiccRequest(uint8_t cReq, uint8_t *cATQA)
{
    uint8_t Status;

    PcdConfig(PCD_TYPE_14443A);

    PcdSetTmo_CV520(3);

    Write_CV520(JREG_RXMODE, 0x00); // Disnable Rx CRC
    Write_CV520(JREG_TXMODE, 0x00); // Disnable Tx CRC
    ClearBitMask(JREG_STATUS2, 0x08);
    Write_CV520(JREG_BITFRAMING, 0x07);
    // SetBitMask_CV520(JREG_TXCONTROL, 0x03);

    g_sPcdTrsv.cData[0] = cReq;
    g_sPcdTrsv.iLength  = 1;

    Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

    if (Status == ERR_SUCCESS) {
        if (g_sPcdTrsv.iLength == 2)
            memcpy(cATQA, g_sPcdTrsv.cData, 2);
        else
            Status = MI_BITCOUNTERR;
    }

    return Status;
}

/**
 * @brief
 *
 * @param cLev PICC_ANTICOLL1: standard select code; PICC_ANTICOLL2: cascaded level 1; PICC_ANTICOLL3: cascaded level 2
 * @param cUID 4 bytes serial number, and 1 byte BCC.
 * @return uint8_t Status
 */
uint8_t PiccAnticoll(uint8_t cLev, uint8_t *cUID)
{
    uint8_t Status;

    PcdSetTmo_CV520(3);

    ClearBitMask(JREG_RXMODE, 0x80);    // Enable Rx CRC
    ClearBitMask(JREG_TXMODE, 0x80);    // Enable Tx CRC
    Write_CV520(JREG_BITFRAMING, 0x00); //
    ClearBitMask(JREG_COLL, 0x80);

    g_sPcdTrsv.iLength  = 0x02;
    g_sPcdTrsv.cData[0] = cLev;
    g_sPcdTrsv.cData[1] = 0x20;

    Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

    if (Status == ERR_SUCCESS)
        memcpy(cUID, g_sPcdTrsv.cData, g_sPcdTrsv.iLength);

    return Status;
}

/**
 * @brief
 *
 * @param cLev
 * @param cUID 4 bytes serial number
 * @param cSAK 1 byte select acknowledge
 * @return uint8_t Status
 */
uint8_t PiccSelect(uint8_t cLev, uint8_t *cUID, uint8_t *cSAK)
{
    uint8_t Status;

    PcdSetTmo_CV520(3);

    SetBitMask_CV520(JREG_RXMODE, 0x80);
    SetBitMask_CV520(JREG_TXMODE, 0x80);
    ClearBitMask(JREG_STATUS2, 0x08);

    g_sPcdTrsv.iLength  = 0x07;
    g_sPcdTrsv.cData[0] = cLev;
    g_sPcdTrsv.cData[1] = 0x70;

    memcpy(&g_sPcdTrsv.cData[2], cUID, 5);

    Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

    if (Status == ERR_SUCCESS) {
        if (g_sPcdTrsv.iLength != 0x01)
            Status = MI_BITCOUNTERR;
        *cSAK = g_sPcdTrsv.cData[0];
    }

    return Status;
}

/**
 * @brief
 *
 * @return uint8_t Status
 */
uint8_t PiccHaltA(void)
{
    uint8_t Status;

    g_sPcdTrsv.cData[0] = PICC_HALT;
    g_sPcdTrsv.cData[1] = 0x00;
    g_sPcdTrsv.iLength  = 0x02;
    Status              = PcdTransceive(PCD_CMD_TRANSMIT);

    if (Status == MI_NOTAGERR)
        Status = ERR_SUCCESS;
    // PcdIdle();

    return Status;
}

/**
 * @brief
 *
 * @param cLen
 * @param cUID
 * @param cSAK
 * @return uint8_t Status
 */
uint8_t PiccAnticollSelect(uint8_t *cLen, uint8_t *cUID, uint8_t *cSAK)
{
    uint8_t Status;

    Status = PiccAnticoll(PICC_ANTICOLL1, cUID);

    if (Status == ERR_SUCCESS)
        Status = PiccSelect(PICC_ANTICOLL1, cUID, cSAK);

    if (Status == ERR_SUCCESS) {
        *cLen = 0x04;

        if (*cSAK & 0x04) {
            memcpy(cUID, &cUID[1], 3);

            Status = PiccAnticoll(PICC_ANTICOLL2, &cUID[3]);

            if (Status == ERR_SUCCESS)
                Status = PiccSelect(PICC_ANTICOLL2, &cUID[3], cSAK);

            if (Status == ERR_SUCCESS) {
                *cLen = 0x07;

                if (*cSAK & 0x04) {
                    memcpy(&cUID[3], &cUID[4], 3);

                    Status = PiccAnticoll(PICC_ANTICOLL3, &cUID[6]);

                    if (Status == ERR_SUCCESS)
                        Status = PiccSelect(PICC_ANTICOLL3, &cUID[6], cSAK);

                    if (Status == ERR_SUCCESS)
                        *cLen = 0x0A;
                }
            }
        }
    }
    return Status;
}

/**
 * @brief
 *
 * @param cpLen
 * @param Data
 * @return uint8_t Status
 */
uint8_t PiccTypeA_Rst(uint8_t *cpLen, uint8_t *Data)
{
    uint8_t Status;
    PcdSetTmo_CV520(4);

    g_sPcdTrsv.cData[0] = PICC_RESET;
    g_sPcdTrsv.cData[1] = g_sPcdPara.cFSD;
    g_sPcdTrsv.iLength  = 2;

    Status = PcdTransceive(PCD_CMD_TRANSCEIVE);
    if (Status == ERR_SUCCESS) {
        memcpy(Data, g_sPcdTrsv.cData, g_sPcdTrsv.iLength);
        *cpLen = g_sPcdTrsv.iLength;
    }
    return Status;
}

/**
 * @brief cFSCI Max INF Length
 *
 * @param cFSCI
 */
void PiccCaluINF(uint8_t cFSCI)
{
    uint8_t cCount = 0x00;

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
void PiccWaitWTX(uint8_t cTimeOut, uint8_t cWTX)
{
    if (cWTX == 0 || cWTX > 59)
        PcdSetTmo_CV520(cTimeOut);
    else
        // Write_CV520(JREG_TRELOADLO, 0x04 * cWTX + 1);

        return;
}

/**
 * @brief
 *
 * @param cpLen
 * @param cpRATS
 * @return uint8_t Status
 */
uint8_t ISO14443A_Rst(uint8_t *cpLen, uint8_t *cpRATS)
{
    uint8_t Status;
    uint8_t i;

    g_sPcdPara.cFWI = 0x0A;
    g_sPcdPara.cFSD = 0x40;
    g_sPcdPara.cCID = 0x00;
    g_sPcdPara.cNAD = 0x00;
    g_sPcdPara.cPCB = 0x00;

    Status = PiccTypeA_Rst(cpLen, cpRATS);

    if (Status == ERR_SUCCESS && cpRATS[0] > 0x01) {
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

    return Status;
}

#define PCB_I_BLOCK 0x02
#define PCB_R_BLOCK 0xA2
#define PCB_S_BLOCK 0xC2

/**
 * @brief CPU Card Write
 *
 * @param cpLen Data length
 * @param cpAPDU Data
 * @return uint8_t Status
 */
uint8_t ISO14443_COS(unsigned short *cpLen, uint8_t *cpAPDU)
{
    uint8_t Status;
    // uint8_t cErr;
    uint8_t cCurIndex;
    uint8_t cSendLenNow = 0x00;
    uint8_t cRecvLenNow = 0x00;
    uint32_t iSendIndex = 0x00;
    uint32_t iRecvIndex = 0x00;

    PcdSetTmo_CV520(g_sPcdPara.cFWI);

    g_sPcdTrsv.cData[0] = 0xA2;

    do {
        if ((g_sPcdTrsv.cData[0] & 0xF2) == 0xA2) {
            cCurIndex = 0x00;

            if ((iSendIndex + g_sPcdPara.cMaxINF) < *cpLen) {
                g_sPcdTrsv.cData[cCurIndex++] = PCB_I_BLOCK | 0x10 | g_sPcdPara.cCID | g_sPcdPara.cNAD | g_sPcdPara.cPCB; // I-Block, Chaining bit, CID, NAD, Block Number

                if (g_sPcdPara.cCID == 0x08)
                    g_sPcdTrsv.cData[cCurIndex++] = 0x00;

                if (g_sPcdPara.cNAD == 0x04)
                    g_sPcdTrsv.cData[cCurIndex++] = 0x00;

                cSendLenNow = g_sPcdPara.cMaxINF;
            } else {
                g_sPcdTrsv.cData[cCurIndex++] = PCB_I_BLOCK | g_sPcdPara.cCID | g_sPcdPara.cNAD | g_sPcdPara.cPCB;

                if (g_sPcdPara.cCID == 0x08)
                    g_sPcdTrsv.cData[cCurIndex++] = 0x00;

                if (g_sPcdPara.cNAD == 0x04)
                    g_sPcdTrsv.cData[cCurIndex++] = 0x00;

                cSendLenNow = (uint8_t)(*cpLen - iSendIndex);
            }

            memcpy(&g_sPcdTrsv.cData[cCurIndex], &cpAPDU[iSendIndex], cSendLenNow);

            g_sPcdTrsv.iLength = cCurIndex + cSendLenNow;
            g_sPcdPara.cPCB ^= 0x01;
            iSendIndex += cSendLenNow;
        } else if ((g_sPcdTrsv.cData[0] & 0xF0) == 0x10) {
            cCurIndex = 1;

            if (g_sPcdTrsv.cData[0] & 0x08)
                cCurIndex++;

            if (g_sPcdTrsv.cData[0] & 0x04)
                cCurIndex++;

            cRecvLenNow = g_sPcdTrsv.iLength - cCurIndex;

            memcpy(&cpAPDU[iRecvIndex], &g_sPcdTrsv.cData[cCurIndex], cRecvLenNow);

            iRecvIndex += cRecvLenNow;
            cCurIndex = 1;

            g_sPcdTrsv.cData[0] = PCB_R_BLOCK | g_sPcdPara.cCID | g_sPcdPara.cPCB;

            if (g_sPcdPara.cCID)
                g_sPcdTrsv.cData[cCurIndex++] = 0x00;

            g_sPcdTrsv.iLength = cCurIndex;
            g_sPcdPara.cPCB ^= 0x01;
        } else if ((g_sPcdTrsv.cData[0] & 0xF2) == 0x02) {
            cCurIndex = 1;

            if (g_sPcdTrsv.cData[0] & 0x08)
                cCurIndex++;

            if (g_sPcdTrsv.cData[0] & 0x04)
                cCurIndex++;

            cRecvLenNow = g_sPcdTrsv.iLength - cCurIndex;

            memcpy(&cpAPDU[iRecvIndex], &g_sPcdTrsv.cData[cCurIndex], cRecvLenNow);

            *cpLen = iRecvIndex + cRecvLenNow;

            break;
        } else if ((g_sPcdTrsv.cData[0] & 0xF2) == 0xF2) {
            cCurIndex = 1;

            if (g_sPcdTrsv.cData[0] & 0x08)
                cCurIndex = 2;

            PiccWaitWTX(g_sPcdPara.cFWI, g_sPcdTrsv.cData[cCurIndex]);
        } else {
            Status = ERR_FAILURE;
            break;
        }

        Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

        if (Status == ERR_SUCCESS)
            break;

    } while (Status == ERR_SUCCESS);

    return Status;
}

/**
 * @brief Mifare卡验证
 *
 * @param cSector 扇区(0-15)
 * @param cMode PICC_AUTHENT1A; PICC_AUTHENT1B
 * @return uint8_t Status
 */
uint8_t PiccAuthState(uint8_t cSector, uint8_t cMode, uint8_t *key)
{
    uint8_t Status;
    PcdSetTmo_CV520(5);

    g_sPcdTrsv.cData[0] = cMode;

    g_sPcdTrsv.cData[1] = cSector * 4 + 3;

    memcpy(&g_sPcdTrsv.cData[2], key, 6);
    memcpy(&g_sPcdTrsv.cData[8], g_sCard.cUID, 4);

    g_sPcdTrsv.iLength = 12;

    Status = PcdTransceive(PCD_CMD_MFAUTHENT);

    if (Status == MI_OK) {
        Status = Read_CV520(JREG_STATUS2);

        if (Status & 0x08)
            Status = MI_OK;
        else
            Status = MI_AUTHERR;
    }

    return Status;
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
    uint8_t Status;

    PcdSetTmo_CV520(4);

    SetBitMask_CV520(JREG_RXMODE, 0x80);
    SetBitMask_CV520(JREG_TXMODE, 0x80);

    g_sPcdTrsv.cData[1] = cSector * 4 + CBlock; // 必须先引用
    g_sPcdTrsv.cData[0] = PICC_READ;
    g_sPcdTrsv.iLength  = 2;

    Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

    if (Status == ERR_SUCCESS) {
        if (g_sPcdTrsv.iLength != 0x10)
            Status = MI_BYTECOUNTERR;
        else
            memcpy(Data, g_sPcdTrsv.cData, 0x10);
    }

    return Status;
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
    uint8_t Status;

    PcdSetTmo_CV520(5);

    ClearBitMask(JREG_RXMODE, 0x80);
    SetBitMask_CV520(JREG_TXMODE, 0x80);

    g_sPcdTrsv.cData[0] = PICC_WRITE;
    g_sPcdTrsv.cData[1] = cSector * 4 + CBlock;
    g_sPcdTrsv.iLength  = 2;

    Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

    if (Status == MI_SERNRERR) {
        g_sPcdTrsv.cData[0] &= 0x0F;

        if (g_sPcdTrsv.cData[0] == 0x0A)
            Status = ERR_SUCCESS;
        else
            Status = MI_WRITEERR;
    }

    if (Status == ERR_SUCCESS) {
        PcdSetTmo_CV520(5);

        memcpy(g_sPcdTrsv.cData, Data, 0x10);

        g_sPcdTrsv.iLength = 0x10;

        Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

        if (Status == MI_SERNRERR) {
            g_sPcdTrsv.cData[0] &= 0x0F;

            if (g_sPcdTrsv.cData[0] == 0x0A)
                Status = ERR_SUCCESS;
            else
                Status = MI_WRITEERR;
        }
    }
    return Status;
}

/**
 * @brief
 *
 * @param cFunc 0xC1 = Increment Value; 0xC0 = Decrement Value;
 * @param cBlock
 * @param cpValue
 * @return uint8_t
 */
uint8_t PiccValue(uint8_t cFunc, uint8_t cBlock, uint8_t *cpValue)
{
    uint8_t Status;
    PcdSetTmo_CV520(4);

    ClearBitMask(JREG_RXMODE, 0x80);
    SetBitMask_CV520(JREG_TXMODE, 0x80);

    g_sPcdTrsv.cData[0] = cFunc;
    g_sPcdTrsv.cData[1] = cBlock;
    g_sPcdTrsv.iLength  = 2;

    Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

    if (Status == MI_SERNRERR) {
        g_sPcdTrsv.cData[0] &= 0x0F;

        if (g_sPcdTrsv.iLength == 0x01 && g_sPcdTrsv.cData[0] == 0x0A) {
            PcdSetTmo_CV520(5);

            memcpy(g_sPcdTrsv.cData, cpValue, 0x04);

            g_sPcdTrsv.iLength = 0x04;

            Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

            if (Status == ERR_SUCCESS || Status == MI_NOTAGERR)
                Status = PiccTransfer(cBlock);
        }
    }
    return Status;
}

/**
 * @brief
 *
 * @param cBlock
 * @return uint8_t Status
 */
uint8_t PiccRestore(uint8_t cBlock)
{
    uint8_t Status;

    PcdSetTmo_CV520(4);

    ClearBitMask(JREG_RXMODE, 0x80);
    SetBitMask_CV520(JREG_TXMODE, 0x80);

    g_sPcdTrsv.cData[0] = PICC_RESTORE;
    g_sPcdTrsv.cData[1] = cBlock;
    g_sPcdTrsv.iLength  = 2;

    Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

    if (Status == MI_SERNRERR) {
        g_sPcdTrsv.cData[0] &= 0x0F;

        if (g_sPcdTrsv.iLength == 0x01 && g_sPcdTrsv.cData[0] == 0x0A) {
            PcdSetTmo_CV520(5);

            g_sPcdTrsv.cData[0] = 0x00;
            g_sPcdTrsv.cData[1] = 0x00;
            g_sPcdTrsv.cData[2] = 0x00;
            g_sPcdTrsv.cData[3] = 0x00;
            g_sPcdTrsv.iLength  = 0x04;

            Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

            if (Status == MI_NOTAGERR)
                Status = ERR_SUCCESS;
        }
    }
    return Status;
}

/**
 * @brief
 *
 * @param cBlock
 * @return uint8_t Status
 */
uint8_t PiccTransfer(uint8_t cBlock)
{
    uint8_t Status;

    PcdSetTmo_CV520(4);

    ClearBitMask(JREG_RXMODE, 0x80);
    SetBitMask_CV520(JREG_TXMODE, 0x80);

    g_sPcdTrsv.cData[0] = PICC_TRANSFER;
    g_sPcdTrsv.cData[1] = cBlock;
    g_sPcdTrsv.iLength  = 2;

    Status = PcdTransceive(PCD_CMD_TRANSCEIVE);

    if (Status == MI_SERNRERR) {
        g_sPcdTrsv.cData[0] &= 0x0F;

        if (g_sPcdTrsv.iLength == 0x01 && g_sPcdTrsv.cData[0] == 0x0A)
            Status = ERR_SUCCESS;
    }

    return Status;
}
