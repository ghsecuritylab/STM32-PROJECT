// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: SDRAM����
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "sdram.h"
#include "delay.h"

SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM���
// ============================================================================
//����ں�������
void  SDRAM_Init_Sequence(SDRAM_HandleTypeDef *hsdram);
UINT8 SDRAM_Send_Cmd(UINT8 byBankX,UINT8 byCmd,UINT8 byRefresh,UINT16 wRegVal);
// ============================================================================
// ��������:SDRAM��ʼ��
// �������:��
// �� �� ֵ:��
// ============================================================================
void SDRAM_Init(void)
{
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;

    SDRAM_Timing.LoadToActiveDelay    = 2;  //����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������
    SDRAM_Timing.ExitSelfRefreshDelay = 6;  //�˳���ˢ���ӳ�Ϊ6��ʱ������
    SDRAM_Timing.SelfRefreshTime      = 4;  //��ˢ��ʱ��Ϊ4��ʱ������                                 
    SDRAM_Timing.RowCycleDelay        = 6;  //��ѭ���ӳ�Ϊ6��ʱ������
    SDRAM_Timing.WriteRecoveryTime    = 2;  //�ָ��ӳ�Ϊ2��ʱ������
    SDRAM_Timing.RPDelay              = 2;  //��Ԥ����ӳ�Ϊ2��ʱ������
    SDRAM_Timing.RCDDelay             = 2;  //�е����ӳ�Ϊ2��ʱ������
                                                     
    SDRAM_Handler.Instance                = FMC_SDRAM_DEVICE;                         
    SDRAM_Handler.Init.SDBank             = FMC_SDRAM_BANK1;                   //��һ��SDRAM BANK
    SDRAM_Handler.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;       //������
    SDRAM_Handler.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;         //������
    SDRAM_Handler.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32;        //���ݿ��Ϊ32λ
    SDRAM_Handler.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;      //һ��4��BANK
    SDRAM_Handler.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;           //CASΪ3
    SDRAM_Handler.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;//ʧ��д����
    SDRAM_Handler.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;          //SDRAMʱ��ΪHCLK/2=180M/2=90M=11.1ns
    SDRAM_Handler.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;           //ʹ��ͻ��
    SDRAM_Handler.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;           //��ͨ����ʱ
    
    HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);

    //����SDRAM��ʼ������	
    SDRAM_Init_Sequence(&SDRAM_Handler);
		
	//ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
	//COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
    //����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=180/2=90Mhz,����Ϊ8192(2^13).
	//����,COUNT=64*1000*90/8192-20=683
	HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler,683);//����ˢ��Ƶ��

}
// ============================================================================
// ��������:����SDRAM��ʼ������
// �������:��
// �� �� ֵ:��
// ============================================================================
void SDRAM_Init_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    UINT32 dwTemp;

    //��ʼ��
    dwTemp = 0;
    
    //ʱ������ʹ��
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_CLK_ENABLE,1,0); 
    //������ʱ200us
    delay_us(500);  
    //�����д洢��Ԥ���
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_PALL,1,0);   
    //������ˢ�´��� 
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);   

    //����ģʽ�Ĵ���,SDRAM��bit0~bit2Ϊָ��ͻ�����ʵĳ��ȣ�
	                      //bit3Ϊָ��ͻ�����ʵ����ͣ�
	                      //bit4~bit6ΪCASֵ
	                      //bit7~bit8Ϊ����ģʽ
	                      //bit9Ϊָ����дͻ��ģʽ
	                      //bit10~bit11λ����λ
	dwTemp = (UINT32)SDRAM_MODEREG_BURST_LENGTH_1   |	//����ͻ������:1(������1/2/4/8)
              SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	//����ͻ������:����(����������/����)
              SDRAM_MODEREG_CAS_LATENCY_3           |	//����CASֵ:3(������2/3)
              SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //���ò���ģʽ:0,��׼ģʽ
              SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //����ͻ��дģʽ:1,�������

    //����SDRAM��ģʽ�Ĵ���
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,dwTemp); 
}

// ============================================================================
// ��������:SDRAM�ײ��������������ã�ʱ��ʹ��
// �������:SDRAM���
// �� �� ֵ:��
// ============================================================================
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef GPIO_Initure;

    /* Enable GPIO clocks */
    __HAL_RCC_GPIOD_CLK_ENABLE();            
    __HAL_RCC_GPIOE_CLK_ENABLE();            
    __HAL_RCC_GPIOF_CLK_ENABLE();            
    __HAL_RCC_GPIOG_CLK_ENABLE();            
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE(); 

    /* Enable FMC clock */
    __HAL_RCC_FMC_CLK_ENABLE(); 
    
    GPIO_Initure.Mode      = GPIO_MODE_AF_PP;        
    GPIO_Initure.Pull      = GPIO_PULLUP;            
    GPIO_Initure.Speed     = GPIO_SPEED_HIGH;        
    GPIO_Initure.Alternate = GPIO_AF12_FMC;          

    /* GPIOD configuration */  
    GPIO_Initure.Pin = GPIO_PIN_0 |GPIO_PIN_1  |GPIO_PIN_8 |GPIO_PIN_9 |GPIO_PIN_10|\
                       GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     

    /* GPIOE configuration */  
    GPIO_Initure.Pin = GPIO_PIN_0 |GPIO_PIN_1  |GPIO_PIN_7 |GPIO_PIN_8 |GPIO_PIN_9 |\
                       GPIO_PIN_10| GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|\
                       GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);     

    /* GPIOF configuration */  
    GPIO_Initure.Pin = GPIO_PIN_0 |GPIO_PIN_1  |GPIO_PIN_2 |GPIO_PIN_3 |GPIO_PIN_4 |\
                       GPIO_PIN_5 |GPIO_PIN_11 |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|\
                       GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);     

    /* GPIOG configuration */  
    GPIO_Initure.Pin = GPIO_PIN_0 |GPIO_PIN_1  |GPIO_PIN_2 |GPIO_PIN_4 |GPIO_PIN_5 |\
                       GPIO_PIN_8 |GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);      

    /* GPIOH configuration */  
    GPIO_Initure.Pin = GPIO_PIN_2 |GPIO_PIN_3  |GPIO_PIN_5 |GPIO_PIN_8 |GPIO_PIN_9 |\
                       GPIO_PIN_10|GPIO_PIN_11 |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|\
                       GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH, &GPIO_Initure); 

    /* GPIOI configuration */  
    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_2 | GPIO_PIN_3|GPIO_PIN_4 |\
                       GPIO_PIN_5 | GPIO_PIN_6 |GPIO_PIN_7 | GPIO_PIN_9|GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);
}

// ============================================================================
// ��������:��SDRAM��������
// �������:byBankX:0,��BANK5�����SDRAM����ָ��;1,��BANK6�����SDRAM����ָ��
// �� �� ֵ:0,����;1,ʧ��.
// ============================================================================
//byCmd:ָ��(0,����ģʽ/1,ʱ������ʹ��/2,Ԥ������д洢��/3,�Զ�ˢ��/4,����ģʽ�Ĵ���/5,��ˢ��/6,����)
//byRefresh:��ˢ�´���
//wRegVal:ģʽ�Ĵ����Ķ���
UINT8 SDRAM_Send_Cmd(UINT8 byBankX,UINT8 byCmd,UINT8 byRefresh,UINT16 wRegVal)
{
    UINT32 dwTargetBank;
    FMC_SDRAM_CommandTypeDef tCmd;

    dwTargetBank = 0;
    
    if(byBankX==0)
    {
        dwTargetBank = FMC_SDRAM_CMD_TARGET_BANK1;       
    }
    else if(byBankX==1)
    {
        dwTargetBank = FMC_SDRAM_CMD_TARGET_BANK2;   
    }
    
    tCmd.CommandMode            = byCmd;             //����
    tCmd.CommandTarget          = dwTargetBank;      //Ŀ��SDRAM�洢����
    tCmd.AutoRefreshNumber      = byRefresh;         //��ˢ�´���
    tCmd.ModeRegisterDefinition = wRegVal;           //Ҫд��ģʽ�Ĵ�����ֵ

    //��SDRAM��������
    if(HAL_SDRAM_SendCommand(&SDRAM_Handler,&tCmd,0X1000)==HAL_OK) 
    {
        return 0;  
    }
    else 
    {
        return 1;    
    }
}
// ============================================================================
// ��������:��ָ����ַ(dwWriteAdr+Bank5_SDRAM_ADDR)��ʼ,����д��dwNum���ֽ�.
// �������:pbyBuf:�ֽ�ָ��;WriteAddr:Ҫд��ĵ�ַ;dwNum:Ҫд����ֽ���
// �� �� ֵ:��
// ============================================================================
void FMC_SDRAM_WrBuf(UINT8 *pbyBuf,UINT32 dwWriteAdr,UINT32 dwNum)
{
	for(;dwNum!=0;dwNum--)
	{
		*(VUINT8*)(Bank5_SDRAM_ADDR+dwWriteAdr) = *pbyBuf;
		dwWriteAdr++;
		pbyBuf++;
	}
}
// ============================================================================
// ��������:��ָ����ַ((dwWriteAdr+Bank5_SDRAM_ADDR))��ʼ,��������dwNum���ֽ�.
// �������:pbyBuf:�ֽ�ָ��;ReadAddr:Ҫ��������ʼ��ַ;dwNum:Ҫд����ֽ���
// �� �� ֵ:��
// ============================================================================
void FMC_SDRAM_RdBuf(UINT8 *pbyBuf,UINT32 dwReadAdr,UINT32 dwNum)
{
	for(;dwNum!=0;dwNum--)
	{
		*pbyBuf++ = *(VUINT8*)( Bank5_SDRAM_ADDR + dwReadAdr );
		dwReadAdr++;
	}
}
// ============================================================================

