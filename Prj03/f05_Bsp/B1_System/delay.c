// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: ��ʱ������
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "delay.h"
#include "mySys.h"
// ============================================================================
#if SYSTEM_SUPPORT_OS
#include "includes.h"					
#endif
// ============================================================================
#if SYSTEM_SUPPORT_OS		
    static UINT16 g_wFacMs=0;				        //ms��ʱ������,��os��,����ÿ�����ĵ�ms��
#endif
static UINT32 g_dwFacUs=0;							//us��ʱ������

// ============================================================================
#if SYSTEM_SUPPORT_OS						
//����UCOSII��UCOSIII��֧��,����OS,�����вο�����ֲ
//delay_osrunning:���ڱ�ʾOS��ǰ�Ƿ���������,�Ծ����Ƿ����ʹ����غ���
//delay_ostickspersec:���ڱ�ʾOS�趨��ʱ�ӽ���,delay_init�����������������ʼ��systick
//delay_osintnesting:���ڱ�ʾOS�ж�Ƕ�׼���,��Ϊ�ж����治���Ե���,delay_msʹ�øò����������������
// ============================================================================
#ifdef 	OS_CRITICAL_METHOD						//Ҫ֧��UCOSII				
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0-������;1-����
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNesting		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif
#ifdef 	CPU_CFG_CRITICAL_METHOD					//Ҫ֧��UCOSIII	
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0-������;1-����
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

// ============================================================================
// ��������:us����ʱʱ,�ر��������(��ֹ���us���ӳ�),��������OS�������,��ֹ����
// ============================================================================
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   			//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);						//UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#else										//����UCOSII
	OSSchedLock();							//UCOSII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#endif
}
// ============================================================================
// ��������:us����ʱʱ,�ָ��������,���ڽ���OS�������,���¿�������
// ============================================================================
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   			//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);					//UCOSIII�ķ�ʽ,�ָ�����
#else										//����UCOSII
	OSSchedUnlock();						//UCOSII�ķ�ʽ,�ָ�����
#endif
}
// ============================================================================
// ��������:����OS�Դ�����ʱ������ʱ,����OS��ʱ,���������������.
// �������:dwTicks-��ʱ������
// �� �� ֵ:��
// ============================================================================
void delay_ostimedly(UINT32 dwTicks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(dwTicks,OS_OPT_TIME_PERIODIC,&err); //UCOSIII��ʱ��������ģʽ
#else
	OSTimeDly(dwTicks);						    //UCOSII��ʱ
#endif 
}
// ============================================================================
// ��������:systick�жϷ�����,ʹ��OSʱ�õ�
// ============================================================================
void SysTick_Handler(void)
{	
    HAL_IncTick();
	if(delay_osrunning==1)					//OS��ʼ����,��ִ�������ĵ��ȴ���
	{
		OSIntEnter();						//�����ж�
		OSTimeTick();       				//����ucos��ʱ�ӷ������               
		OSIntExit();       	 				//���������л����ж�
	}
}
// ============================================================================
#endif //end-SYSTEM_SUPPORT_OS
// ============================================================================
// ��������:��ʼ���ӳٺ���,��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
// �������:bySysClk-ϵͳʱ��Ƶ��,SYSTICK��ʱ�ӹ̶�ΪAHBʱ��
// �� �� ֵ:��
// ============================================================================
void delay_init(UINT8 bySysClk)
{
#if SYSTEM_SUPPORT_OS 						
	UINT32 dwReload;//reloadΪ24λ�Ĵ���,���ֵ:16777216,��180M��,Լ��0.745s����	
#endif

    //SysTickƵ��ΪHCLK
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    g_dwFacUs = bySysClk;						

#if SYSTEM_SUPPORT_OS 						//�����Ҫ֧��OS.
	dwReload = bySysClk;					    //ÿ���ӵļ������� ��λΪK
	//����delay_ostickspersec�趨���ʱ��
	dwReload = dwReload * (1000000/delay_ostickspersec);	
	g_wFacMs = 1000/delay_ostickspersec;		//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  //����SYSTICK�ж�
	SysTick->LOAD  = dwReload; 					//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //����SYSTICK
#endif

}								    

// ============================================================================
#if SYSTEM_SUPPORT_OS 						
// ============================================================================
// ��������:us����ʱ����
// �������:Ҫ��ʱ��us��(0~190887435)(���ֵ��2^32/g_dwFacUs@fac_us=22.5)	
// �� �� ֵ:��
// ============================================================================
void delay_us(UINT32 dwUsCnt)
{		
	UINT32 dwTicks;
	UINT32 dwTOld,dwTNow,dwTCnt;
	UINT32 dwReload;
	
    dwTCnt   = 0;
    dwReload = SysTick->LOAD;           //LOAD��ֵ  
    dwTicks  = dwUsCnt * g_dwFacUs; 	//��Ҫ�Ľ����� 
	delay_osschedlock();				//��ֹOS���ȣ���ֹ���us��ʱ
	dwTOld = SysTick->VAL;        		//�ս���ʱ�ļ�����ֵ
	
	while(1)
	{
		dwTNow = SysTick->VAL;	
		if(dwTNow != dwTOld)
		{	    
			if(dwTNow < dwTOld)
			{
                dwTCnt = dwTCnt + ( dwTOld - dwTNow );  
            }
			else 
			{
                dwTCnt = dwTCnt + ( dwReload - dwTNow + dwTOld );     
            }
			dwTOld=dwTNow;
			if(dwTCnt >= dwTicks)
			{
                break;          //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
            }
		}  
	};
	delay_osschedunlock();					//�ָ�OS����											    
}
// ============================================================================
// ��������:ms����ʱ����
// �������:Ҫ��ʱ��ms��(0~65535)
// �� �� ֵ:��
// ============================================================================
void delay_ms(UINT16 wMsCnt)
{	
	if(delay_osrunning&&delay_osintnesting==0)//���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)	    
	{		 
		if(wMsCnt>=g_wFacMs)						//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			delay_ostimedly(wMsCnt/g_wFacMs);	//OS��ʱ
		}
		wMsCnt%=g_wFacMs;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((UINT32)(wMsCnt*1000));				//��ͨ��ʽ��ʱ
}

// ============================================================================
#else  //else-SYSTEM_SUPPORT_OS
// ============================================================================
// ��������:us����ʱ����
// �������:nusΪҪ��ʱ��us��,0~190887435(���ֵ��2^32/g_dwFacUs@fac_us=22.5)	 
// �� �� ֵ:��
// ============================================================================
void delay_us(UINT32 dwUsCnt)
{		
	UINT32 dwTicks;
	UINT32 dwTOld,dwTNow,dwTCnt;
	UINT32 dwReload;
    
    dwTCnt   = 0;
	dwReload = SysTick->LOAD;         // LOAD��ֵ
	dwTicks  = dwUsCnt * g_dwFacUs;   // ��Ҫ�Ľ����� 
	dwTOld   = SysTick->VAL;          // �ս���ʱ�ļ�����ֵ
	
	while(1)
	{
		dwTNow = SysTick->VAL;	
        
		if(dwTNow != dwTOld)
		{	    
			if(dwTNow<dwTOld)
			{
                dwTCnt = dwTCnt + ( dwTOld - dwTNow );
            }
			else 
			{
                dwTCnt = dwTCnt + ( dwReload - dwTNow + dwTOld );       
            }

            dwTOld = dwTNow;
            
            //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�
			if(dwTCnt >= dwTicks)
            {
                break;         
            }
		}  
	};
}

// ============================================================================
// ��������:ms����ʱ����
// �������:Ҫ��ʱ��ms��
// �� �� ֵ:��
// ============================================================================
void delay_ms(UINT16 wMsCnt)
{
	UINT32 dwLoop;
	for(dwLoop=0;dwLoop<wMsCnt;dwLoop++)
    {
        delay_us(1000);
    }
}
// ============================================================================
#endif//end-SYSTEM_SUPPORT_OS
// ============================================================================

