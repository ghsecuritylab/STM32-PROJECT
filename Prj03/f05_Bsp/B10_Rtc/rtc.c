
// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: RTC�������
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
#include "rtc.h" 

// ============================================================================
RTC_HandleTypeDef RTC_Handler;  //RTC���
// ============================================================================
//��������:RTCʱ������
//�������:ʱ,��,��,byTimeFormat:RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
//���ز���:SUCEE(1)-�ɹ�,ERROR(0)-ʧ�� 
// ============================================================================
HAL_StatusTypeDef RTC_Set_Time(UINT8 byHour,UINT8 byMin,UINT8 bySec,UINT8 byTimeFormat)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_TimeStructure.Hours          = byHour;
	RTC_TimeStructure.Minutes        = byMin;
	RTC_TimeStructure.Seconds        = bySec;
	RTC_TimeStructure.TimeFormat     = byTimeFormat;
	RTC_TimeStructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation = RTC_STOREOPERATION_RESET;
    
	return HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);	
}
// ============================================================================
//��������:RTC��������
//�������:��(0~99),��(1~12),��(0~31)week:����(1~7,0,�Ƿ�!)
//���ز���:SUCEE(1)-�ɹ�,ERROR(0)-ʧ�� 
// ============================================================================
HAL_StatusTypeDef RTC_Set_Date(UINT8 byYear,UINT8 byMonth,UINT8 byDate,UINT8 byWeek)
{
	RTC_DateTypeDef RTC_DateStructure;
    
	RTC_DateStructure.Year    = byYear;
	RTC_DateStructure.Month   = byMonth;
	RTC_DateStructure.Date    = byDate;
	RTC_DateStructure.WeekDay = byWeek;
    
	return HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
}
// ============================================================================
//��������:RTC��ʼ������
//�������:��
//���ز���:0-�ɹ�;2-ʧ��
// ============================================================================
UINT8 Rtc_Init(void)
{      
	RTC_Handler.Instance            = RTC;
    RTC_Handler.Init.HourFormat     = RTC_HOURFORMAT_24;//RTC����Ϊ24Сʱ��ʽ 
    RTC_Handler.Init.AsynchPrediv   = 0X7F;             //RTC�첽��Ƶϵ��(1~0X7F)
    RTC_Handler.Init.SynchPrediv    = 0XFF;             //RTCͬ����Ƶϵ��(0~7FFF)   
    RTC_Handler.Init.OutPut         = RTC_OUTPUT_DISABLE;     
    RTC_Handler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RTC_Handler.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
    
    if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK)
    {
        return 2;
    }
      
    if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0)!=0X5050)    //�Ƿ��һ������
    { 
        RTC_Set_Time(14,45,56,RTC_HOURFORMAT12_PM);	            //����ʱ�� 
		RTC_Set_Date(18,3,9,5);		                            //��������
        HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5050);   //����Ѿ���ʼ������
    }
    return 0;
}
// ============================================================================
//��������:RTCʱ������
//�������:RTC���
//���ز���:��
// ============================================================================
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();//ʹ�ܵ�Դʱ��PWR
	HAL_PWR_EnableBkUpAccess();//ȡ����������д����
    
    RCC_OscInitStruct.LSEState       = RCC_LSE_ON;            //RTCʹ��LSE
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;//LSE����
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;   //����ΪRTC
    PeriphClkInitStruct.RTCClockSelection    = RCC_RTCCLKSOURCE_LSE;//RTCʱ��ԴΪLSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        
    __HAL_RCC_RTC_ENABLE();//RTCʱ��ʹ��
}
// ============================================================================
//��������:��������ʱ��(����������,24Сʱ��)
//�������:����(1~7)
//���ز���:��
// ============================================================================
void RTC_Set_AlarmA(UINT8 byWeek,UINT8 byHour,UINT8 byMin,UINT8 bySec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours      = byHour;     //ʱ
    RTC_AlarmSturuct.AlarmTime.Minutes    = byMin;      //��
    RTC_AlarmSturuct.AlarmTime.Seconds    = bySec;      //��
    RTC_AlarmSturuct.AlarmTime.SubSeconds = 0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
    
    RTC_AlarmSturuct.AlarmMask            = RTC_ALARMMASK_NONE;             //��ȷƥ�����ڣ�ʱ����
    RTC_AlarmSturuct.AlarmSubSecondMask   = RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel  = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//������
    RTC_AlarmSturuct.AlarmDateWeekDay     = byWeek;                         //����
    RTC_AlarmSturuct.Alarm                = RTC_ALARM_A;                    //����A
    
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,0x01,0x02); //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}
// ============================================================================
//�����Ի��Ѷ�ʱ������  
//dwWclockSetVal:  @ref RTCEx_Wakeup_Timer_Definitions
//#define RTC_WAKEUPCLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)
//#define RTC_WAKEUPCLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)
//#define RTC_WAKEUPCLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)
//#define RTC_WAKEUPCLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)
//#define RTC_WAKEUPCLOCK_CK_SPRE_16BITS      ((uint32_t)0x00000004)
//#define RTC_WAKEUPCLOCK_CK_SPRE_17BITS      ((uint32_t)0x00000006)
// ============================================================================
//��������:cnt:�Զ���װ��ֵ.����0,�����ж�.
//�������:��
//���ز���:��
// ============================================================================
void RTC_Set_WakeUp(UINT32 dwWclockSetVal,UINT16 wCnt)
{ 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);//���RTC WAKE UP�ı�־
	
	HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,wCnt,dwWclockSetVal);//������װ��ֵ��ʱ�� 
	
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,0x02,0x02);                //��ռ���ȼ�1,�����ȼ�2
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}
// ============================================================================
//��������:RTC�����жϷ�����
//�������:��
//���ز���:��
// ============================================================================
void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);
}
// ============================================================================
//��������:RTC����A�жϴ���ص�����
//�������:��
//���ز���:��
// ============================================================================
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{

}
// ============================================================================
//��������:RTC WAKE UP�жϷ�����
//�������:��
//���ز���:��
// ============================================================================
void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 
}
// ============================================================================
//��������:RTC WAKE UP�жϴ���
//�������:��
//���ز���:��
// ============================================================================
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{

}
// ============================================================================

