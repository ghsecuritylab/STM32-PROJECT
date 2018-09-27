// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: SD������(����SDIO)
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#ifndef __SDCARD_SDIO_H__
#define __SDCARD_SDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mySys.h"
// ============================================================================
#define SD_TIMEOUT      ((UINT32)100000000)         //��ʱʱ��
#define SD_DMA_MODE    	( 0 )		                //1��DMAģʽ��0����ѯģʽ   
#define CN_SDIO_DATABUF_LEN  (512)                  //SDIO�ӿ�ͨ������buf��С
// ============================================================================
extern SD_HandleTypeDef        SDCARD_Handler;     //SD�����
extern HAL_SD_CardInfoTypedef  SDCardInfo;         //SD����Ϣ�ṹ��
// ============================================================================
UINT8 SD_Init(void);
UINT8 SD_GetCardInfo(HAL_SD_CardInfoTypedef *cardinfo);
UINT8 SD_ReadDisk(UINT8* pbyBuf,UINT32 dwSector,UINT32 dwCnt);
UINT8 SD_WriteDisk(UINT8 *pbyBuf,UINT32 dwSector,UINT32 dwCnt);
UINT8 SD_ReadBlocks_DMA(UINT32 *pdwBuf,UINT64 uSector,UINT32 dwBlockSize,UINT32 dwCnt);
UINT8 SD_WriteBlocks_DMA(UINT32 *pdwBuf,UINT64 uSector,UINT32 dwBlockSize,UINT32 dwCnt);
// ============================================================================
#ifdef __cplusplus
}
#endif
#endif//__SDCARD_SDIO_H__

