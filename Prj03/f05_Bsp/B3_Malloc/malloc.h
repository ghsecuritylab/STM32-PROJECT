// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: �ڴ������ģ��
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#ifndef __MALLOC_H__
#define __MALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mySys.h" 

// ============================================================================
#ifndef NULL
#define NULL 0
#endif

//���������ڴ��
#define SRAMIN	 0		//�ڲ��ڴ��
#define SRAMEX   1		//�ⲿ�ڴ��(SDRAM)
#define SRAMCCM  2		//CCM�ڴ��(�˲���SRAM����CPU���Է���!!!)
#define SRAMBANK 3	    //����֧�ֵ�SRAM����.	

//mem1�ڴ�����趨.�ڲ�SRAM
#define MEM1_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM1_MAX_SIZE			100*1024  						//�������ڴ� 160K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С

//mem2�ڴ�����趨.�ⲿSDRAM
#define MEM2_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM2_MAX_SIZE			4096 *1024  					//�������ڴ�4096K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С
		 
//mem3�ڴ�����趨.mem3����CCM,���ڹ���CCM(�ر�ע��,�ⲿ��SRAM,��CPU���Է���!!)
#define MEM3_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM3_MAX_SIZE			60 *1024  						//�������ڴ�60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//�ڴ���С
		 
//�ڴ���������
typedef struct 
{
	void   (*init)(UINT8);					//��ʼ��
	UINT16 (*perused)(UINT8);		  	    //�ڴ�ʹ����
	UINT8 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
	UINT32  *memmap[SRAMBANK]; 				//�ڴ����״̬��
	UINT8    memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����
}tagMallocDev;
// ============================================================================
//�û����ú���
extern void my_mem_init(UINT8 byMemX);                                  //�ڴ�س�ʼ������  
extern void myfree(UINT8 byMemX,void *pMemBaseAdr);  			        //�ڴ��ͷ�(�ⲿ����)
extern void *mymalloc(UINT8 byMemX,UINT32 dwSize);			            //�ڴ����(�ⲿ����)
extern void *myrealloc(UINT8 byMemX,void *pMemBaseAdr,UINT32 dwSize);   //���·����ڴ�(�ⲿ����)
// ============================================================================

#ifdef __cplusplus
}
#endif

#endif//__MALLOC_H__

