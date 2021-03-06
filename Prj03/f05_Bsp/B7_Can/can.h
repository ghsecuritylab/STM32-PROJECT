// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// 模块描述: CAN通信驱动
// 模块版本: V1.00
// 创建人员: GuXY
// 创建时间: 2017-04-18
// ============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// ============================================================================
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mySys.h"
// ============================================================================
//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	0		//0-不使能;1-使能.
// ============================================================================
typedef struct
{
    UINT32 dwMsgID;   //帧ID
    UINT32 dwIDE;     //标准帧还是扩展帧
    UINT32 dwRTR;     //数据帧还是远程帧
    UINT32 dwDLC;     //数据长度
    UINT8* pbyMsg;    //数据地址  
}tagCanTxMsg;
// ============================================================================
UINT8 CAN1_Mode_Init(void);
UINT8 CAN1_Send_Msg(tagCanTxMsg *ptCanTxMsg);				//发送数据
UINT8 CAN1_Receive_Msg(UINT8 *pbyBuf);						//接收数据
// ============================================================================
#ifdef __cplusplus
}
#endif


#endif//__CAN_H__

