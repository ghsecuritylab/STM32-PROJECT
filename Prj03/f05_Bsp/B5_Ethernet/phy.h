// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: ethernetͨ��PHY����
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#ifndef __PHY_H__
#define __PHY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mySys.h"

// ============================================================================
extern UINT8 *g_pbyRxBuf; 							//��̫���ײ���������buffersָ�� 
extern UINT8 *g_pbyTxBuf; 							//��̫���ײ���������buffersָ��
extern ETH_HandleTypeDef    ETH_Handler;            //��̫�����
extern ETH_DMADescTypeDef  *DMARxDscrTab;			//��̫��DMA�������������ݽṹ��ָ��
extern ETH_DMADescTypeDef  *DMATxDscrTab;			//��̫��DMA�������������ݽṹ��ָ�� 
extern ETH_DMADescTypeDef  *DMATxDescToSet;			//DMA����������׷��ָ��
extern ETH_DMADescTypeDef  *DMARxDescToGet; 		//DMA����������׷��ָ�� 
// ============================================================================
UINT8   PHY_Init(void);
UINT8   PHY_Get_Speed(void);
UINT32  PHY_ReadPHY(UINT16 wReg);
void    PHY_WritePHY(UINT16 wReg,UINT16 wValue);
UINT8   ETH_MACDMA_Config(void);
UINT8   ETH_Mem_Malloc(void);
void    ETH_Mem_Free(void);
UINT32  ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDesc);
// ============================================================================

#ifdef __cplusplus
}
#endif

#endif//__PHY_H__

