/**
 * @file CV520.h
 * @author Yikui Zhang (xingdala@qq.com)
 * @brief
 * @version 1.0
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __CV520_H__
#define __CV520_H__

#include "stdint.h"
#include "RC52x.h"
#include "bsp_gpio.h"

/**
 * @brief SPI communication pin definition
 *
 */
#define CV520_CLK_Pin 5
#define CV520_CS_Pin 6
#define CV520_MOSI_Pin 21
#define CV520_MISO_Pin 1
#define CV520_IRQ_Pin 7
#define CV520_RST_Pin 4


/**
 * @brief Analog SPI pin high level
 *
 */
#define CV520_MOSI_Hight gpio_output_high(CV520_MOSI_Pin)
#define CV520_MISO_Hight gpio_output_high(CV520_MISO_Pin)
#define CV520_CS_Hight gpio_output_high(CV520_CS_Pin)
#define CV520_CLK_Hight gpio_output_high(CV520_CLK_Pin)
#define CV520_RST_Hight gpio_output_high(CV520_RST_Pin)

/**
 * @brief Analog SPI pin low level
 *
 */
#define CV520_MOSI_Low gpio_output_low(CV520_MOSI_Pin)
#define CV520_MISO_Low gpio_output_low(CV520_MISO_Pin)
#define CV520_CS_Low gpio_output_low(CV520_CS_Pin)
#define CV520_CLK_Low gpio_output_low(CV520_CLK_Pin)
#define CV520_RST_Low gpio_output_low(CV520_RST_Pin)

/**
 * @brief Analog SPI MISO Read
 *
 */
#define CV520_MISO_Read IO_READ(CV520_MISO_Pin)

#define PCD_CMD_MFAUTHENT JCMD_AUTHENT
#define PCD_CMD_RECEIVE JCMD_RECEIVE
#define PCD_CMD_TRANSMIT JCMD_TRANSMIT
#define PCD_CMD_TRANSCEIVE JCMD_TRANSCEIVE

/**
 * @brief CV520 instruction
 *
 */
#define MI_OK 0x00 // No error
#define MI_NOTAGERR 0xFF
#define MI_CRCERR 0xFE
#define MI_EMPTY 0xFD
#define MI_AUTHERR 0xFC
#define MI_PARITYERR 0xFB
#define MI_CODEERR 0xFA
#define MI_SERNRERR 0xF8 // Sernr error
#define MI_KEYERR 0xF7
#define MI_NOTAUTHERR 0xF6
#define MI_BITCOUNTERR 0xF5
#define MI_BYTECOUNTERR 0xF4 // Byte counter error
#define MI_IDLE 0xF3
#define MI_TRANSERR 0xF2
#define MI_WRITEERR 0xF1
#define MI_INCRERR 0xF0
#define MI_DECRERR 0xEF
#define MI_READERR 0xEE
#define MI_OVFLERR 0xED
#define MI_POLLING 0xEC
#define MI_FRAMINGERR 0xEB
#define MI_ACCESSERR 0xEA
#define MI_UNKNOWN_COMMAND 0xE9
#define MI_COLLERR 0xE8      // Collision error
#define MI_RESETERR 0xE7     // Reset error
#define MI_INTERFACEERR 0xE6 // Interface error
#define MI_ACCESSTIMEOUT 0xE5
#define MI_NOBITWISEANTICOLL 0xE4 // No bit wise anticoll
#define MI_QUIT 0xE2
#define MI_RECBUF_OVERFLOW 0xCE
#define MI_SENDBYTENR 0xCD
#define MI_SENDBUF_OVERFLOW 0xCB
#define MI_BAUDRATE_NOT_SUPPORTED 0xCA
#define MI_SAME_BAUDRATE_REQUIRED 0xC9
#define MI_WRONG_PARAMETER_VALUE 0xC4
#define MI_BREAK 0x9D
#define MI_NY_IMPLEMENTED 0x9C
#define MI_NO_MFRC 0x9B
#define MI_MFRC_NOTAUTH 0x9A
#define MI_WRONG_DES_MODE 0x99
#define MI_HOST_AUTH_FAILED 0x98
#define MI_WRONG_LOAD_MODE 0x96
#define MI_WRONG_DESKEY 0x95
#define MI_MKLOAD_FAILED 0x94
#define MI_FIFOERR 0x93
#define MI_WRONG_ADDR 0x92
#define MI_DESKEYLOAD_FAILED 0x91
#define MI_WRONG_SEL_CNT 0x8E
#define MI_WRONG_TEST_MODE 0x8B
#define MI_TEST_FAILED 0x8A
#define MI_TOC_ERROR 0x89
#define MI_COMM_ABORT 0x88
#define MI_INVALID_BASE 0x87
#define MI_MFRC_RESET 0x86
#define MI_WRONG_VALUE 0x85
#define MI_VALERR 0x84
#define MI_COM_ERR 0x83
#define MI_FATAL_ERR 0x82
#define MI_CONTINUE_ERR 0x81
#define MI_RESIDUAL_BIT_ERR 0x80

/**
 * @brief Type Define
 *
 */
typedef struct
{
    uint8_t cType;
    uint8_t cPCB; // Block
    // uint8_t cFSC;
    uint8_t cFSD;
    uint8_t cFWI; // Frame wait intager
    uint8_t cCID; // 0x80:present; 0x00:absent;
    uint8_t cNAD; // 0x40:present; 0x00:absent;
    uint8_t cMaxINF;
} tpd_pcdpara;

typedef struct
{
    uint8_t iLength;
    uint8_t cData[512];
} tpd_pcdtrsv;

/**
 * @brief Port Define
 *
 */
#define _WATER_LEVEL_ 0x30

#define PCD_TYPE_14443A 0x1A
#define PCD_TYPE_14443B 0x1B

void CV520_SPI_Init(void);
void CV520_Init(void);

uint8_t PcdReset_CV520(void);
uint8_t Read_CV520(uint8_t Address);
void Write_CV520(uint8_t Address, uint8_t Data);
void SetBitMask_CV520(uint8_t cAddr, uint8_t cMask);
void ClearBitMask(uint8_t cAddr, uint8_t cMask);
uint8_t PcdConfig(uint8_t cType);
uint8_t PcdTransceive(uint8_t cCommand);
void PcdSetTmo_CV520(uint8_t cTmo);
void PcdSetFSD_CV520(uint8_t cFSDI);
void PcdAntenna_CV520(uint8_t cSta);

void CV520_Read_Card(void);

#endif
