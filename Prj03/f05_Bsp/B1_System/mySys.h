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
#ifndef __SYS_H__
#define __SYS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"
#include "Typedef.h"
#include "Logicgate.h"

// ============================================================================
#define SYSTEM_SUPPORT_OS		0		//ϵͳ�ļ����Ƿ�֧��OS(0:��֧��os,1:֧��os)
// ============================================================================
//����һЩ���õ��������Ͷ̹ؼ��� 
typedef int8_t     s8;
typedef int16_t    s16;
typedef int32_t    s32;

typedef uint8_t    u8;
typedef int16_t    u16;
typedef int32_t    u32;

typedef __IO  int8_t    vs8;
typedef __IO  int16_t   vs16;
typedef __IO  int32_t   vs32;

typedef __IO  uint8_t   vu8;
typedef __IO  uint16_t  vu16;
typedef __IO  uint32_t  vu32;

typedef __I   int8_t    vsc8;   
typedef __I   int16_t   vsc16; 
typedef __I   int32_t   vsc32; 

typedef __I   uint8_t   vuc8;  
typedef __I   uint16_t  vuc16; 
typedef __I   uint32_t  vuc32;  

typedef const int8_t    sc8;  
typedef const int16_t   sc16;  
typedef const int32_t   sc32; 

typedef const uint8_t   uc8; 
typedef const uint16_t  uc16;  
typedef const uint32_t  uc32;  
// ============================================================================
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014 
#define GPIOJ_ODR_Addr    (GPIOJ_BASE+20) //0x40022414
#define GPIOK_ODR_Addr    (GPIOK_BASE+20) //0x40022814

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
#define GPIOJ_IDR_Addr    (GPIOJ_BASE+16) //0x40022410 
#define GPIOK_IDR_Addr    (GPIOK_BASE+16) //0x40022810 

//λ������,IO�ڲ����궨��
#define MACRO_MEM_ADDR(Addr)           ( *((volatile unsigned long  *)(Addr)) ) 
#define MACRO_BIT_BAND(Addr, BitNum)   ( (Addr & 0xF0000000)+0x2000000+((Addr &0xFFFFF)<<5)+(BitNum<<2) ) 
#define MACRO_BIT_ADDR(Addr, BitNum)   ( MACRO_MEM_ADDR(MACRO_BIT_BAND(Addr, BitNum)) ) 

//IO�ڲ���,ֻ�Ե�һ��IO��,ȷ��n��ֵС��16
#define MACRO_PA_Out(n)   MACRO_BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define MACRO_PA_In(n)    MACRO_BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 
#define MACRO_PB_Out(n)   MACRO_BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define MACRO_PB_In(n)    MACRO_BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 
#define MACRO_PC_Out(n)   MACRO_BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define MACRO_PC_In(n)    MACRO_BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 
#define MACRO_PD_Out(n)   MACRO_BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define MACRO_PD_In(n)    MACRO_BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 
#define MACRO_PE_Out(n)   MACRO_BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define MACRO_PE_In(n)    MACRO_BIT_ADDR(GPIOE_IDR_Addr,n)  //����
#define MACRO_PF_Out(n)   MACRO_BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define MACRO_PF_In(n)    MACRO_BIT_ADDR(GPIOF_IDR_Addr,n)  //����
#define MACRO_PG_Out(n)   MACRO_BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define MACRO_PG_In(n)    MACRO_BIT_ADDR(GPIOG_IDR_Addr,n)  //����
#define MACRO_PH_Out(n)   MACRO_BIT_ADDR(GPIOH_ODR_Addr,n)  //��� 
#define MACRO_PH_In(n)    MACRO_BIT_ADDR(GPIOH_IDR_Addr,n)  //����
#define MACRO_PI_Out(n)   MACRO_BIT_ADDR(GPIOI_ODR_Addr,n)  //��� 
#define MACRO_PI_In(n)    MACRO_BIT_ADDR(GPIOI_IDR_Addr,n)  //����
#define MACRO_PJ_Out(n)   MACRO_BIT_ADDR(GPIOJ_ODR_Addr,n)  //��� 
#define MACRO_PJ_In(n)    MACRO_BIT_ADDR(GPIOJ_IDR_Addr,n)  //����
#define MACRO_PK_Out(n)   MACRO_BIT_ADDR(GPIOK_ODR_Addr,n)  //��� 
#define MACRO_PK_In(n)    MACRO_BIT_ADDR(GPIOK_IDR_Addr,n)  //����

// ============================================================================
extern void SystemClock_Config(void);//ʱ��ϵͳ����
extern void Error_Handler(void);
// ============================================================================
//����Ϊ��ຯ��
void  WFI_SET(void);		//ִ��WFIָ��
void  INTX_DISABLE(void);   //�ر������ж�
void  INTX_ENABLE(void);	//���������ж�
void  MSR_MSP(UINT32 addr);	//���ö�ջ��ַ 

// ============================================================================

#ifdef __cplusplus
}
#endif
#endif//__SYS_H__

