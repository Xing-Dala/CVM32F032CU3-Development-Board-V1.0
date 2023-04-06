/**
 * @file ISO14443.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-11-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef ISO14443_H
#define ISO14443_H

// ISO14443-3 Command
#define PICC_REQIDL    0x26 // 寻天线区内未进入休眠状态的卡
#define PICC_REQALL    0x52 // 寻天线区内全部卡
#define PICC_ANTICOLL1 0x93 // 一级防冲撞
#define PICC_ANTICOLL2 0x95 // 二级防冲撞
#define PICC_ANTICOLL3 0x97 // 三级防冲撞
#define PICC_HALT      0x50 // 停止

// ISO14443-4 Command
#define PICC_RESET 0xE0 // 复位功能
#define PICC_PPSS  0xD0 //

// MFIRE卡命令字定义
#define PICC_AUTHENT1A 0x60 // 验证A密钥
#define PICC_AUTHENT1B 0x61 // 验证B密钥
#define PICC_READ      0x30 // 读块
#define PICC_WRITE     0xA0 // 写块
#define PICC_DECREMENT 0xC0 // 扣款
#define PICC_INCREMENT 0xC1 // 充值
#define PICC_RESTORE   0xC2 // 调钱包到缓冲区
#define PICC_TRANSFER  0xB0 // 保存缓冲区中数据

// Mifare密钥来源定义
#define KEY_CURRENT 0x01
#define KEY_E2PROM  0x02

// Type Define
typedef struct
{
    unsigned char cUID[11];
    unsigned char cATQ[2];
    unsigned char cSAK;
    unsigned char cLen;
} tpd_Card;

// Extern Functions Define
// ISO14443-3 Type A functions define
void PiccReset(void);
unsigned char PiccRequest(unsigned char cReq, unsigned char *cATQA);                             // 寻卡
unsigned char PiccAnticoll(unsigned char cLev, unsigned char *cUID);                             // 防冲撞
unsigned char PiccSelect(unsigned char cLev, unsigned char *cUID, unsigned char *cSAK);          // 选定一张卡
unsigned char PiccHaltA(void);                                                                   // 卡休眠
unsigned char PiccAnticollSelect(unsigned char *cLen, unsigned char *cUID, unsigned char *cSAK); // 三级防碰撞

// ISO14443-3 Type B functions define
unsigned char PiccRequestB(unsigned char cReq, unsigned char AFI, unsigned char N, unsigned char *cpATQB);
unsigned char PiccAttrib(unsigned char *PUPI, unsigned char PARAM3, unsigned char cFSDI, unsigned char *answer);
unsigned char PiccHaltB(unsigned char *PUPI);

// ISO14443-4 Type A&B CPU card functions define
// ISO14443A
unsigned char PiccTypeA_Rst(unsigned char *cpLen, unsigned char *cpData);
unsigned char ISO14443A_Rst(unsigned char *cpLen, unsigned char *cpRATS);
// ISO14443B
unsigned char ISO14443B_Rst(unsigned char cReq, unsigned char *pLen, unsigned char *pATQB);
// ISO14443-4 COS
unsigned char ISO14443_COS(unsigned short *pLen, unsigned char *pAPDU);

void PiccWaitWTX(unsigned char cTimeOut, unsigned char cWTX); // CPU卡操作时间函数

void PiccCaluINF(unsigned char cFSCI);

// Mifare S50/S70 card functions define
uint8_t PiccAuthState(uint8_t cSector, uint8_t cMode, uint8_t *key); // 验证密钥
uint8_t PiccRead(uint8_t cSector, uint8_t CBlock, uint8_t *Data);    // 读块
uint8_t PiccWrite(uint8_t cSector, uint8_t CBlock, uint8_t *Data);   // 写块

unsigned char PiccValue(unsigned char cFct, unsigned char cBlk, unsigned char *cValue); // 充值扣款
unsigned char PiccRestore(unsigned char cBlock);                                        // 备份
unsigned char PiccTransfer(unsigned char cBlock);                                       // 操作

#endif
