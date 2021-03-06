// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// 模块描述: 串口通信驱动
// 模块版本: V1.00
// 创建人员: GuXY
// 创建时间: 2017-04-18
// ============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// ============================================================================
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mySys.h"
#include "stdio.h"	

// ============================================================================
#define CN_USART1_RX_BUF_LEN  			   ( 256 )  	//定义最大接收字节数
#define CN_USART2_RX_BUF_LEN  			   ( 256 )  	//定义最大接收字节数
#define CN_USART3_RX_BUF_LEN  			   ( 256 )  	//定义最大接收字节数
// ============================================================================

// ============================================================================
extern void uart_init(UINT8 byUsartNum,UINT32 dwBound);
extern void Usart1_Send_Data(UINT8 *pbyBuf,UINT8 byLen);//串口1发送数据
extern void Usart1_Receive_Data(UINT8 *pbyBuf,UINT8 *pbyLen);//串口1接收数据 
// ============================================================================
#ifdef __cplusplus
}
#endif

#endif//__USART_H__

