// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: RTC���
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// ============================================================================
#ifndef  _USER_RTC_H_
#define  _USER_RTC_H_

#ifdef __cplusplus
extern "C" {
#endif
// ============================================================================
#include "mySys.h"

// ============================================================================
extern RTC_HandleTypeDef RTC_Handler;  //RTC���

// ============================================================================   
extern UINT8  Rtc_Init(void);
extern HAL_StatusTypeDef RTC_Set_Time(UINT8 byHour,UINT8 byMin,UINT8 bySec,UINT8 byTimeFormat);
extern HAL_StatusTypeDef RTC_Set_Date(UINT8 byYear,UINT8 byMonth,UINT8 byDate,UINT8 byWeek);

// ============================================================================

#ifdef __cplusplus
}
#endif

#endif // _USER_RTC_H_

