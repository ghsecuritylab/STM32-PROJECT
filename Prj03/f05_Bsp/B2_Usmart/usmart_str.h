// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: usmart_str.h
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#ifndef __USMART_STR_H
#define __USMART_STR_H	 

#ifdef __cplusplus
extern "C" {
#endif

#include "mySys.h"
  
UINT8  usmart_get_parmpos(UINT8 byNum);						//�õ�ĳ�������ڲ������������ʼλ��
UINT8  usmart_strcmp(UINT8 *pbyStr1,UINT8 *pbyStr2);					//�Ա������ַ����Ƿ����
UINT8  usmart_str2num(UINT8 *pbyStr,UINT32 *pdwRes);					//�ַ���תΪ����
UINT8  usmart_get_cmdname(UINT8 *pbyStr,UINT8 *pbyCmdName,UINT8 *pbyNLen,UINT8 byMaxLen);//��str�еõ�ָ����,������ָ���
UINT8  usmart_get_fname(UINT8 *pbyStr,UINT8 *pbyFName,UINT8 *pbyNum,UINT8 *pbyRVal);		//��str�еõ�������
UINT8  usmart_get_aparm(UINT8 *pbyStr,UINT8 *pbyFParm,UINT8 *pbyType); 	//��str�еõ�һ����������
UINT8  usmart_get_fparam(UINT8*pbyStr,UINT8 *pbyArn);  			//�õ�str�����еĺ�������.
UINT32 usmart_pow(UINT8 byM,UINT8 byN);							//M^N�η�


#ifdef __cplusplus
}
#endif


#endif//__USMART_STR_H

