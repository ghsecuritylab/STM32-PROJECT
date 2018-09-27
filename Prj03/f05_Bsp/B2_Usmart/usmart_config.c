// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: �û�����ģ��
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "usmart.h"
#include "usmart_str.h"

// ============================================================================
// Ҫ�������õ��ĺ�����������ͷ�ļ�(�û����) 
#include "delay.h"	 	
#include "mySys.h"
#include "rtc.h" 
//#include "lcd.h"
//#include "sdram.h"
//#include "ltdc.h"
								 
//extern void led_set(UINT8 bySta);
//extern void test_fun(void(*ledset)(UINT8),UINT8 bySta);	

// ============================================================================
//�������б��ʼ��(�û����)
tagUsmartFuncNameList usmart_namelist[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr, "UINT32  read_addr(UINT32 dwAddr);",
	(void*)write_addr,"void write_addr(UINT32 dwAddr,UINT32 dwVal)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(UINT16 wMsCnt)",
 	(void*)delay_us,"void delay_us(UINT32 dwUsCnt)",	
    (HAL_StatusTypeDef*)RTC_Set_Time,"HAL_StatusTypeDef RTC_Set_Time(UINT8 byHour,UINT8 byMin,UINT8 bySec,UINT8 byTimeFormat)",
    (HAL_StatusTypeDef*)RTC_Set_Date,"HAL_StatusTypeDef RTC_Set_Date(UINT8 byYear,UINT8 byMonth,UINT8 byDate,UINT8 byWeek)",

//	(void*)led_set, "void led_set(UINT8 bySta)",
//	(void*)test_fun,"void test_fun(void(*ledset)(UINT8),UINT8 bySta)",						
};
// ============================================================================
// �������ƹ�������ʼ��,�õ������ܿغ���������,����������
// ============================================================================
tagUsmartDev usmart_dev=
{
	usmart_namelist,
	usmart_init,
	usmart_exe,
	usmart_scan,
    usmart_cmd_rec,
	sizeof(usmart_namelist)/sizeof(tagUsmartFuncNameList),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   

// ============================================================================

