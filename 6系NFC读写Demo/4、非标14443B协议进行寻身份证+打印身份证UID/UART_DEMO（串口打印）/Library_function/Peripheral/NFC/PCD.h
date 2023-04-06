/*******************************************************************************
 * FileName: PCD.h
 *   Author: Layne Wen
 *     Date: 2017.02.22
 *  COMMENT: 1, ͷ�ļ����� RC500.H, RC52X.H, CV663.H
 *			 2, �����ض���,��������
 *			 3, ������붨��
 ******************************************************************************/
#ifndef _PCD_H_
#define _PCD_H_

#include "main.h"

#define _PCD663_


//------------------------------------------------------------------------------
// 1,ͷ�ļ�����
// 2,�����ض���
//------------------------------------------------------------------------------
#define PCD_CMD_LOADKEY				PHHAL_HW_CV663_CMD_LOADKEY
#define PCD_CMD_MFAUTHENT			PHHAL_HW_CV663_CMD_MFAUTHENT
#define PCD_CMD_RECEIVE				PHHAL_HW_CV663_CMD_RECEIVE
#define PCD_CMD_TRANSMIT			PHHAL_HW_CV663_CMD_TRANSMIT
#define PCD_CMD_TRANSCEIVE			PHHAL_HW_CV663_CMD_TRANSCEIVE


//------------------------------------------------------------------------------
// 3,������붨��
//------------------------------------------------------------------------------
#define MI_OK                 			0x00

#define MI_NOTAGERR						0xFF		// 0xFF �ޱ�ǩ������
#define MI_CRCERR						0xFE
#define MI_EMPTY						0xFD
#define MI_AUTHERR						0xFC		// ��֤ʧ��
#define MI_PARITYERR					0xFB
#define MI_CODEERR						0xFA		// �������
#define MI_SERNRERR						0xF8		// Sernr error 		β�˴���
#define MI_KEYERR						0xF7
#define MI_NOTAUTHERR					0xF6		// û����֤����
#define MI_BITCOUNTERR					0xF5
#define MI_BYTECOUNTERR					0xF4		// Byte counter error	�ֽ�������
#define MI_IDLE							0xF3
#define MI_TRANSERR						0xF2		// �������
#define MI_WRITEERR						0xF1
#define MI_INCRERR						0xF0
#define MI_DECRERR						0xEF
#define MI_READERR						0xEE
#define MI_OVFLERR						0xED
#define MI_POLLING						0xEC
#define MI_FRAMINGERR					0xEB
#define MI_ACCESSERR					0xEA
#define MI_UNKNOWN_COMMAND				0xE9		// δ֪����
#define MI_COLLERR						0xE8		// Collision error	��ײ����
#define MI_RESETERR						0xE7		// Reset error		��λ����
#define MI_INTERFACEERR					0xE6		// Interface error	�ӿڴ���
#define MI_ACCESSTIMEOUT				0xE5
#define MI_NOBITWISEANTICOLL			0xE4		// No bit wise anticoll
#define MI_QUIT							0xE2
#define MI_RECBUF_OVERFLOW				0xCE
#define MI_SENDBYTENR					0xCD
#define MI_SENDBUF_OVERFLOW				0xCB
#define MI_BAUDRATE_NOT_SUPPORTED		0xCA
#define MI_SAME_BAUDRATE_REQUIRED		0xC9
#define MI_WRONG_PARAMETER_VALUE		0xC4
#define MI_BREAK						0x9D
#define MI_NY_IMPLEMENTED				0x9C
#define MI_NO_MFRC						0x9B
#define MI_MFRC_NOTAUTH					0x9A
#define MI_WRONG_DES_MODE				0x99
#define MI_HOST_AUTH_FAILED				0x98
#define MI_WRONG_LOAD_MODE				0x96
#define MI_WRONG_DESKEY					0x95
#define MI_MKLOAD_FAILED				0x94
#define MI_FIFOERR						0x93
#define MI_WRONG_ADDR					0x92
#define MI_DESKEYLOAD_FAILED			0x91
#define MI_WRONG_SEL_CNT				0x8E
#define MI_WRONG_TEST_MODE				0x8B
#define MI_TEST_FAILED					0x8A
#define MI_TOC_ERROR					0x89
#define MI_COMM_ABORT					0x88
#define MI_INVALID_BASE					0x87
#define MI_MFRC_RESET					0x86
#define MI_WRONG_VALUE					0x85
#define MI_VALERR						0x84		// 0x84 Ǯ������
#define MI_COM_ERR                      0x83
#define MI_FATAL_ERR                    0x82
#define MI_CONTINUE_ERR                 0x81
#define MI_RESIDUAL_BIT_ERR             0x80






//------------------------------------------------------------------------------
// Type Define
//------------------------------------------------------------------------------
typedef struct
{
	unsigned char cType;
	unsigned char cPCB;				// Block
	//unsigned char cFSC;			// 
	unsigned char cFSD;				// 
	unsigned char cFWI;				// Frame wait intager
	unsigned char cCID;				// 0x80:present; 0x00:absent;
	unsigned char cNAD;				// 0x40:present; 0x00:absent;
	unsigned char cMaxINF;			// ����FSI CID NAD�����INF��󳤶�
} tpd_pcdpara;						// Э�����


typedef struct
{
	unsigned char  iLength;
	unsigned char  cData[512];
} tpd_pcdtrsv;





//------------------------------------------------------------------------------
// Port Define
//------------------------------------------------------------------------------
#define _WATER_LEVEL_			0x30		// ��RC5xxϵ�ж���оƬ�е� WATER LEVEL, �������ͺͶ�ȡFIFO���ֽ���


#define PCD_TYPE_14443A			0x1A
#define PCD_TYPE_14443B			0x1B

//------------------------------------------------------------------------------
// Pcd Functions Define
//------------------------------------------------------------------------------
unsigned char ReadRawRC(unsigned char cAddress);
void WriteRawRC(unsigned char cAddress, unsigned char cValue);
void SetBitMask(unsigned char cAddr, unsigned char cMask);
void ClearBitMask(unsigned char cAddr, unsigned char cMask);

unsigned char PcdReset(void);
unsigned char PcdConfig(unsigned char cType);
unsigned char PcdTransceive(unsigned char cCommand);

void PcdSetTmo(unsigned char tmoLength);
void PcdSetFSD(unsigned char cFSDI);

void PcdAntenna(unsigned char cSta);

void PcdIdle(void);



#endif
