/**
 * @file ISO14443.h
 * @author Yikui Zhang (xingdala@qq.com)
 * @brief
 * @version 0.1
 * @date 2022-10-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef ISO14443_H
#define ISO14443_H

#include "stdint.h"

// ISO14443-3 Command
#define PICC_REQIDL 0x26
#define PICC_REQALL 0x52
#define PICC_ANTICOLL1 0x93
#define PICC_ANTICOLL2 0x95
#define PICC_ANTICOLL3 0x97
#define PICC_HALT 0x50

// ISO14443-4 Command
#define PICC_RESET 0xE0
#define PICC_PPSS 0xD0

#define PICC_AUTHENT1A 0x60
#define PICC_AUTHENT1B 0x61
#define PICC_READ 0x30
#define PICC_WRITE 0xA0
#define PICC_DECREMENT 0xC0
#define PICC_INCREMENT 0xC1
#define PICC_RESTORE 0xC2
#define PICC_TRANSFER 0xB0

#define KEY_CURRENT 0x01
#define KEY_E2PROM 0x02

// Type Define
typedef struct
{
    uint8_t cUID[11];
    uint8_t cATQ[2];
    uint8_t cSAK;
    uint8_t cLen;
} tpd_Card;

// Extern Functions Define
// ISO14443-3 Type A functions define
void PiccReset(void);
uint8_t PiccRequest(uint8_t cReq, uint8_t *cATQA);
uint8_t PiccAnticoll(uint8_t cLev, uint8_t *cUID);
uint8_t PiccSelect(uint8_t cLev, uint8_t *cUID, uint8_t *cSAK);
uint8_t PiccHaltA(void);
uint8_t PiccAnticollSelect(uint8_t *cLen, uint8_t *cUID, uint8_t *cSAK);

// ISO14443-4 Type A CPU card functions define
uint8_t PiccTypeA_Rst(uint8_t *cpLen, uint8_t *cpData);

uint8_t ISO14443A_Rst(uint8_t *cpLen, uint8_t *cpRATS);

uint8_t ISO14443_COS(uint16_t *pLen, uint8_t *pAPDU);

// Mifare S50/S70 card functions define
uint8_t PiccAuthState(uint8_t cSector, uint8_t cMode, uint8_t *key);

uint8_t PiccRead(uint8_t cSector, uint8_t CBlock, uint8_t *Data);
uint8_t PiccWrite(uint8_t cSector, uint8_t CBlock, uint8_t *Data);
uint8_t PiccValue(uint8_t cFct, uint8_t cBlk, uint8_t *cValue);
uint8_t PiccRestore(uint8_t cBlk);
uint8_t PiccTransfer(uint8_t cBlk);

#endif
