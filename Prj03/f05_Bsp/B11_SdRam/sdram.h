// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: SDRAM����
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#ifndef __SDRAM_H__
#define __SDRAM_H__

#include "mySys.h"

extern SDRAM_HandleTypeDef SDRAM_Handler;//SDRAM���
#define Bank5_SDRAM_ADDR    ((UINT32)(0XC0000000)) //SDRAM��ʼ��ַ

//SDRAM���ò���
#define SDRAM_MODEREG_BURST_LENGTH_1             ((UINT16)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((UINT16)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((UINT16)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((UINT16)0x0004)

#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((UINT16)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((UINT16)0x0008)

#define SDRAM_MODEREG_CAS_LATENCY_2              ((UINT16)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((UINT16)0x0030)

#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((UINT16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((UINT16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((UINT16)0x0200)

// ============================================================================
extern void SDRAM_Init(void);//SDRAM��ʼ��
extern void FMC_SDRAM_WrBuf(UINT8 *pbyBuf,UINT32 dwWriteAdr,UINT32 dwNum);//SDRAMд
extern void FMC_SDRAM_RdBuf(UINT8 *pbyBuf,UINT32 dwReadAdr ,UINT32 dwNum);//SDRAM��
// ============================================================================
#endif  //__SDRAM_H__

