// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: Usmart.h
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#ifndef __USMART_H
#define __USMART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usmart_str.h"

// ============================================================================
// USMART��Դռ�����@MDK 3.80A@2.0�汾��
// FLASH:4K~K�ֽ�(ͨ��USMART_USE_HELP��USMART_USE_WRFUNS����)
// SRAM:���ٵ������72�ֽ�
// SRAM���㹫ʽ:   SRAM=PARM_LEN+72-4  ����PARM_LEN������ڵ���4.
// Ӧ�ñ�֤��ջ��С��100���ֽ�.
// ============================================================================
// �û����ò���
#define MAX_FNAME_LEN 		30	//��������󳤶�										   
#define MAX_PARM 			10	//���Ϊ10������ ,�޸Ĵ˲���,�����޸�usmart_exe��֮��Ӧ.
#define PARM_LEN 			200	//���в���֮�͵ĳ��Ȳ�����PARM_LEN���ֽ�,ע�⴮�ڽ��ղ���Ҫ��֮��Ӧ(��С��PARM_LEN)

#define USMART_ENTIMX_SCAN 	1	//ʹ��TIM�Ķ�ʱ�ж���ɨ��SCAN����,�������Ϊ0,��Ҫ�Լ�ʵ�ָ�һ��ʱ��ɨ��һ��scan����.
								//(ע��:���Ҫ��runtimeͳ�ƹ���,��������USMART_ENTIMX_SCANΪ1)
#define USMART_USE_HELP		1	//ʹ�ð�������ֵ��Ϊ0�����Խ�ʡ��700���ֽڣ����ǽ������޷���ʾ������Ϣ��
#define USMART_USE_WRFUNS	1	//ʹ�ܶ�д����,��ȡ�κε�ַ��ֵ,д�Ĵ�����ֵ.

// ============================================================================
#define USMART_OK 			0  //�޴���
#define USMART_FUNCERR 		1  //��������
#define USMART_PARMERR 		2  //��������
#define USMART_PARMOVER 	3  //�������
#define USMART_NOFUNCFIND 	4  //δ�ҵ�ƥ�亯��

#define SP_TYPE_DEC      	0  //10���Ʋ�����ʾ
#define SP_TYPE_HEX       	1  //16���Ʋ�����ʾ
// ============================================================================
//�������б�	
typedef struct
{
	void * func;			//����ָ��
	const UINT8* name;		//������(���Ҵ�)	 
}tagUsmartFuncNameList;

//usmart���ƹ�����
typedef struct 
{
	tagUsmartFuncNameList *ptFuns;	    //������ָ��

	void   (*init)(UINT8);				//��ʼ��
	void   (*exe)(void); 				//ִ�� 
	void   (*scan)(void);               //ɨ��
	UINT8  (*cmd_rec)(UINT8 *str);	    //ʶ������������
	UINT8  fnum; 				  		//��������
	UINT8  pnum;                        //��������
	UINT8  id;							//����id
	UINT8  sptype;						//������ʾ����(���ַ�������):0,10����;1,16����;
	UINT8  plentbl[MAX_PARM];  		    //ÿ�������ĳ����ݴ��
	UINT8  parm[PARM_LEN];  			//�����Ĳ���
	UINT8  runtimeflag;					//0,��ͳ�ƺ���ִ��ʱ��;1,ͳ�ƺ���ִ��ʱ��,ע��:�˹��ܱ�����USMART_ENTIMX_SCANʹ�ܵ�ʱ��,������
	UINT16 parmtype;					//����������
	UINT32 runtime;					    //����ʱ��,��λ:0.1ms,�����ʱʱ��Ϊ��ʱ��CNTֵ��2��*0.1ms
}tagUsmartDev;

extern tagUsmartFuncNameList usmart_namelist[];	//��usmart_config.c���涨��
extern tagUsmartDev usmart_dev;		            //��usmart_config.c���涨��

void    usmart_init(UINT8 bySysclk);    //��ʼ��
void    usmart_exe(void);		        //ִ��
void    usmart_scan(void);              //ɨ��
void    write_addr(UINT32 dwAddr,UINT32 dwVal);//��ָ����ַд��ָ����ֵ
void    usmart_reset_runtime(void);     //��λ����ʱ��
UINT8   usmart_cmd_rec(UINT8 *pbyStr);	//ʶ��
UINT32  read_addr(UINT32 dwAddr);	    //��ȡָ����ַ��ֵ
UINT32  usmart_get_runtime(void);	    //��ȡ����ʱ��

#ifdef __cplusplus
}
#endif

#endif//__USMART_H

