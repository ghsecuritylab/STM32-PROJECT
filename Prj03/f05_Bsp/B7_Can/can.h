// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: CANͨ������
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mySys.h"
// ============================================================================
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	0		//0-��ʹ��;1-ʹ��.
// ============================================================================
typedef struct
{
    UINT32 dwMsgID;   //֡ID
    UINT32 dwIDE;     //��׼֡������չ֡
    UINT32 dwRTR;     //����֡����Զ��֡
    UINT32 dwDLC;     //���ݳ���
    UINT8* pbyMsg;    //���ݵ�ַ  
}tagCanTxMsg;
// ============================================================================
UINT8 CAN1_Mode_Init(void);
UINT8 CAN1_Send_Msg(tagCanTxMsg *ptCanTxMsg);				//��������
UINT8 CAN1_Receive_Msg(UINT8 *pbyBuf);						//��������
// ============================================================================
#ifdef __cplusplus
}
#endif


#endif//__CAN_H__
