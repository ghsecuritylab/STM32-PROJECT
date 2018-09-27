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
#include "malloc.h"	   

// ============================================================================
//�ڴ��(32�ֽڶ���)
__align(32) UINT8 tabMem1Base[MEM1_MAX_SIZE];													//�ڲ�SRAM�ڴ��
__align(32) UINT8 tabMem2Base[MEM2_MAX_SIZE] __attribute__((at(0XC0000000)));					//�ⲿSDRAM�ڴ��
__align(32) UINT8 tabMem3Base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//�ڲ�CCM�ڴ��

//�ڴ�����
UINT32 tabMem1MapBase[MEM1_ALLOC_TABLE_SIZE];													//�ڲ�SRAM�ڴ��MAP
UINT32 tabMem2MapBase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0XC0000000+MEM2_MAX_SIZE)));	    //�ⲿSRAM�ڴ��MAP
UINT32 tabMem3MapBase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));	    //�ڲ�CCM�ڴ��MAP

//�ڴ�������	   
const UINT32 tabMemTblSize[SRAMBANK] = { MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,
                                         MEM3_ALLOC_TABLE_SIZE };	//�ڴ���С
const UINT32 tabMemBlkSize[SRAMBANK] = { MEM1_BLOCK_SIZE, MEM2_BLOCK_SIZE,
                                         MEM3_BLOCK_SIZE };         //�ڴ�ֿ��С
const UINT32 tabMemSumSize[SRAMBANK] = { MEM1_MAX_SIZE,MEM2_MAX_SIZE,
                                         MEM3_MAX_SIZE };           //�ڴ��ܴ�С
// ============================================================================
void   mymemset(void *,UINT8,UINT32);   //�����ڴ�
void   mymemcpy(void *,void *,UINT32);  //�����ڴ�     
void   my_mem_init(UINT8);				//�ڴ�����ʼ������(��/�ڲ�����)
UINT8  my_mem_free(UINT8,UINT32);	    //�ڴ��ͷ�(�ڲ�����)
UINT16 my_mem_perused(UINT8) ;			//����ڴ�ʹ����(��/�ڲ�����) 
UINT32 my_mem_malloc(UINT8,UINT32);	    //�ڴ����(�ڲ�����)
// ============================================================================
//�ڴ���������
tagMallocDev tMallcoDev=
{
	my_mem_init,						            //�ڴ��ʼ��
	my_mem_perused,						            //�ڴ�ʹ����
	tabMem1Base,                                    //�ڴ��
	tabMem2Base,
	tabMem3Base,			
	tabMem1MapBase,                                 //�ڴ����״̬��
	tabMem2MapBase,
	tabMem3MapBase,   
	0,0,0,  		 					            //�ڴ����δ����
};
// ============================================================================
// ��������:�����ڴ�
// �������:Ŀ�ĵ�ַ,Դ��ַ,��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
// �� �� ֵ:
// ============================================================================
void mymemcpy(void *pDes,void *pSrc,UINT32 dwLen)  
{  
    UINT8 *pbyXDes = pDes;
	UINT8 *pbyXSrc = pSrc; 
    
    while(dwLen--)
    {
        *pbyXDes++ = *pbyXSrc++;  
    }
}  
// ============================================================================
// ��������:�����ڴ�
// �������:*pBaseAdr:�ڴ��׵�ַ, bySetVal:Ҫ���õ�ֵ,count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
// �� �� ֵ:
// ============================================================================
void mymemset(void *pBaseAdr,UINT8 bySetVal,UINT32 dwCnt)  
{  
    UINT8 *pbyXS = pBaseAdr; 
    
    while(dwCnt--)
    {
        *pbyXS ++= bySetVal;  
    }
}
// ============================================================================
// ��������:�ڴ�����ʼ��  
// �������:byMemX:�����ڴ��
// �� �� ֵ:
// ============================================================================
void my_mem_init(UINT8 byMemX)  
{  
    //�ڴ�״̬����������  
    mymemset(tMallcoDev.memmap[byMemX],0,tabMemTblSize[byMemX]*4);
    
    //�ڴ�����ʼ��OK  
 	tMallcoDev.memrdy[byMemX] = 1;								
}
// ============================================================================
// ��������:��ȡ�ڴ�ʹ����
// �������:�����ڴ��
// �� �� ֵ:ʹ����(������10��,0~1000,����0.0%~100.0%)
// ============================================================================
UINT16 my_mem_perused(UINT8 byMemX)  
{  
    UINT32 dwUsedFlg=0;  
    UINT32 dwLoop;  

    for(dwLoop=0;dwLoop<tabMemTblSize[byMemX];dwLoop++)  
    {  
        if(tMallcoDev.memmap[byMemX][dwLoop])
        {
            dwUsedFlg++; 
        }
    } 
    return (dwUsedFlg*1000)/(tabMemTblSize[byMemX]);  
} 
// ============================================================================
// ��������:�ڴ����(�ڲ�����)
// �������:byMemX:�����ڴ��;dwSize:Ҫ������ڴ��С(�ֽ�)
// �� �� ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
// ============================================================================
UINT32 my_mem_malloc(UINT8 byMemX,UINT32 dwSize)  
{  
    SINT32 iOffset=0;  
    UINT32 dwNMemB;	  //��Ҫ���ڴ����  
	UINT32 dwCMemB=0; //�������ڴ����
    UINT32 dwLoop; 
    
    if(!tMallcoDev.memrdy[byMemX])
    {
        //δ��ʼ��,��ִ�г�ʼ�� 
        tMallcoDev.init(byMemX);
    }
    
    if(dwSize==0)
    {
        return 0XFFFFFFFF;//����Ҫ����
    }
    
    //��ȡ��Ҫ����������ڴ����
    dwNMemB=dwSize/tabMemBlkSize[byMemX];  	

    if(dwSize%tabMemBlkSize[byMemX])
    {
        dwNMemB++;  
    }
    
    //���������ڴ������  
    for(iOffset=tabMemTblSize[byMemX]-1;iOffset>=0;iOffset--)
    {     
		if(!tMallcoDev.memmap[byMemX][iOffset])
		{
            //�������ڴ��������
            dwCMemB++;
        }
		else 
		{
            //�����ڴ������
            dwCMemB=0;                                
        }
        
        //�ҵ�������nmemb�����ڴ��
		if(dwCMemB==dwNMemB)							
		{
            //��ע�ڴ��ǿ� 
            for(dwLoop=0;dwLoop<dwNMemB;dwLoop++)  					
            {  
                tMallcoDev.memmap[byMemX][iOffset+dwLoop]=dwNMemB;  
            }  
            //����ƫ�Ƶ�ַ  
            return (iOffset*tabMemBlkSize[byMemX]);
		}
    }  
    //δ�ҵ����Ϸ����������ڴ��  
    return 0XFFFFFFFF;
}  
// ============================================================================
// ��������:�ͷ��ڴ�(�ڲ�����) 
// �������:byMemX:�����ڴ��offset:�ڴ��ַƫ��
// �� �� ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
// ============================================================================
UINT8 my_mem_free(UINT8 byMemX,UINT32 iOffset)  
{  
    UINT32 dwLoop;  
    UINT32 dwIndex;
    UINT32 dwNMemB;

    dwIndex = 0;
    dwNMemB = 0;
    
    //δ��ʼ��,��ִ�г�ʼ��
    if(!tMallcoDev.memrdy[byMemX])
	{
		tMallcoDev.init(byMemX);    
        //δ��ʼ��  
        return 1;
    }  
    //ƫ�����ڴ����. 
    if(iOffset<tabMemSumSize[byMemX])
    {  
        dwIndex = iOffset/tabMemBlkSize[byMemX];		//ƫ�������ڴ�����  
        dwNMemB = tMallcoDev.memmap[byMemX][dwIndex];	//�ڴ������

        //�ڴ������
        for(dwLoop=0;dwLoop<dwNMemB;dwLoop++)  						
        {  
            tMallcoDev.memmap[byMemX][dwIndex+dwLoop]=0;  
        }  
        
        return 0;  
        
    }
    else 
    {
        //ƫ�Ƴ�����
        return 2;
    }
}  

// ============================================================================
// ��������:�ͷ��ڴ�(�ⲿ����) 
// �������:byMemX:�����ڴ��ptr:�ڴ��׵�ַ 
// �� �� ֵ:��
// ============================================================================
void myfree(UINT8 byMemX,void *pMemBaseAdr)  
{  
	UINT32 dwOffset;   
    
	if(pMemBaseAdr==NULL)
	{
        return;//��ַΪ0.  
    }
 	dwOffset=(UINT32)pMemBaseAdr-(UINT32)tMallcoDev.membase[byMemX];     
    //�ͷ��ڴ�     
    my_mem_free(byMemX,dwOffset);	 
}  

// ============================================================================
// ��������:�����ڴ�(�ⲿ����)
// �������:byMemX:�����ڴ��size:�ڴ��С(�ֽ�)
// �� �� ֵ:�·��䵽���ڴ��׵�ַ.
// ============================================================================
void *mymalloc(UINT8 byMemX,UINT32 dwSize)  
{  
    UINT32 dwOffset;   
    
	dwOffset=my_mem_malloc(byMemX,dwSize); 
    
    if(dwOffset==0XFFFFFFFF)
    {
        return NULL;  
    }
    else
    {
        return (void*)((UINT32)tMallcoDev.membase[byMemX] + dwOffset);  
    }
}  

// ============================================================================
// ��������:���·����ڴ�(�ⲿ����)
// �������:byMemX:�����ڴ��*pMemBaseAdr:���ڴ��׵�ַsize:Ҫ������ڴ��С(�ֽ�)
// �� �� ֵ:�·��䵽���ڴ��׵�ַ.
// ============================================================================
void *myrealloc(UINT8 byMemX,void *pMemBaseAdr,UINT32 dwSize)  
{  
    UINT32 dwOffset;    
    
    dwOffset=my_mem_malloc(byMemX,dwSize);   
    
    if(dwOffset==0XFFFFFFFF)
    {
        return NULL;     
    }
    else  
    {  									
        //�������ڴ����ݵ����ڴ�     
	    mymemcpy((void*)((UINT32)tMallcoDev.membase[byMemX]+dwOffset),pMemBaseAdr,dwSize);	
        //�ͷž��ڴ�
        myfree(byMemX,pMemBaseAdr);  	
        //�������ڴ��׵�ַ
        return (void*)((UINT32)tMallcoDev.membase[byMemX]+dwOffset);  				
    }  
}
// ============================================================================

