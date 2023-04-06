/**
 * @file ISO15693.c
 * @author zhangyikui
 * @brief
 * @version 0.1
 * @date 2023-03-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"
#include "PCD.h"

extern tpd_pcdpara g_sPcdPara;
extern tpd_pcdtrsv g_sPcdTrsv;

/**
 * @brief 清查卡片
 *
 * @param Flags 标志
 * @param AFI
 * @param Mask_Length 掩码长度
 * @param Mask 掩码值
 * @param cpReceiveLength 数据应答长度
 * @param cpReceiveData 应答数据
 * @return unsigned char
 */
unsigned char ISO15693_Inventory(unsigned char Flags, unsigned char AFI,
                                 unsigned char Mask_Length, unsigned char *Mask,
                                 unsigned char *cpReceiveLength, unsigned char *cpReceiveData)
{
    unsigned char cIndex, cByteCnt;
    unsigned char cStatus = 0xFF;
    unsigned char i, j, k;
    unsigned char cTempStatus;

    *cpReceiveLength = 0;

    g_sPcdTrsv.cData[0] = Flags;              // Flag
    g_sPcdTrsv.cData[1] = ISO15693_INVENTORY; // Command
    cIndex              = 2;

    if ((Flags & 0x10) != 0x00)
        g_sPcdTrsv.cData[cIndex++] = AFI;

    g_sPcdTrsv.cData[cIndex++] = Mask_Length;

    if (Mask_Length % 8)
        cByteCnt = Mask_Length / 8 + 1;
    else
        cByteCnt = Mask_Length / 8;

    if (cByteCnt) {
        memcpy(&(g_sPcdTrsv.cData[cIndex]), Mask, cByteCnt);
        cIndex += cByteCnt;
    }

    g_sPcdTrsv.iLength = cIndex;

    if ((Flags & 0x20) != 0x00) {
        SetTimeOut();

        cStatus = PcdTransceive_CV663(PCD_TRANSCEIVE);

        if (cStatus == 0) {

            j = g_sPcdTrsv.iLength - 1;

            if ((g_sPcdTrsv.cData[0] & 0x01) == 1)
                cStatus = 0xFF;
            else {
                memcpy(cpReceiveData, &g_sPcdTrsv.cData[1], j);
                *cpReceiveLength = j;
            }
        }
    } else {

        SetTimeOut();
        PcdTransceive_CV663(PCD_TRANSMIT);

        j = 0;

        for (i = 0; i < 16; i++) {
            ClearBitMask_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x01); // ClearBitMask_CV663(RegChannelRedundancy, 0x04);
            // SetBitMask_CV663(RegCoderControl, 0x80);
            // SetBitMask_CV663(RegTxControl, 0x10);

            g_sPcdTrsv.iLength = 0x00;
            cTempStatus        = PcdTransceive_CV663(PCD_TRANSCEIVE);

            if ((cTempStatus == MI_OK) && (g_sPcdTrsv.iLength == 0x50)) {

                cStatus = MI_OK;

                *cpReceiveLength += 9;

                for (k = 0; k < 9; k++)
                    cpReceiveData[k + 9 * j] = g_sPcdTrsv.cData[1 + k];

                j++;
            }
        }

        // ClearBitMask_CV663(RegCoderControl, 0x80);
        SetBitMask_CV663(PHHAL_HW_CV663_REG_TXCRCCON, 0x01); // SetBitMask_CV663(RegChannelRedundancy, 0x04);
    }
    return cStatus;
}

/**
 * @brief 选择一个标签
 *
 * @param Flags
 * @param UID
 * @return unsigned char
 */
unsigned char ISO15693_Select(unsigned char Flags, unsigned char *UID)
{
    unsigned char cStatus;
    //	unsigned char i;
    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    g_sPcdTrsv.iLength  = 10;
    g_sPcdTrsv.cData[0] = Flags;           // Flag
    g_sPcdTrsv.cData[1] = ISO15693_SELECT; // Command

    memcpy(&g_sPcdTrsv.cData[2], UID, 8);
    SetTimeOut();
    cStatus = PcdTransceive_CV663(PCD_TRANSMIT);
    return cStatus;
}

/**
 * @brief 卡保持安静
 *
 * @param Flags
 * @param UID
 * @return unsigned char
 */
unsigned char ISO15693_StayQuiet(unsigned char Flags, unsigned char *UID)
{
    unsigned char cStatus;
    //	unsigned char i;
    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    g_sPcdTrsv.iLength  = 10;
    g_sPcdTrsv.cData[0] = Flags;               // Flag
    g_sPcdTrsv.cData[1] = ISO15693_STAY_QUIET; // Command

    memcpy(&g_sPcdTrsv.cData[2], UID, 8);
    SetTimeOut();
    cStatus = PcdTransceive_CV663(PCD_TRANSMIT);
    return cStatus;
}

/**
 * @brief 读取标签指定单块 / 多块内容
 *
 * @param Flags 标志位 单卡0x26 / 0x36
 * @param UID 15693UID
 * @param cBlockNumber 块编号起点
 * @param cNumberOfBlock 读取块的数量，一个块为0
 * @param cpReceiveLength 数据长度
 * @param cpReceiveData 数据
 * @return unsigned char
 */
unsigned char ISO15693_ReadBlock(
    unsigned char Flags, unsigned char *UID,
    unsigned char cBlockNumber, unsigned char cNumberOfBlock,
    unsigned char *cpReceiveLength, unsigned char *cpReceiveData)
{
    unsigned char cStatus;
    //	unsigned char i;
    unsigned char cIndex;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    g_sPcdTrsv.cData[0] = Flags; // Flag

    if (cNumberOfBlock > 0)
        g_sPcdTrsv.cData[1] = ISO15693_READ_MULTIPLE_BLOCK; // Command
    else
        g_sPcdTrsv.cData[1] = ISO15693_READ_SINGLE_BLOCK; // Command

    cIndex = 2;

    if (Flags & 0x20) { // flags & 0x20 - Adresflag request flags & 0x10 - Selectflag request
        memcpy(&g_sPcdTrsv.cData[cIndex], UID, 8);
        cIndex += 8;
    }

    g_sPcdTrsv.cData[cIndex++] = cBlockNumber;

    if (cNumberOfBlock > 0)
        g_sPcdTrsv.cData[cIndex++] = cNumberOfBlock;

    g_sPcdTrsv.iLength = cIndex;

    SetTimeOut();

    cStatus = PcdTransceive_CV663(PCD_TRANSCEIVE);

    *cpReceiveLength = 0;

    if (cStatus == 0) {

        unsigned char j = (g_sPcdTrsv.iLength / 8) - 1;

        if ((g_sPcdTrsv.cData[0] & 0x01) == 1)
            cStatus = 0xFF;
        else {
            memcpy(cpReceiveData, &g_sPcdTrsv.cData[1], j);
            *cpReceiveLength = j;
        }
    }
    return cStatus;
}

/**
 * @brief
 *
 * @param Flags
 * @return unsigned char
 */
unsigned char TI_Process(unsigned char Flags)
{
    unsigned char cStatus = 0;

    if ((Flags & 0x40) == 0) {
        if ((g_sPcdTrsv.cData[0] & 0x01) == 1)
            cStatus = 0xFF;
    } else {
        Delay_us(100);

        ClearBitMask_CV663(PHHAL_HW_CV663_REG_FRAMECON, 0x03); // SetBitMask_CV663(RegCoderControl, 0x80);
        ClearBitMask_CV663(PHHAL_HW_CV663_REG_TXDATANUM, 0x08);

        g_sPcdTrsv.iLength = 0;
        cStatus            = PcdTransceive_CV663(PCD_TRANSCEIVE);

        SetBitMask_CV663(PHHAL_HW_CV663_REG_FRAMECON, 0x03); // ClearBitMask_CV663(RegCoderControl, 0x80);
        SetBitMask_CV663(PHHAL_HW_CV663_REG_TXDATANUM, 0x08);

        if (cStatus == 0)
            if ((g_sPcdTrsv.cData[0] & 0x01) == 1)
                cStatus = 0xFF;
    }
    return cStatus;
}

/**
 * @brief 将指定信息写入标签指定单块位置
 *
 * @param Flags
 * @param UID
 * @param cBlockNumber
 * @param cpWriteData
 * @return unsigned char
 */
unsigned char ISO15693_Write(unsigned char Flags, unsigned char *UID, unsigned char cBlockNumber, unsigned char *cpWriteData)
{
    unsigned char cStatus;
    unsigned char i;
    unsigned char cIndex;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    g_sPcdTrsv.cData[0] = Flags;                       // Flag
    g_sPcdTrsv.cData[1] = ISO15693_WRITE_SINGLE_BLOCK; // Command
    cIndex              = 2;

    if (Flags & 0x20) { // flags & 0x20 - Adresflag request flags & 0x10 - Selectflag request
        memcpy(&g_sPcdTrsv.cData[cIndex], UID, 8);
        cIndex += 8;
    }

    g_sPcdTrsv.cData[cIndex++] = cBlockNumber;

    for (i = 0; i < 4; i++)
        g_sPcdTrsv.cData[cIndex++] = cpWriteData[i];

    g_sPcdTrsv.iLength = cIndex;

    SetTimeOut();

    if (Flags & 0x40)
        cStatus = PcdTransceive_CV663(PCD_TRANSMIT);
    else
        cStatus = PcdTransceive_CV663(PCD_TRANSCEIVE);

    if (cStatus == 0)
        cStatus = TI_Process(Flags);

    //	*cpReceiveLength = 0;

    return cStatus;
}

/**
 * @brief 锁定指定块
 *
 * @param Flags
 * @param UID
 * @param cBlockNumber
 * @return unsigned char
 */
unsigned char ISO15693_LockBlock(unsigned char Flags, unsigned char *UID, unsigned char cBlockNumber)
{
    unsigned char cStatus;
    //	unsigned char i;
    unsigned char cIndex;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    g_sPcdTrsv.cData[0] = Flags;               // Flag
    g_sPcdTrsv.cData[1] = ISO15693_LOCK_BLOCK; // Command
    cIndex              = 2;

    if (Flags & 0x20) // flags & 0x20 - Adresflag request flags & 0x10 - Selectflag request
    {
        memcpy(&g_sPcdTrsv.cData[cIndex], UID, 8);
        cIndex += 8;
    }

    g_sPcdTrsv.cData[cIndex] = cBlockNumber;
    cIndex += 1;
    g_sPcdTrsv.iLength = cIndex;

    SetTimeOut();

    if (Flags & 0x40)
        cStatus = PcdTransceive_CV663(PCD_TRANSMIT);
    else
        cStatus = PcdTransceive_CV663(PCD_TRANSCEIVE);

    if (cStatus == 0)
        cStatus = TI_Process(Flags);

    //	*cpReceiveLength = 0;

    return cStatus;
}

/**
 * @brief 重新准备
 *
 * @param Flags
 * @param UID
 * @return unsigned char
 */
unsigned char ISO15693_ResetToReady(unsigned char Flags, unsigned char *UID)
{
    unsigned char cStatus;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    //	unsigned char i;

    g_sPcdTrsv.iLength  = 10;
    g_sPcdTrsv.cData[0] = Flags;                   // Flag
    g_sPcdTrsv.cData[1] = ISO15693_RESET_TO_READY; // Command

    memcpy(&g_sPcdTrsv.cData[2], UID, 8);

    SetTimeOut();

    cStatus = PcdTransceive_CV663(PCD_TRANSCEIVE);

    if (cStatus == 0)
        if ((g_sPcdTrsv.cData[0] & 0x01) == 1)
            cStatus = 0xFF;

    return cStatus;
}

/**
 * @brief 写入AFI
 *
 * @param Flags
 * @param UID
 * @param cAFI
 * @return unsigned char
 */
unsigned char ISO15693_WriteAFI(unsigned char Flags, unsigned char *UID, unsigned char cAFI)
{
    unsigned char cStatus;
    //	unsigned char i;
    unsigned char cIndex;
    unsigned char cCmd;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    if (Flags & 0x40)
        cCmd = PCD_TRANSMIT;
    else
        cCmd = PCD_TRANSCEIVE;

    g_sPcdTrsv.cData[0] = Flags;              // Flag
    g_sPcdTrsv.cData[1] = ISO15693_WRITE_AFI; // Command
    cIndex              = 2;

    if (Flags & 0x20) // flags & 0x20 - Adresflag request flags & 0x10 - Selectflag request
    {
        memcpy(&g_sPcdTrsv.cData[cIndex], UID, 8);
        cIndex += 8;
    }

    g_sPcdTrsv.cData[cIndex] = cAFI;
    cIndex += 1;
    g_sPcdTrsv.iLength = cIndex;

    SetTimeOut();

    cStatus = PcdTransceive_CV663(cCmd);

    if (cStatus == 0)
        cStatus = TI_Process(Flags);

    //	*cpReceiveLength = 0;

    return cStatus;
}

/**
 * @brief 锁定AFI
 *
 * @param Flags
 * @param UID
 * @return unsigned char
 */
unsigned char ISO15693_LockAFI(unsigned char Flags, unsigned char *UID)
{
    unsigned char cStatus;
    //	unsigned char i;
    unsigned char cIndex;
    unsigned char cCmd;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    if (Flags & 0x40)
        cCmd = PCD_TRANSMIT;
    else
        cCmd = PCD_TRANSCEIVE;

    g_sPcdTrsv.cData[0] = Flags;             // Flag
    g_sPcdTrsv.cData[1] = ISO15693_LOCK_AFI; // Command
    cIndex              = 2;

    if (Flags & 0x20) // flags & 0x20 - Adresflag request flags & 0x10 - Selectflag request
    {
        memcpy(&g_sPcdTrsv.cData[cIndex], UID, 8);
        cIndex += 8;
    }

    g_sPcdTrsv.iLength = cIndex;

    SetTimeOut();

    cStatus = PcdTransceive_CV663(cCmd);

    if (cStatus == 0)
        cStatus = TI_Process(Flags);

    //	*cpReceiveLength = 0;

    return cStatus;
}

/**
 * @brief 写入DSFID
 *
 * @param Flags
 * @param UID
 * @param cDSFID
 * @return unsigned char
 */
unsigned char ISO15693_WriteDSFID(unsigned char Flags, unsigned char *UID, unsigned char cDSFID)
{
    unsigned char cStatus;
    //	unsigned char i;
    unsigned char cIndex;
    unsigned char cCmd;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    if (Flags & 0x40)
        cCmd = PCD_TRANSMIT;
    else
        cCmd = PCD_TRANSCEIVE;

    g_sPcdTrsv.cData[0] = Flags;                // Flag
    g_sPcdTrsv.cData[1] = ISO15693_WRITE_DSFID; // Command
    cIndex              = 2;

    if (Flags & 0x20) // flags & 0x20 - Adresflag request flags & 0x10 - Selectflag request
    {

        memcpy(&g_sPcdTrsv.cData[cIndex], UID, 8);
        cIndex += 8;
    }

    g_sPcdTrsv.cData[cIndex] = cDSFID;
    cIndex += 1;
    g_sPcdTrsv.iLength = cIndex;

    SetTimeOut();

    cStatus = PcdTransceive_CV663(cCmd);

    if (cStatus == 0)
        cStatus = TI_Process(Flags);

    //	*cpReceiveLength = 0;

    return cStatus;
}

/**
 * @brief 锁定DSFID
 *
 * @param Flags
 * @param UID
 * @return unsigned char
 */
unsigned char ISO15693_LockDSFID(unsigned char Flags, unsigned char *UID)
{
    unsigned char cStatus;
    //	unsigned char i;
    unsigned char cIndex;
    unsigned char cCmd;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    if (Flags & 0x40)
        cCmd = PCD_TRANSMIT;
    else
        cCmd = PCD_TRANSCEIVE;

    g_sPcdTrsv.cData[0] = Flags;               // Flag
    g_sPcdTrsv.cData[1] = ISO15693_LOCK_DSFID; // Command
    cIndex              = 2;

    if (Flags & 0x20) // flags & 0x20 - Adresflag request flags & 0x10 - Selectflag request
    {
        memcpy(&g_sPcdTrsv.cData[cIndex], UID, 8);
        cIndex += 8;
    }

    g_sPcdTrsv.iLength = cIndex;

    SetTimeOut();

    cStatus = PcdTransceive_CV663(cCmd);

    if (cStatus == 0)
        cStatus = TI_Process(Flags);

    //	*cpReceiveLength = 0;

    return cStatus;
}

/**
 * @brief 获取标签系统信息
 *
 * @param Flags
 * @param UID
 * @param cpReceiveLength
 * @param cpReceiveData
 * @return unsigned char
 */
unsigned char ISO15693_GetSystemInformation(unsigned char Flags, unsigned char *UID, unsigned char *cpReceiveLength, unsigned char *cpReceiveData)
{
    unsigned char cStatus;
    //	unsigned char i;
    unsigned char cIndex;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    g_sPcdTrsv.cData[0] = Flags;                          // Flag
    g_sPcdTrsv.cData[1] = ISO15693_GET_SYSTEM_INFOMATION; // Command
    g_sPcdTrsv.cData[2] = 0;
    cIndex              = 2;

    if ((Flags & 0x20)) // flags & 0x20 - Adresflag request flags & 0x10 - Selectflag request
    {
        memcpy(&g_sPcdTrsv.cData[cIndex], UID, 8);
        cIndex += 8;
    }

    g_sPcdTrsv.iLength = cIndex; // + 1;

    SetTimeOut();

    cStatus          = PcdTransceive_CV663(PCD_TRANSCEIVE);
    *cpReceiveLength = 0;

    if (cStatus == 0) {

        unsigned char j = (g_sPcdTrsv.iLength / 8) - 1;

        if ((g_sPcdTrsv.cData[0] & 0x01) == 1)
            cStatus = 0xFF;
        else {
            memcpy(cpReceiveData, &g_sPcdTrsv.cData[1], j);
            *cpReceiveLength = j;
        }
    }
    return cStatus;
}

/**
 * @brief 获取多块的安全状态
 *
 * @param Flags
 * @param UID
 * @param cBlockNumber
 * @param cNumberOfBlock
 * @param cpReceiveLength
 * @param cpReceiveData
 * @return unsigned char
 */
unsigned char ISO15693_GetBlockSecurity(unsigned char Flags, unsigned char *UID,
                                        unsigned char cBlockNumber, unsigned char cNumberOfBlock,
                                        unsigned char *cpReceiveLength, unsigned char *cpReceiveData)
{
    unsigned char cStatus;
    //	unsigned char i;
    unsigned char cIndex;

    //	cStatus = Set15693Mode();

    //	if (cStatus != 0)
    //		return cStatus;

    g_sPcdTrsv.cData[0] = Flags;                                       // Flag
    g_sPcdTrsv.cData[1] = ISO15693_GET_MULTIPLE_BLOCK_SECURITY_STATUS; // Command
    cIndex              = 2;

    if (Flags & 0x20) // flags & 0x20 - Adresflag request flags & 0x10 - Selectflag request
    {
        memcpy(&g_sPcdTrsv.cData[cIndex], UID, 8);
        cIndex += 8;
    }

    g_sPcdTrsv.cData[cIndex++] = cBlockNumber;
    g_sPcdTrsv.cData[cIndex++] = cNumberOfBlock;
    g_sPcdTrsv.iLength         = cIndex;

    SetTimeOut();

    cStatus          = PcdTransceive_CV663(PCD_TRANSCEIVE);
    *cpReceiveLength = 0;

    if (cStatus == 0) {
        unsigned char j = (g_sPcdTrsv.iLength / 8) - 1;
        if ((g_sPcdTrsv.cData[0] & 0x01) == 1)
            cStatus = 0xFF;
        else {
            memcpy(cpReceiveData, &g_sPcdTrsv.cData[1], j);
            *cpReceiveLength = j;
        }
    }

    return cStatus;
}

/**
 * @brief 标签自动识别
 *
 * @param cpTagSNR 标签UID缓存
 * @param cAFI
 * @param cAFI_Enable
 * @return unsigned char
 */
unsigned char ISO15693_MultiTagAutoInventory(unsigned char *cpTagSNR, unsigned char cAFI, unsigned char cAFI_Enable)
{
    unsigned char j, k;
    unsigned char cCollByte;
    unsigned char cCollBit;
    unsigned char cStatus;
    unsigned char cMaskLength = 0;
    unsigned char cReceiveLength;
    unsigned char cFlag;

    cFlag = (cAFI_Enable == 0) ? 0x26 : 0x36;
    j     = 0;

    do {
        j++;
        cStatus = ISO15693_Inventory(cFlag, cAFI, cMaskLength, cpTagSNR, &cReceiveLength, cpTagSNR);
    } while ((cStatus != MI_OK) && (cStatus != MI_COLLERR) && j < 4);

    if (cStatus == MI_COLLERR) { // 碰撞错误

        j = 0;

        do {
            j++;
            cMaskLength = Read_CV663(PHHAL_HW_CV663_REG_STATUS) - 0x10;

            memcpy(cpTagSNR, &g_sPcdTrsv.cData[2], 8);

            cCollByte = (cMaskLength - 1) / 8;
            cCollBit  = (cMaskLength - 1) % 8;

            cpTagSNR[cCollByte] |= (0x01 << cCollBit);

            k = 0;

            do {
                k++;
                cStatus = ISO15693_Inventory(cFlag, cAFI, cMaskLength, cpTagSNR, &cReceiveLength, cpTagSNR);
            } while ((cStatus != MI_OK) && (cStatus != MI_COLLERR) && k < 4);
        } while (cStatus == MI_COLLERR && j < 64);
    }

    return cStatus;
}

/**
 * @brief Set the Time Out object
 *
 */
void SetTimeOut(void)
{
    unsigned char cReload;

    switch (g_sPcdTrsv.cData[1]) {
        case ISO15693_STAY_QUIET:
            cReload = 0x04;
            break;
        case ISO15693_SELECT:
        case ISO15693_RESET_TO_READY:
            cReload = 0x0F;
            break;
        case ISO15693_LOCK_AFI:
        case ISO15693_LOCK_DSFID:
        case ISO15693_LOCK_BLOCK:
        case ISO15693_WRITE_SINGLE_BLOCK:
        // case ISO15693_WRITE_MULTIPLE_BLOCKS:
        case ISO15693_WRITE_AFI:
        case ISO15693_WRITE_DSFID:
            cReload = 0x29;
            break;
        case ISO15693_READ_SINGLE_BLOCK:
            cReload = 0x17;
            break;
        case ISO15693_INVENTORY:
            cReload = 0x1F;
            break;
        case ISO15693_GET_SYSTEM_INFOMATION:
            cReload = 0x25;
            break;
        case ISO15693_GET_MULTIPLE_BLOCK_SECURITY_STATUS:
            cReload = 0x40;
            break;
        case ISO15693_READ_MULTIPLE_BLOCK:
            cReload = 0x40;
            break;
        default:
            cReload = 0x86;
            break;
    }
    // T0理论值为0x0800，运行读块失败，改为0x0A00可以成功。
    // T0理论值为0x0800，运行StayQuiet不成功，所以修改为0x2A00。
    Write_CV663(PHHAL_HW_CV663_REG_T0RELOADHI, (unsigned char)(0x2A00 >> 8));  // 定义定时器 0 重载值的高字节。有待进一步计算时间
    Write_CV663(PHHAL_HW_CV663_REG_T0RELOADLO, (unsigned char)0x2A00);         // 定义定时器 0 重载值的低字节。
    Write_CV663(PHHAL_HW_CV663_REG_T1RELOADHI, (unsigned char)(cReload >> 8)); // 定义定时器 1 重载值的高字节。
    Write_CV663(PHHAL_HW_CV663_REG_T1RELOADLO, (unsigned char)cReload);        // 定义定时器 1 重载值的低字节。
    Write_CV663(PHHAL_HW_CV663_REG_TCONTROL, 0x33);                            // 用于定时器段的控制寄存器。
}
