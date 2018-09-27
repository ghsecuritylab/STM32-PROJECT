// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: IICʵ�����
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#ifndef __IIC_H__
#define __IIC_H__

#include "mySys.h"

// ============================================================================
//�ӿں���
extern void  IIC1_Init(void);			
extern void  IIC1_Write_Byte(UINT8 byAddr, UINT8 byReg, UINT8 byValue);  
extern UINT8 IIC1_Read_Byte(UINT8 byAddr, UINT8 byReg);
extern void  IIC1_Write_Bytes(UINT8 byAddr, UINT8 byReg, UINT16 wMemAdr,UINT8 *pbyBuf,UINT16 wLen);
extern void  IIC1_Read_Bytes(UINT8 byAddr, UINT8 byReg,UINT16 wMemAdr,UINT8 *pbyBuf,UINT16 wLen);    
// ============================================================================

#endif  //__IIC_H__

