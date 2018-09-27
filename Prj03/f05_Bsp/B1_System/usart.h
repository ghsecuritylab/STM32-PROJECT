// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: ���ڴ�ӡ������
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mySys.h"
#include "stdio.h"	

#define CN_BUFSIZE_RX               1       //�����С
#define CN_USART_RX_LEN  			200  	//�����������ֽ��� 200
#define CN_USART1_RX_EN 			1		//ʹ�ܣ�1��/��ֹ��0������1����

// ============================================================================
extern UINT8    USART_RX_BUF[CN_USART_RX_LEN];  //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern UINT8    aRxBuffer[CN_BUFSIZE_RX];       //HAL��USART����Buffer
extern UINT16   USART_RX_STA;         		    //����״̬���	
extern UART_HandleTypeDef UART1_Handler;        //UART���

// ============================================================================
extern void uart_init(UINT32 dwBound);

#ifdef __cplusplus
}
#endif

#endif//__USART_H__
