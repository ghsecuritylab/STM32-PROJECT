// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: ADC�ڲ��¶ȶ�ȡ���
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
#include "adc_temperature.h" 
#include "Delay.h"

// ============================================================================
ADC_HandleTypeDef ADC1_Handler;

// ============================================================================
//��������:ADC��ʼ������
//�������:��
//���ز���:��
// ============================================================================
void Adc_Temperature_Init(void)
{ 
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    
    ADC1_Handler.Instance                   = ADC1;
    ADC1_Handler.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;     //4��Ƶ��ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution            = ADC_RESOLUTION_12B;           //12λģʽ
    ADC1_Handler.Init.DataAlign             = ADC_DATAALIGN_RIGHT;          //�Ҷ���
    ADC1_Handler.Init.ScanConvMode          = DISABLE;                      //��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection          = DISABLE;                      //�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode    = DISABLE;                      //�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion       = 1;                            //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode = DISABLE;                      //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion   = 0;                            //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv      = ADC_SOFTWARE_START;           //�������
    ADC1_Handler.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests = DISABLE;                      //�ر�DMA����
    HAL_ADC_Init(&ADC1_Handler);                                            //��ʼ��    
}
// ============================================================================
//��������:���ADCֵ
//�������:ͨ��ֵ
//���ز���:ת�����
// ============================================================================
UINT16 getAdcVal(UINT32 dwChannel)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel       = dwChannel;                    //ͨ��
    ADC1_ChanConf.Rank          = 1;                            //1������
    ADC1_ChanConf.SamplingTime  = ADC_SAMPLETIME_480CYCLES;     //����ʱ��
    ADC1_ChanConf.Offset        = 0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
   
	return (UINT16)HAL_ADC_GetValue(&ADC1_Handler);	            //�������һ��ADC1�������ת�����
}
// ============================================================================
//��������:��ȡָ��ͨ����ת��ֵ��ȡbyTimes��,Ȼ��ƽ�� 
//�������:��ȡ����
//���ز���:byTimes��ת�����ƽ��ֵ
// ============================================================================
UINT16 getAdcAverageVal(UINT32 dwChannel,UINT8 byTimes)
{
	UINT32 dwTemp;
	UINT8  byLoop;

    dwTemp=0;
    
	for(byLoop=0;byLoop<byTimes;byLoop++)
	{
		dwTemp += getAdcVal(dwChannel);
		delay_ms(5);
	}
	return (UINT16)(dwTemp/byTimes);
} 
// ============================================================================
//��������:�õ��¶�ֵ
//�������:
//���ز���:�¶�ֵ(������100��,��λ:��.)
// ============================================================================
SINT16 getTemperature(void)
{
	UINT32  dwAdcVal;
	SINT16  siResult;

    siResult = 0;
    //��ȡ�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
	dwAdcVal = getAdcAverageVal(ADC_CHANNEL_TEMPSENSOR,10);
    
	siResult = (((dwAdcVal*330*4*100)>>12)-76*4*100) + 25*100;  //ת��Ϊ�¶�ֵ,����100��.
	
	return siResult;
}
// ============================================================================



