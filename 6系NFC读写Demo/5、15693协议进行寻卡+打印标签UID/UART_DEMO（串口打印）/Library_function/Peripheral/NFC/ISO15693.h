/**
 * @file ISO15693.h
 * @author zhangyikui
 * @brief
 * @version 0.1
 * @date 2023-03-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __ISO15693_H_
#define __ISO15693_H_

/**
 * 15693 卡命令字
 */
#define ISO15693_INVENTORY                          0x01 // 清查命令(查找所有在RF场中的标签VICC)
#define ISO15693_STAY_QUIET                         0x02 // 保持安静命令(指定某个标签不再响应清查命令)
#define ISO15693_READ_SINGLE_BLOCK                  0x20 // 读取标签指定单块内容信息
#define ISO15693_WRITE_SINGLE_BLOCK                 0x21 // 将指定信息写入标签指定单块位置
#define ISO15693_LOCK_BLOCK                         0x22 // 将标签指定块进行锁定
#define ISO15693_READ_MULTIPLE_BLOCK                0x23 // 读取标签指定多块的信息
#define ISO15693_WRITE_MULTIPLE_BLOCK               0x24 // 将信息写入到标签多块位置
#define ISO15693_SELECT                             0x25 // 选择某一个标签(选中被发送了保持安静命令的标签)
#define ISO15693_RESET_TO_READY                     0x26 // 重新准备(重置被发送了保持安静命令的标签)
#define ISO15693_WRITE_AFI                          0x27 // 写入AFI（Application Family Identifier）
#define ISO15693_LOCK_AFI                           0x28 // 锁定AFI
#define ISO15693_WRITE_DSFID                        0x29 // 写入DSFID(Data Storage Format Identifier)
#define ISO15693_LOCK_DSFID                         0x2A // 锁定DSFID
#define ISO15693_GET_SYSTEM_INFOMATION              0x2B // 获取标签系统信息
#define ISO15693_GET_MULTIPLE_BLOCK_SECURITY_STATUS 0x2C // 获取多块的安全状态

unsigned char ISO15693_Inventory(unsigned char Flags, unsigned char AFI, unsigned char Mask_Length, unsigned char *Mask, unsigned char *RecLength, unsigned char *RecData);
unsigned char ISO15693_StayQuiet(unsigned char Flags, unsigned char *UID);
unsigned char ISO15693_Select(unsigned char Flags, unsigned char *UID);
unsigned char ISO15693_ResetToReady(unsigned char Flags, unsigned char *UID);
unsigned char ISO15693_ReadBlock(unsigned char Flags, unsigned char *UID, unsigned char cBlockNumber, unsigned char cNumberOfBlock, unsigned char *cpReceiveLength, unsigned char *cpReceiveData);
unsigned char ISO15693_Write(unsigned char Flags, unsigned char *UID, unsigned char cBlockNumber, unsigned char *cpWriteData);
unsigned char ISO15693_LockBlock(unsigned char Flags, unsigned char *UID, unsigned char cBlockNumber);
unsigned char ISO15693_WriteAFI(unsigned char Flags, unsigned char *UID, unsigned char cAFI);
unsigned char ISO15693_LockAFI(unsigned char Flags, unsigned char *UID);
unsigned char ISO15693_WriteDSFID(unsigned char Flags, unsigned char *UID, unsigned char cDSFID);
unsigned char ISO15693_LockDSFID(unsigned char Flags, unsigned char *UID);
unsigned char ISO15693_GetBlockSecurity(unsigned char Flags, unsigned char *UID, unsigned char cBlockNumber, unsigned char cNumberOfBlock, unsigned char *cpReceiveLength, unsigned char *cpReceiveData);
unsigned char ISO15693_GetSystemInformation(unsigned char Flags, unsigned char *UID, unsigned char *cpReceiveLength, unsigned char *cpReceiveData);
unsigned char ISO15693_MultiTagInventory(unsigned char cAFI_Enable, unsigned char cAFI, unsigned char *cpReceiveBytes, unsigned char *cpTagSNR);
unsigned char ISO15693_MultiTagAutoInventory(unsigned char *cpTagSNR, unsigned char cAFI, unsigned char cAFI_Enable);

void SetTimeOut(void);

#endif
