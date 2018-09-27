// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: �������ⲿ������ģ��
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "usmart.h"
#include "usart.h"
#include "mySys.h" 

// ============================================================================
TIM_HandleTypeDef TIM4_Handler;      //��ʱ����� 

//ϵͳ����
UINT8 *tabSysCmd[]=
{
	"?",
	"help",
	"list",
	"id",
	"hex",
	"dec",
	"runtime",	   
};	
// ============================================================================
// ��������:����ϵͳָ��
// �������:
// �� �� ֵ:0,�ɹ�����;����,�������;
// ============================================================================
UINT8 usmart_sys_cmd_exe(UINT8 *bypStr)
{
	UINT8  byLoop;
	UINT8  bySFName[MAX_FNAME_LEN];//��ű��غ�����
	UINT8  byPNum;
	UINT8  byRVal;
	UINT32 dwRes;  
    
	dwRes = usmart_get_cmdname(bypStr,bySFName,&byLoop,MAX_FNAME_LEN);//�õ�ָ�ָ���
	if(dwRes)
	{
        return USMART_FUNCERR;//�����ָ�� 
    }
    
	bypStr = bypStr + byLoop;
    
	for(byLoop=0;byLoop<sizeof(tabSysCmd)/4;byLoop++)//֧�ֵ�ϵͳָ��
	{
        if(usmart_strcmp(bySFName,tabSysCmd[byLoop])==0)
        {
            break;
        }
	}
	switch(byLoop)
	{					   
		case 0:
		case 1://����ָ��
			printf("\r\n");
#if USMART_USE_HELP 
			printf("------------------------MyShell v1.0-------------------------\r\n");
//			printf("    MyShell��һ�����ɵĴ��ڵ������,����ͨ���������ֵ��ó���\r\n");
//			printf("������κκ�����ִ��,����������ĺ������������,�����������\r\n");
//			printf("֧��10���������,��֧�ֺ�������ֵ��ʾ.֧�ֲ�����ʾ�������ù�\r\n"),
//          printf("��,֧�ֽ���ת������.(֧������(10/16����,֧�ָ���)���ַ����� \r\n");
//			printf("������ڵ�ַ����Ϊ����)\r\n"),  
//			printf("MyShell��7��ϵͳ����(����Сд):\r\n\r\n");
			printf("?:      ��ȡ������Ϣ\r\n");
			printf("help:   ��ȡ������Ϣ\r\n");
			printf("list:   ���õĺ����б�\r\n\n");
			printf("id:     ���ú�����ID�б�\r\n\n");
			printf("hex:    ����16������ʾ,����ո�+���ּ�ִ�н���ת��\r\n\n");
			printf("dec:    ����10������ʾ,����ո�+���ּ�ִ�н���ת��\r\n\n");
			printf("runtime:1,�����������м�ʱ;0,�رպ������м�ʱ;\r\n\n");
			printf("�밴�ճ����д��ʽ���뺯�������������Իس�������.\r\n");    
			printf("--------------------------MADE-BY-Sanyu---------------------- \r\n");
#else
			printf("ָ��ʧЧ\r\n");
#endif
			break;
		case 2://��ѯָ��
			printf("\r\n");
			printf("-------------------------�����嵥--------------------------- \r\n");
			for(byLoop=0;byLoop<usmart_dev.fnum;byLoop++)printf("%s\r\n",usmart_dev.ptFuns[byLoop].name);
			printf("\r\n");
			break;	 
		case 3://��ѯID
			printf("\r\n");
			printf("-------------------------���� ID --------------------------- \r\n");
			for(byLoop=0;byLoop<usmart_dev.fnum;byLoop++)
			{
				usmart_get_fname((UINT8*)usmart_dev.ptFuns[byLoop].name,bySFName,&byPNum,&byRVal);//�õ����غ����� 
				printf("%s id is:\r\n0X%08X\r\n",bySFName,usmart_dev.ptFuns[byLoop].func); //��ʾID
			}
			printf("\r\n");
			break;
		case 4://hexָ��
			printf("\r\n");
			usmart_get_aparm(bypStr,bySFName,&byLoop);
			if(byLoop==0)//��������
			{
				byLoop = usmart_str2num(bySFName,&dwRes);	   	//��¼�ò���	
				if(byLoop==0)						  	//����ת������
				{
					printf("HEX:0X%X\r\n",dwRes);	   	//תΪ16����
				}
                else if(byLoop!=4)
                {
                    return USMART_PARMERR;//��������.
                }
				else 				   				//������ʾ�趨����
				{
					printf("16���Ʋ�����ʾ!\r\n");
					usmart_dev.sptype=SP_TYPE_HEX;  
				}

			}
            else 
            {
                return USMART_PARMERR;          //��������.
            }
			printf("\r\n"); 
			break;
		case 5://decָ��
			printf("\r\n");
			usmart_get_aparm(bypStr,bySFName,&byLoop);
			if(byLoop==0)//��������
			{
				byLoop=usmart_str2num(bySFName,&dwRes);	   	//��¼�ò���	
				if(byLoop==0)						   	//����ת������
				{
					printf("DEC:%lu\r\n",dwRes);	   	//תΪ10����
				}
                else if(byLoop!=4)
                {
                    return USMART_PARMERR;//��������.
                }
				else 				   				//������ʾ�趨����
				{
					printf("10���Ʋ�����ʾ!\r\n");
					usmart_dev.sptype=SP_TYPE_DEC;  
				}

			}
            else 
            {
                return USMART_PARMERR;          //��������. 
            }
			printf("\r\n"); 
			break;	 
		case 6://runtimeָ��,�����Ƿ���ʾ����ִ��ʱ��
			printf("\r\n");
			usmart_get_aparm(bypStr,bySFName,&byLoop);
			if(byLoop==0)//��������
			{
				byLoop=usmart_str2num(bySFName,&dwRes);	   		//��¼�ò���	
				if(byLoop==0)						   		//��ȡָ����ַ���ݹ���
				{
					if(USMART_ENTIMX_SCAN==0)
					{
                        printf("\r\nError! \r\nTo EN RunTime function,Please set USMART_ENTIMX_SCAN = 1 first!\r\n");//����
                    }
					else
					{
						usmart_dev.runtimeflag=dwRes;
						if(usmart_dev.runtimeflag)
						{
                            printf("Run Time Calculation ON\r\n");
                        }
						else 
						{
                            printf("Run Time Calculation OFF\r\n"); 
                        }
					}
				}
                else 
                {
                    return USMART_PARMERR;              //δ������,���߲�������  
                }
 			}
            else 
            {
                return USMART_PARMERR;              //��������. 
            }
			printf("\r\n"); 
			break;	    
		default://�Ƿ�ָ��
			return USMART_FUNCERR;
	}
	return 0;
}
// ============================================================================
// TIM4_IRQHandler��Timer4_Init,��Ҫ����MCU�ص������޸�.ȷ������������Ƶ��Ϊ:10Khz����.����,��ʱ����Ҫ�����Զ���װ�ع���!!
// ============================================================================
#if USMART_ENTIMX_SCAN==1
// ============================================================================
// ��������:��λruntime 
// �������:��
// �� �� ֵ:��
// ============================================================================
void usmart_reset_runtime(void)
{
    __HAL_TIM_CLEAR_FLAG(    &TIM4_Handler,TIM_FLAG_UPDATE);//����жϱ�־λ 
    __HAL_TIM_SET_AUTORELOAD(&TIM4_Handler,0XFFFF);         //��װ��ֵ���õ����
    __HAL_TIM_SET_COUNTER(   &TIM4_Handler,0);              //��ն�ʱ����CNT
    
	usmart_dev.runtime=0;	
}
// ============================================================================
// ��������:��ȡ��������ʱ��,ͨ����ȡCNTֵ��ȡ,����usmart��ͨ���жϵ��õĺ���,
// ���Զ�ʱ���жϲ�����Ч,��ʱ����޶�,ֻ��ͳ��2��CNT��ֵ,Ҳ���������+���һ��,
// ���������2��,û������,���������ʱ,������:2*������CNT*0.1ms.��STM32��˵,��:13.1s����
// �������:
// �� �� ֵ:ִ��ʱ��,��λ:0.1ms,�����ʱʱ��Ϊ��ʱ��CNTֵ��2��*0.1ms
// ============================================================================
UINT32 usmart_get_runtime(void)
{
	if(__HAL_TIM_GET_FLAG(&TIM4_Handler,TIM_FLAG_UPDATE)==SET)//�������ڼ�,�����˶�ʱ�����
	{
		usmart_dev.runtime = usmart_dev.runtime + 0XFFFF;
	}
    
	usmart_dev.runtime = usmart_dev.runtime + __HAL_TIM_GET_COUNTER(&TIM4_Handler);

    //���ؼ���ֵ
	return usmart_dev.runtime;		
}  
// ============================================================================
// ��������:��ʱ��4�жϷ������
// �������:��
// �� �� ֵ:��
// ============================================================================
void TIM4_IRQHandler(void)
{ 		    		  			       
    if(__HAL_TIM_GET_IT_SOURCE(&TIM4_Handler,TIM_IT_UPDATE)==SET)//����ж�
    {
        usmart_dev.scan();	//ִ��usmartɨ��
        __HAL_TIM_SET_COUNTER(   &TIM4_Handler,0);;     //��ն�ʱ����CNT
        __HAL_TIM_SET_AUTORELOAD(&TIM4_Handler,100);    //�ָ�ԭ��������
    }
    __HAL_TIM_CLEAR_IT(&TIM4_Handler, TIM_IT_UPDATE);   //����жϱ�־λ
}
// ============================================================================
// ��������:ʹ�ܶ�ʱ��4,ʹ���ж�.
// �������:�Զ�װ��ֵ,��Ƶֵ
// �� �� ֵ:��
// ============================================================================
void Timer4_Init(UINT16 wArr,UINT16 wPsc)
{ 
    
    __HAL_RCC_TIM4_CLK_ENABLE();            //��ʱ��4
    
    HAL_NVIC_SetPriority(TIM4_IRQn,3,3);    //�����ж����ȼ�����ռ���ȼ�3�������ȼ�3
    HAL_NVIC_EnableIRQ(  TIM4_IRQn);        //����ITM4�ж�    
    
    TIM4_Handler.Instance           = TIM4;                   //ͨ�ö�ʱ��4
    TIM4_Handler.Init.Prescaler     = wPsc;                   //��Ƶ
    TIM4_Handler.Init.CounterMode   = TIM_COUNTERMODE_UP;     //���ϼ�����
    TIM4_Handler.Init.Period        = wArr;                   //�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(    &TIM4_Handler);
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�ж� 					 
}
#endif

// ============================================================================
// ��������:��ʼ�����ڿ�����
// �������:sysclk:ϵͳʱ�ӣ�Mhz��
// �� �� ֵ:
// ============================================================================
void usmart_init(UINT8 bySysclk)
{
#if USMART_ENTIMX_SCAN==1
    //��Ƶ,ʱ��Ϊ10K ,100ms�ж�һ��,ע��,����Ƶ�ʱ���Ϊ10Khz,�Ժ�runtime��λ(0.1ms)ͬ��.
	Timer4_Init(1000,(UINT32)bySysclk*100-1);
#endif
	usmart_dev.sptype=1;	//ʮ��������ʾ����
}
// ============================================================================
// ��������:��str�л�ȡ������,id,��������Ϣ
// �������:�ַ���ָ��.
// �� �� ֵ:0,ʶ��ɹ�;����,�������.
// ============================================================================
UINT8 usmart_cmd_rec(UINT8 *pbyStr) 
{
	UINT8 bySta,byLoop,byRval;      //״̬	 
	UINT8 byRpnum,bySpnum;
	UINT8 byRfname[MAX_FNAME_LEN];  //�ݴ�ռ�,���ڴ�Ž��յ��ĺ�����  
	UINT8 bySfname[MAX_FNAME_LEN];  //��ű��غ�����
	
	bySta = usmart_get_fname(pbyStr,byRfname,&byRpnum,&byRval);//�õ����յ������ݵĺ���������������	  

    if(bySta)
	{
        return bySta;//����
    }
	for(byLoop=0;byLoop<usmart_dev.fnum;byLoop++)
	{
		bySta = usmart_get_fname((UINT8*)usmart_dev.ptFuns[byLoop].name,bySfname,&bySpnum,&byRval);//�õ����غ���������������
		if(bySta)
		{
            return bySta;//���ؽ�������     
        }
        
		if(usmart_strcmp(bySfname,byRfname)==0)     //���
		{
			if(bySpnum>byRpnum)
			{
                return USMART_PARMERR;              //��������(���������Դ����������)
            }
			usmart_dev.id = byLoop;                 //��¼����ID.
			break;
		}	
	}
    
	if(byLoop==usmart_dev.fnum)
	{
        return USMART_NOFUNCFIND;                   //δ�ҵ�ƥ��ĺ���
    }
    
 	bySta = usmart_get_fparam(pbyStr,&byLoop);		//�õ�������������	

    if(bySta)
    {
        return bySta;                               //���ش���
    }
	usmart_dev.pnum = byLoop;						//����������¼
    return USMART_OK;
}
// ============================================================================
// ��������:usamrtִ�к���,����ִ�дӴ����յ�����Ч����.
// �������:
// �� �� ֵ:
// ============================================================================
// ���֧��10�������ĺ���,����Ĳ���֧��Ҳ������ʵ��.�����õĺ���.һ��5�����ҵĲ����ĺ����Ѿ����ټ���.
// �ú������ڴ��ڴ�ӡִ�����.��:"������(����1������2...����N)=����ֵ".����ʽ��ӡ.
// ����ִ�еĺ���û�з���ֵ��ʱ��,����ӡ�ķ���ֵ��һ�������������.
void usmart_exe(void)
{
	UINT8  byId,byLoop;
	UINT8  bySfname[MAX_FNAME_LEN];//��ű��غ�����
	UINT8  byPnum,byRval;
	UINT32 dwRes;		   
	UINT32 dwTemp[MAX_PARM];//����ת��,ʹ֧֮�����ַ��� 
    
	byId = usmart_dev.id;
    
	if(byId >= usmart_dev.fnum)
	{
        return;//��ִ��.
    }
    
	usmart_get_fname((UINT8*)usmart_dev.ptFuns[byId].name,bySfname,&byPnum,&byRval);//�õ����غ�����,���������� 

    printf("\r\n%s(",bySfname);//�����Ҫִ�еĺ�����

    for(byLoop=0;byLoop<byPnum;byLoop++)//�������
	{
		if(usmart_dev.parmtype&(1<<byLoop))//�������ַ���
		{
			printf("%c",'"');			 
			printf("%s",usmart_dev.parm+usmart_get_parmpos(byLoop));
			printf("%c",'"');
			dwTemp[byLoop]=(UINT32)&(usmart_dev.parm[usmart_get_parmpos(byLoop)]);
		}
        else						  //����������
		{
			dwTemp[byLoop]=*(UINT32*)(usmart_dev.parm+usmart_get_parmpos(byLoop));
			if(usmart_dev.sptype==SP_TYPE_DEC)
			{
                printf("%ld",dwTemp[byLoop]);//10���Ʋ�����ʾ
            }
			else 
			{
                printf("0X%X",dwTemp[byLoop]);//16���Ʋ�����ʾ       
            }
		}
		if(byLoop!=byPnum-1)
		{
            printf(",");
        }
	}
    
	printf(")");
    
#if USMART_ENTIMX_SCAN==1
	usmart_reset_runtime();	//��ʱ������,��ʼ��ʱ
#endif

	switch(usmart_dev.pnum)
	{
		case 0://�޲���(void����)											  
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)();
			break;
	    case 1://��1������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0]);
			break;
	    case 2://��2������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0],dwTemp[1]);
			break;
	    case 3://��3������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0],dwTemp[1],dwTemp[2]);
			break;
	    case 4://��4������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0],dwTemp[1],dwTemp[2],dwTemp[3]);
			break;
	    case 5://��5������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0],dwTemp[1],dwTemp[2],dwTemp[3],dwTemp[4]);
			break;
	    case 6://��6������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0],dwTemp[1],dwTemp[2],dwTemp[3],dwTemp[4],\
			dwTemp[5]);
			break;
	    case 7://��7������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0],dwTemp[1],dwTemp[2],dwTemp[3],dwTemp[4],\
			dwTemp[5],dwTemp[6]);
			break;
	    case 8://��8������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0],dwTemp[1],dwTemp[2],dwTemp[3],dwTemp[4],\
			dwTemp[5],dwTemp[6],dwTemp[7]);
			break;
	    case 9://��9������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0],dwTemp[1],dwTemp[2],dwTemp[3],dwTemp[4],\
			dwTemp[5],dwTemp[6],dwTemp[7],dwTemp[8]);
			break;
	    case 10://��10������
			dwRes=(*(UINT32(*)())usmart_dev.ptFuns[byId].func)(dwTemp[0],dwTemp[1],dwTemp[2],dwTemp[3],dwTemp[4],\
			dwTemp[5],dwTemp[6],dwTemp[7],dwTemp[8],dwTemp[9]);
			break;
	}
    
#if USMART_ENTIMX_SCAN==1
	usmart_get_runtime();//��ȡ����ִ��ʱ��
#endif

    //��Ҫ����ֵ.
	if(byRval==1)
	{
		if(usmart_dev.sptype==SP_TYPE_DEC)
		{
            printf("=%lu;\r\n",dwRes);//���ִ�н��(10���Ʋ�����ʾ)
        }
		else 
		{
            printf("=0X%X;\r\n",dwRes);//���ִ�н��(16���Ʋ�����ʾ)      
        }
	}
    else 
    {
        printf(";\r\n");//����Ҫ����ֵ,ֱ���������        
    }

    //��Ҫ��ʾ����ִ��ʱ��
	if(usmart_dev.runtimeflag)	
	{ 
		printf("Function Run Time:%d.%1dms\r\n",usmart_dev.runtime/10,usmart_dev.runtime%10);//��ӡ����ִ��ʱ�� 
	}	
}
// ============================================================================
// ��������:usmartɨ�躯��,ʵ��usmart�ĸ�������.�ú�����Ҫÿ��һ��ʱ�䱻����һ��
// �Լ�ʱִ�дӴ��ڷ������ĸ�������,�������ж��������,�Ӷ�ʵ���Զ�����.
// �������:��
// �� �� ֵ:��
// ============================================================================
void usmart_scan(void)
{
	UINT8 bySta,byLen;  
    
    //���ڽ�����ɣ�
	if(USART_RX_STA&0x8000)
	{					   
		byLen = USART_RX_STA&0x3fff;	//�õ��˴ν��յ������ݳ���
		
		USART_RX_BUF[byLen] = '\0';	//��ĩβ���������. 
		
		bySta = usmart_dev.cmd_rec(USART_RX_BUF);//�õ�����������Ϣ
		
		if(bySta==0)
		{
            usmart_dev.exe();   //ִ�к��� 
        }
		else 
		{  
			byLen=usmart_sys_cmd_exe(USART_RX_BUF);
            
			if(byLen!=USMART_FUNCERR)
			{
                bySta = byLen;
            }
            
			if(bySta)
			{   
				switch(bySta)
				{
					case USMART_FUNCERR:
						printf("��������!\r\n");
						break;	
					case USMART_PARMERR:
						printf("��������!\r\n");   			
						break;				
					case USMART_PARMOVER:
						printf("����̫��!\r\n");   			
						break;		
					case USMART_NOFUNCFIND:
						printf("δ�ҵ�ƥ��ĺ���!\r\n");   			
						break;		
				}
			}
		}
		USART_RX_STA=0;//״̬�Ĵ������	    
	}
}
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
// ============================================================================
// ��������:��ȡָ����ַ��ֵ
// �������:��
// �� �� ֵ:��
// ============================================================================
UINT32 read_addr(UINT32 dwAddr)
{
	return *(UINT32*)dwAddr;
}
// ============================================================================
// ��������:��ָ����ַд��ָ����ֵ	
// �������:��
// �� �� ֵ:��
// ============================================================================
void write_addr(UINT32 dwAddr,UINT32 dwVal)
{
	*(UINT32*)dwAddr = dwVal; 	
}
#endif
// ============================================================================



