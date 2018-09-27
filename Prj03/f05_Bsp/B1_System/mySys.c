// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: ϵͳʱ�ӳ�ʼ��,ʱ������/�жϹ���/GPIO���õ�
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================

#include "mySys.h"
// ============================================================================
#define CN_PLL_M  (25 )//��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
#define CN_PLL_N  (360)//��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.
#define CN_PLL_P  (RCC_PLLP_DIV2)//ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
#define CN_PLL_Q  (7)//USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.

// ============================================================================
// ��������:ϵͳʱ������
// Fvco(VCOƵ��)=Fs*(plln/pllm);
// SYSCLK(ϵͳʱ��Ƶ��)=Fvco/pllp=Fs*(plln/(pllm*pllp));
// Fusb(USB,SDIO,RNG�ȵ�ʱ��Ƶ��)=Fvco/pllq=Fs*(plln/(pllm*pllq));
// Fs:PLL����ʱ��Ƶ��,������HSI,HSE��. 
// �ⲿ����Ϊ25M��ʱ��,�Ƽ�ֵ:plln=360,pllm=25,pllp=2,pllq=8.
// �õ�:Fvco=25*(360/25)=360Mhz,SYSCLK=360/2=180Mhz,Fusb=360/8=45Mhz
// ============================================================================
void SystemClock_Config(void)
{    
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    
    //ʹ��PWRʱ��
    __HAL_RCC_PWR_CLK_ENABLE(); 
    
    /* ������������������õ�ѹ�������ѹ�����Ա�������δ�����Ƶ�ʹ���
    ʱʹ�����빦��ʵ��ƽ�⣬�˹���ֻ��STM32F42xx��STM32F43xx������ */
    
    //���õ�ѹ�������ѹ����1
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  //ʱ��ԴΪHSE
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;              //��HSE
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;              //��PLL
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;       //PLLʱ��Դѡ��HSE
    RCC_OscInitStruct.PLL.PLLM = CN_PLL_M; //��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
    RCC_OscInitStruct.PLL.PLLN = CN_PLL_N; //��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.  
    RCC_OscInitStruct.PLL.PLLP = CN_PLL_P; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
    RCC_OscInitStruct.PLL.PLLQ = CN_PLL_Q; //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.
    //��ʼ��
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }    
    //����Over-Driver����
    if(HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }   
    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  //APB1��Ƶϵ��Ϊ4
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  //APB2��Ƶϵ��Ϊ2 
    //ͬʱ����FLASH��ʱ����Ϊ5WS��Ҳ����6��CPU���ڡ�
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
}

// ============================================================================
// @brief  This function is executed in case of error occurrence.
// @param  None
// @retval None
// ============================================================================
void Error_Handler(void)
{
    /* User may add here some code to deal with this error */
    while(1)
    {
        
    }
}
// ============================================================================
#ifdef  USE_FULL_ASSERT
//��������ʾ�����ʱ��˺����������������ļ���������
//file��ָ��Դ�ļ�
//line��ָ�����ļ��е�����
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif
// ============================================================================

// ============================================================================
// THUMBָ�֧�ֻ������
// ============================================================================
// ��������:�������·���ʵ��ִ�л��ָ��WFI  
// �������:��
// �� �� ֵ:��
// ============================================================================
__asm void WFI_SET(void)
{
	WFI;		  
}
// ============================================================================
// ��������:�ر������ж�(���ǲ�����fault��NMI�ж�)
// �������:��
// �� �� ֵ:��
// ============================================================================
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
// ============================================================================
// ��������:���������ж�
// �������:��
// �� �� ֵ:��
// ============================================================================
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
// ============================================================================
// ��������:����ջ����ַ
// �������:addr:ջ����ַ
// �� �� ֵ:��
// ============================================================================
__asm void MSR_MSP(UINT32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
// ============================================================================

