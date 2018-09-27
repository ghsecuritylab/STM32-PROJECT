// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: ����Ͳ�������
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "usmart_str.h"
#include "usmart.h"		   

// ============================================================================
// ��������:�Ա��ַ���
// �������:�ַ���1ָ��,�ַ���2ָ��
// �� �� ֵ:0-���;1-�����;
// ============================================================================
UINT8 usmart_strcmp(UINT8 *pbyStr1,UINT8 *pbyStr2)
{
	while(1)
	{   
		if(*pbyStr1 != *pbyStr2)
        {
            return 1;
        }
		if(*pbyStr1=='\0')
        {
            break;
        }
		pbyStr1++;
		pbyStr2++;
	}
	return 0;
}
// ============================================================================
// ��������:��pbyStr1������copy��pbyStr2
// �������:�ַ���1ָ��,�ַ���2ָ��
// �� �� ֵ:��
// ============================================================================
void usmart_strcopy(UINT8 *pbyStr1,UINT8 *pbyStr2)
{
	while(1)
	{										   
		*pbyStr2 = *pbyStr1;
		if(*pbyStr1=='\0')
        {
            break;
        }
		pbyStr1++;
		pbyStr2++;
	}
}
// ============================================================================
// ��������:��ȡ�ַ����ĳ���(�ֽ�)
// �������:�ַ���ָ��
// �� �� ֵ:�ַ����ĳ���	
// ============================================================================
UINT8 usmart_strlen(UINT8 *pbyStr)
{
	UINT8 byLen;
    
    byLen = 0;
    
	while(1)
	{							 
		if(*pbyStr=='\0')
        {
            break;
        }
		byLen++;
		pbyStr++;
	}
	return byLen;
}
// ============================================================================
// ��������:���ݺ���(m^n����)
// �������:
// �� �� ֵ:m^n�η�
// ============================================================================
UINT32 usmart_pow(UINT8 byM,UINT8 byN)
{
	UINT32 dwResult;

    dwResult = 1;
    
	while(byN--)
    {
        dwResult = dwResult * byM;    
    }
	return dwResult;
}
// ============================================================================
// ��������:���ַ���תΪ����,֧��16����ת��,����16������ĸ�����Ǵ�д��,�Ҹ�ʽΪ��0X��ͷ��,֧�ָ��� 
// �������:*pbyStr:�����ַ���ָ��;*pdwRes:ת����Ľ����ŵ�ַ.
// �� �� ֵ:0,�ɹ�ת�����.����,�������.
//          1,���ݸ�ʽ����
//          2,16����λ��Ϊ0
//          3,��ʼ��ʽ����
//          4,ʮ����λ��Ϊ0
// ============================================================================
UINT8 usmart_str2num(UINT8 *pbyStr,UINT32 *pdwRes)
{
	UINT8  byNum;		
	UINT8  byHexDec;	
	UINT8  byFlag;		
	UINT8 *pbyTemp;		  
	SINT32 iNum;
	UINT32 dwTemp;

    //��ʼ��
	pbyTemp  = pbyStr;
    byNum    = 0;		    // ���ֵ�λ��
	byHexDec = 10;	        // Ĭ��Ϊʮ��������
	byFlag   = 0;		    // 0,û�з��ű��;1,��ʾ����;2,��ʾ����.
   *pdwRes   = 0;           // ����.
	
	while(1)
	{
		if((*pbyTemp <= '9' && *pbyTemp >= '0')                   \
            ||((*pbyStr =='-' || *pbyStr =='+') && byNum==0)      \
            ||( *pbyTemp<='F' && *pbyTemp>='A')                   \
            ||( *pbyTemp=='X' &&  byNum==1))//�����Ϸ�
		{
			if(*pbyTemp>='A')
            {
                byHexDec = 16;  //�ַ����д�����ĸ,Ϊ16���Ƹ�ʽ.
            }
			if(*pbyStr=='-')
            {
                byFlag = 2;
                pbyStr+= 1;
            }//ƫ�Ƶ�����
			else if(*pbyStr=='+')
            {
                byFlag=1;
                pbyStr+=1;
            }//ƫ�Ƶ�����
			else 
            {
                //λ������.
                byNum++;
            }
		}
        else if(*pbyTemp=='\0')
        {
            //����������,�˳�.
            break;    
        }
		else
		{ 
            //��ȫ��ʮ���ƻ���16��������.
            return 1;              
        }
		pbyTemp++; 
	} 
    
    //���¶�λ���ַ�����ʼ�ĵ�ַ.
	pbyTemp = pbyStr;			    
	
	if(byHexDec==16)		//16��������
	{
		if(byNum < 3)
        {
            return 2;           //λ��С��3��ֱ���˳�.��Ϊ0X��ռ��2��,���0X���治������,������ݷǷ�.
        }
		if(*pbyTemp=='0' && (*(pbyTemp+1)=='X'))//������'0X'��ͷ.
		{
			pbyTemp+= 2;	//ƫ�Ƶ�������ʼ��ַ.
			byNum  -= 2;//��ȥƫ����	 
		}
        else 
        {
            return 3;//��ʼͷ�ĸ�ʽ����
        }
	}
    else if(byNum==0)
    {
        return 4;//λ��Ϊ0��ֱ���˳�.     
    }
	while(1)
	{
		if(byNum)
		{
            byNum--;
        }
		if(*pbyTemp<='9'&&*pbyTemp>='0')
		{
            dwTemp=*pbyTemp-'0';    //�õ����ֵ�ֵ
        }
		else 
		{
            dwTemp=*pbyTemp-'A'+10;             //�õ�A~F��Ӧ��ֵ       
        }
		*pdwRes += dwTemp * usmart_pow(byHexDec,byNum);		   
		pbyTemp++;
        
		if(*pbyTemp=='\0')
		{
            break;//���ݶ�������.   
        }
	}
	if(byFlag==2)//�Ǹ���?
	{	
		iNum   =-*pdwRes; 
		*pdwRes= iNum;
	}
	return 0;//�ɹ�ת��
}
// ============================================================================
// ��������:�õ�ָ����
// �������:*pbyStr:Դ�ַ���*cmdname:ָ����*nlen:ָ��������	maxlen:��󳤶�(������,ָ�����̫����)
// �� �� ֵ:0,�ɹ�;����,ʧ��.	  
// ============================================================================
UINT8 usmart_get_cmdname(UINT8 *pbyStr,UINT8*cmdname,UINT8 *nlen,UINT8 maxlen)
{
	*nlen=0;
 	while(*pbyStr!=' '&&*pbyStr!='\0') //�ҵ��ո���߽���������Ϊ������
	{
		*cmdname=*pbyStr;
		pbyStr++;
		cmdname++;
		(*nlen)++;//ͳ�������
		if(*nlen>=maxlen)
		{
            return 1;//�����ָ��
        }
	}
	*cmdname='\0';//���������
	return 0;//��������
}
// ============================================================================
// ��������:��ȡ��һ���ַ������м��кܶ�ո��ʱ�򣬴˺���ֱ�Ӻ��Կո��ҵ��ո�֮��ĵ�һ���ַ���
// �������:pbyStr:�ַ���ָ��	
// �� �� ֵ:��һ���ַ�
// ============================================================================
UINT8 usmart_search_nextc(UINT8* pbyStr)
{		   	 	
	pbyStr++;
	while(*pbyStr==' ' && pbyStr!='\0')
	{
        pbyStr++;
    }
	return *pbyStr;
} 
// ============================================================================
// ��������:��str�еõ�������
// �������:*pbyStr:Դ�ַ���ָ��*pbyFName:��ȡ���ĺ�������ָ��*pbyNum:�����Ĳ�������*rval:�Ƿ���Ҫ��ʾ����ֵ(0,����Ҫ;1,��Ҫ)
// �� �� ֵ:0,�ɹ�;����,�������.
// ============================================================================
UINT8 usmart_get_fname(UINT8 *pbyStr,UINT8 *pbyFName,UINT8 *pbyNum,UINT8 *rval)
{
	UINT8  byRes;
	UINT8  byFover;	        //�������
	UINT8  byOffset;  
	UINT8  byParmNum;
	UINT8  byTemp;
	UINT8  byFpName[6];     //void+X+'/0'
	UINT8  byFplCnt;        //��һ�������ĳ��ȼ�����
	UINT8  byPCnt;	        //����������
	UINT8  byNChar;
	UINT8 *pbyStrTemp;

    //��ʼ��
    byFover   = 0;
    byOffset  = 0; 
    byParmNum = 0;
    byFplCnt  = 0;
    byPCnt    = 0;
    byTemp    = 1;
    
	//�жϺ����Ƿ��з���ֵ
	pbyStrTemp = pbyStr;
	while(*pbyStrTemp != '\0')//û�н���
	{
		if(*pbyStrTemp!=' ' && (byPCnt&0X7F)<5)//����¼5���ַ�
		{	
			if(byPCnt==0)
			{
                byPCnt = byPCnt | 0X80;//��λ���λ,��ǿ�ʼ���շ���ֵ����
            }
			if(((byPCnt&0x7f)==4) && (*pbyStrTemp!='*'))
			{
                break;//���һ���ַ�,������*
            }
			byFpName[byPCnt&0x7f] = *pbyStrTemp;//��¼�����ķ���ֵ����
			byPCnt++;
		}
        else if(byPCnt==0X85)
        {
            break;
        }
		pbyStrTemp++; 
	} 
	if(byPCnt)//��������
	{
		byFpName[byPCnt&0x7f] = '\0';//���������
		if(usmart_strcmp(byFpName,"void")==0)
		{
            *rval = 0;//����Ҫ����ֵ
        }
		else
		{
            *rval = 1;                               //��Ҫ����ֵ
        }
		byPCnt = 0;
	} 
	byRes      = 0;
	pbyStrTemp = pbyStr;
	while(*pbyStrTemp!='(' && *pbyStrTemp!='\0') //�˴����ҵ���������������ʼλ��
	{  
		pbyStrTemp++;
		byRes++;
		if(*pbyStrTemp==' '||*pbyStrTemp=='*')
		{
			byNChar = usmart_search_nextc(pbyStrTemp);		//��ȡ��һ���ַ�
			if(byNChar!='(' && byNChar!='*')
			{
                byOffset = byRes; //�����ո��*��
            }
		}
	}	 
	pbyStrTemp=pbyStr;
	if(byOffset)
	{
        pbyStrTemp = pbyStrTemp + (byOffset + 1);//������������ʼ�ĵط�      
    }
	byRes   = 0;
	byNChar = 0;//�Ƿ������ַ�������ı�־,0�������ַ���;1�����ַ���;
	while(1)
	{
		if(*pbyStrTemp==0)
		{
			byRes = USMART_FUNCERR;//��������
			break;
		}
        else if(*pbyStrTemp=='(' && byNChar==0)
        {
            byFover++;//�����������һ��     
        }
		else if(*pbyStrTemp==')' && byNChar==0)
		{
			if(byFover)
			{
                byFover--;
            }
			else
			{
                byRes=USMART_FUNCERR;//�������,û�յ�'('
            }
			if(byFover==0)
			{
                break;//��ĩβ��,�˳�       
            }
		}
        else if(*pbyStrTemp=='"')
        {
            byNChar = !byNChar;
        }

		if(byFover==0)//��������û������
		{
			if(*pbyStrTemp!=' ')//�ո����ں�����
			{
			   *pbyFName = *pbyStrTemp;//�õ�������
				pbyFName++;
			}
		}
        else //�Ѿ��������˺�������.
		{
			if(*pbyStrTemp==',')
			{
				byTemp=1;		//ʹ������һ������
				byPCnt++;	
			}
            else if(*pbyStrTemp!=' '&&*pbyStrTemp!='(')
			{
				if(byPCnt==0&&byFplCnt<5)		//����һ��������ʱ,Ϊ�˱���ͳ��void���͵Ĳ���,�������ж�.
				{
					byFpName[byFplCnt]=*pbyStrTemp;//��¼��������.
					byFplCnt++;
				}
				byTemp++;	//�õ���Ч����(�ǿո�)
			}
			if(byFover==1&&byTemp==2)
			{
				byTemp++;		//��ֹ�ظ�����
				byParmNum++; 	//��������һ��
			}
		}
		pbyStrTemp++; 			
	}   
	if(byParmNum==1)//ֻ��1������.
	{
		byFpName[byFplCnt]='\0';//���������
		if(usmart_strcmp(byFpName,"void")==0)
		{
            byParmNum=0;//����Ϊvoid,��ʾû�в���.
        }
	}
	*pbyNum   = byParmNum;	//��¼��������
	*pbyFName = '\0';	    //���������
	return byRes;		    //����ִ�н��
}
// ============================================================================
// ��������:��str�еõ�һ�������Ĳ���
// �������:*pbyStr:Դ�ַ���ָ��*pbyFparm:�����ַ���ָ��*pbyType:�������� 0������;1���ַ���;0XFF����������
// �� �� ֵ:0,�Ѿ��޲�����;����,��һ��������ƫ����.
// ============================================================================
UINT8 usmart_get_aparm(UINT8 *pbyStr,UINT8 *pbyFparm,UINT8 *pbyType)
{
	UINT8 byRet;
	UINT8 byEnOut;
	UINT8 byType;   //Ĭ��������
	UINT8 byString; //���str�Ƿ����ڶ�

    //��ʼ��
    byRet    = 0;
	byEnOut  = 0;
	byType   = 0;   
	byString = 0;   
	
	while(1)
	{		    
		if(*pbyStr==',' && byString==0)
		{
            byEnOut=1;          //�ݻ������˳�,Ŀ����Ѱ����һ����������ʼ��ַ
        }
		if((*pbyStr==')'||*pbyStr=='\0')&&byString==0)
		{
            break;//�����˳���ʶ��
        }
		if(byType==0)//Ĭ�������ֵ�
		{
			if((*pbyStr>='0' && *pbyStr<='9')||*pbyStr=='-'||*pbyStr=='+'||(*pbyStr>='a' && *pbyStr<='f')||(*pbyStr>='A' && *pbyStr<='F')||*pbyStr=='X'||*pbyStr=='x')//���ִ����
			{
				if(byEnOut)
				{
                    break;                  //�ҵ�����һ������,ֱ���˳�.
                }
				if(*pbyStr>='a')
				{
                    *pbyFparm = *pbyStr-0X20; //Сдת��Ϊ��д
                }
				else
				{
                    *pbyFparm = *pbyStr;              //Сд�������ֱ��ֲ���
                }
				pbyFparm++;
			}
            else if(*pbyStr=='"')//�ҵ��ַ����Ŀ�ʼ��־
			{
				if(byEnOut)
				{
                    break;//�ҵ�,����ҵ�",��Ϊ������.
                }
				byType   = 1;
				byString = 1;//�Ǽ�STRING ���ڶ���
			}
            else if(*pbyStr!=' ' && *pbyStr!=',')//���ַǷ��ַ�,��������
			{
				byType=0XFF;
				break;
			}
		}
        else//string��
		{ 
			if(*pbyStr=='"')
			{
                byString=0;
            }
			if(byEnOut)
			{
                break;          //�ҵ�����һ������,ֱ���˳�.
            }
			if(byString)				//�ַ������ڶ�
			{	
				if(*pbyStr=='\\')		//����ת���(������ת���)
				{ 
					pbyStr++;			//ƫ�Ƶ�ת���������ַ�,����ʲô�ַ�,ֱ��COPY
					byRet++;
				}					
				*pbyFparm=*pbyStr;		//Сд�������ֱ��ֲ���
				pbyFparm++;
			}	
		}
		byRet++;        //ƫ��������
		pbyStr++;
	}
	*pbyFparm = '\0';	//���������
	*pbyType  = byType;	//���ز�������
	return byRet;		//���ز�������
}
// ============================================================================
// ��������:�õ�ָ����������ʼ��ַ
// �������:num:��num������,��Χ0~9.
// �� �� ֵ:�ò�������ʼ��ַ
// ============================================================================
UINT8 usmart_get_parmpos(UINT8 byNum)
{
	UINT8 byTemp;
	UINT8 byLoop;

    byTemp=0;
    
	for(byLoop=0;byLoop<byNum;byLoop++)
	{
        byTemp = byTemp + usmart_dev.plentbl[byLoop];
    }
	return byTemp;
}
// ============================================================================
// ��������:��str�еõ���������
// �������:pbyStr:Դ�ַ���;pbyArn:�����Ķ���.0��ʾ�޲��� void����
// �� �� ֵ:0,�ɹ�;����,�������.
// ============================================================================
UINT8 usmart_get_fparam(UINT8 *pbyStr,UINT8 *pbyArn)
{	
	UINT8  byLoop,byType;  
	UINT8  byNum;
	UINT8  byLen;
	UINT8  byStr[PARM_LEN+1];//�ֽڳ��ȵĻ���,�����Դ��PARM_LEN���ַ����ַ���
	UINT32 dwRes;

    byNum=0;

	for(byLoop=0;byLoop<MAX_PARM;byLoop++)
	{
        usmart_dev.plentbl[byLoop]=0;//��ղ������ȱ�
    }
	while(*pbyStr!='(')//ƫ�Ƶ�������ʼ�ĵط�
	{
		pbyStr++;											    
		if(*pbyStr=='\0')
		{
            return USMART_FUNCERR;//������������
        }
	}
	pbyStr++;//ƫ�Ƶ�"("֮��ĵ�һ���ֽ�
	while(1)
	{
		byLoop = usmart_get_aparm(pbyStr,byStr,&byType);	//�õ���һ������  
		pbyStr+= byLoop;								    //ƫ��
		switch(byType)
		{
			case 0:	//����
				if(byStr[0]!='\0')				            //���յ��Ĳ�����Ч
				{					    
					byLoop=usmart_str2num(byStr,&dwRes);	//��¼�ò���	 
					if(byLoop)
					{
                        return USMART_PARMERR;              //��������.
                    }
					*(u32*)(usmart_dev.parm+usmart_get_parmpos(byNum))=dwRes;//��¼ת���ɹ��Ľ��.
					usmart_dev.parmtype&=~(1<<byNum);	//�������
					usmart_dev.plentbl[byNum]=4;		//�ò����ĳ���Ϊ4  
					byNum++;							//��������  
					if(byNum>MAX_PARM)
					{
                        return USMART_PARMOVER;         //����̫��
                    }
				}
				break;
			case 1://�ַ���	 	
				byLen=usmart_strlen(byStr)+1;	//�����˽�����'\0'
				usmart_strcopy(byStr,&usmart_dev.parm[usmart_get_parmpos(byNum)]);//����tstr���ݵ�usmart_dev.parm[n]
				usmart_dev.parmtype|=1<<byNum;	//����ַ��� 
				usmart_dev.plentbl[byNum]=byLen;	//�ò����ĳ���Ϊlen  
				byNum++;
				if(byNum>MAX_PARM)
				{
                    return USMART_PARMOVER;//����̫��
                }
				break;
			case 0XFF://����
				return USMART_PARMERR;//��������	  
		}
		if(*pbyStr==')'||*pbyStr=='\0')
		{
            break;//�鵽������־��.
        }
	}
	*pbyArn = byNum;	//��¼�����ĸ���
	return USMART_OK;   //��ȷ�õ��˲���
}
// ============================================================================

