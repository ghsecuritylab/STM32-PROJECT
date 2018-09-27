// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: SD������(����SDIO)
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "sdcard_sdio.h"
#include "string.h"

// ============================================================================
HAL_SD_CardInfoTypedef  SDCardInfo;                         //SD����Ϣ�ṹ��
SD_HandleTypeDef        SDCARD_Handler;                     //SD�����
DMA_HandleTypeDef       SDTxDMAHandler,SDRxDMAHandler;      //SD��DMA���ͺͽ��վ��
// ============================================================================
//SD_ReadDisk/SD_WriteDisk����ר��buf,�����������������ݻ�������ַ����4�ֽڶ����ʱ��,
//��Ҫ�õ�������,ȷ�����ݻ�������ַ��4�ֽڶ����.
__align(4) UINT8 SDIO_DATA_BUFFER[CN_SDIO_DATABUF_LEN];

// ============================================================================
// ��������:SD����ʼ��
// �������:�� 
// �� �� ֵ:0-��ʼ����ȷ������ֵ-��ʼ������
// ============================================================================
UINT8 SD_Init(void)
{
    UINT8 bySDErr;
    
    //��ʼ��ʱ��ʱ�Ӳ��ܴ���400KHZ 
    SDCARD_Handler.Instance                 = SDIO;
    SDCARD_Handler.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;               //������     
    SDCARD_Handler.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;            //��ʹ��bypassģʽ��ֱ����HCLK���з�Ƶ�õ�SDIO_CK
    SDCARD_Handler.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;        //����ʱ���ر�ʱ�ӵ�Դ
    SDCARD_Handler.Init.BusWide             = SDIO_BUS_WIDE_1B;                     //1λ������
    SDCARD_Handler.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;   //�ر�Ӳ������
    SDCARD_Handler.Init.ClockDiv            = SDIO_TRANSFER_CLK_DIV;                //SD����ʱ��Ƶ�����25MHZ
    
    bySDErr=HAL_SD_Init(&SDCARD_Handler,&SDCardInfo);
    if(bySDErr!=SD_OK)
    {
        return 1;
    }
    //ʹ�ܿ�����ģʽ
    bySDErr=HAL_SD_WideBusOperation_Config(&SDCARD_Handler,SDIO_BUS_WIDE_4B);
    if(bySDErr!=SD_OK)
    {
        return 2;
    }
    return 0;
}
// ============================================================================
// ��������:SDMMC�ײ�����,ʱ��ʹ��,��������,DMA����,�˺����ᱻHAL_SD_Init()����
// �������:SD�����
// �� �� ֵ:�� 
// ============================================================================
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    GPIO_InitTypeDef  GPIO_Initure;
    DMA_HandleTypeDef TxDMAHandler,RxDMAHandler;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();   //ʹ��GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();   //ʹ��GPIODʱ��
    __HAL_RCC_DMA2_CLK_ENABLE();    //ʹ��DMA2ʱ�� 
    __HAL_RCC_SDIO_CLK_ENABLE();    //ʹ��SDIOʱ��

    //-------------------------------------------------------------------------
    //PC8,9,10,11,12
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Initure.Mode       = GPIO_MODE_AF_PP;      //���츴��
    GPIO_Initure.Pull       = GPIO_PULLUP;          //����
    GPIO_Initure.Speed      = GPIO_SPEED_HIGH;      //����
    GPIO_Initure.Alternate  = GPIO_AF12_SDIO;       //����ΪSDIO
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);             //��ʼ��
    //-------------------------------------------------------------------------
    //PD2
    GPIO_Initure.Pin=GPIO_PIN_2;
    //��ʼ��            
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     

//-------------------------------------------------------------------------
//ʹ��DMAģʽ
#if (SD_DMA_MODE==1)          
    //����SDMMC1�жϣ���ռ���ȼ�2�������ȼ�0              
    HAL_NVIC_SetPriority(SDMMC1_IRQn,2,0); 
    //ʹ��SDMMC1�ж�
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);        
    
    //-------------------------------------------------------------------------
    //���÷���DMA
    SDRxDMAHandler.Instance                 = DMA2_Stream3;
    SDRxDMAHandler.Init.Channel             = DMA_CHANNEL_4;
    SDRxDMAHandler.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    SDRxDMAHandler.Init.PeriphInc           = DMA_PINC_DISABLE;
    SDRxDMAHandler.Init.MemInc              = DMA_MINC_ENABLE;
    SDRxDMAHandler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    SDRxDMAHandler.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    SDRxDMAHandler.Init.Mode                = DMA_PFCTRL;
    SDRxDMAHandler.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
    SDRxDMAHandler.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
    SDRxDMAHandler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    SDRxDMAHandler.Init.MemBurst            = DMA_MBURST_INC4;
    SDRxDMAHandler.Init.PeriphBurst         = DMA_PBURST_INC4;

    //������DMA��SD���ķ���DMA��������
    __HAL_LINKDMA(hsd, hdmarx, SDRxDMAHandler); 
    HAL_DMA_DeInit(&SDRxDMAHandler);
    //��ʼ������DMA
    HAL_DMA_Init(&SDRxDMAHandler);              
    
    //-------------------------------------------------------------------------
    //���ý���DMA 
    SDTxDMAHandler.Instance                 = DMA2_Stream6;
    SDTxDMAHandler.Init.Channel             = DMA_CHANNEL_4;
    SDTxDMAHandler.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    SDTxDMAHandler.Init.PeriphInc           = DMA_PINC_DISABLE;
    SDTxDMAHandler.Init.MemInc              = DMA_MINC_ENABLE;
    SDTxDMAHandler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    SDTxDMAHandler.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    SDTxDMAHandler.Init.Mode                = DMA_PFCTRL;
    SDTxDMAHandler.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
    SDTxDMAHandler.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
    SDTxDMAHandler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    SDTxDMAHandler.Init.MemBurst            = DMA_MBURST_INC4;
    SDTxDMAHandler.Init.PeriphBurst         = DMA_PBURST_INC4;

     //������DMA��SD���ķ���DMA��������
    __HAL_LINKDMA(hsd, hdmatx, SDTxDMAHandler);
    HAL_DMA_DeInit(&SDTxDMAHandler);
    //��ʼ������DMA 
    HAL_DMA_Init(&SDTxDMAHandler);              
  
    //����DMA�ж����ȼ�
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 3, 0);  
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    //����DMA�ж����ȼ�
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 3, 0);  
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
#endif
//-------------------------------------------------------------------------
}
// ============================================================================
// ��������:��ȡ����Ϣ
// �������:����Ϣ�洢��
// �� �� ֵ:����״̬
// ============================================================================
UINT8 SD_GetCardInfo(HAL_SD_CardInfoTypedef *cardinfo)
{
    UINT8 bySta;
    
    bySta = HAL_SD_Get_CardInfo(&SDCARD_Handler,cardinfo);
    return bySta;
}
// ============================================================================
//DMAģʽ
#if (SD_DMA_MODE==1)        
// ============================================================================
// ��������:ͨ��DMA��ȡSD��һ������
// �������:buf:�����ݻ�����sector:������ַblocksize:������С(һ�㶼��512�ֽ�)cnt:��������	
// �� �� ֵ:0-����;����-�������;
// ============================================================================
UINT8 SD_ReadBlocks_DMA(UINT32 *pdwBuf,UINT64 uSector,UINT32 dwBlockSize,UINT32 dwCnt)
{
    UINT8 byErr;

    byErr = 0;
    
    //ͨ��DMA��ȡSD��һ������
    byErr = HAL_SD_ReadBlocks_DMA(&SDCARD_Handler,pdwBuf,uSector,dwBlockSize,dwCnt);
    //��ȡ�ɹ�
    if(byErr==0)
    {
        //�ȴ���ȡ���
        byErr = HAL_SD_CheckReadOperation(&SDCARD_Handler,(UINT32)SD_TIMEOUT);
    }

    return byErr;
}
// ============================================================================
// ��������:дSD��
// �������:buf:д���ݻ�����sector:������ַblocksize:������С(һ�㶼��512�ֽ�)cnt:��������	
// �� �� ֵ:0-����;����-�������;
// ============================================================================
UINT8 SD_WriteBlocks_DMA(UINT32 *pdwBuf,UINT64 uSector,UINT32 dwBlockSize,UINT32 dwCnt)
{
    UINT8 byErr;

    byErr = 0;
    
    //ͨ��DMAдSD��һ������
    byErr=HAL_SD_WriteBlocks_DMA(&SDCARD_Handler,pdwBuf,uSector,dwBlockSize,dwCnt);
    
    if(byErr==0)
    {  
        //�ȴ���ȡ���
        byErr=HAL_SD_CheckWriteOperation(&SDCARD_Handler,(UINT32)SD_TIMEOUT);
    }
    return byErr;
}
// ============================================================================
// ��������:��SD��
// �������:buf:�����ݻ�����sector:������ַcnt:��������	
// �� �� ֵ:0-����;����-�������;
// ============================================================================
UINT8 SD_ReadDisk(UINT8* pbyBuf,UINT32 dwSector,UINT32 dwCnt)
{
    UINT8 bySta;
    UINT8 byLoop;
    UINT64 uSector;

    bySta   = SD_OK;
    uSector = (UINT64)dwSector;
    
    if(SDCardInfo.CardType!=STD_CAPACITY_SD_CARD_V1_1)
    {
        uSector<<=9;
    }
    if((UINT32)pbyBuf%4!=0)
    {
        for(byLoop=0;byLoop<dwCnt;byLoop++)
        {
            bySta=SD_ReadBlocks_DMA((UINT32*)SDIO_DATA_BUFFER,uSector+CN_SDIO_DATABUF_LEN*byLoop,CN_SDIO_DATABUF_LEN,1);
            memcpy(pbyBuf,SDIO_DATA_BUFFER,CN_SDIO_DATABUF_LEN);
            pbyBuf += CN_SDIO_DATABUF_LEN;
        }
    }
    else
    {
        bySta=SD_ReadBlocks_DMA((UINT32*)pbyBuf,uSector, CN_SDIO_DATABUF_LEN,dwCnt);
    }
    return bySta;
}  
// ============================================================================
// ��������:дSD��
// �������:buf:д���ݻ�����sector:������ַcnt:��������	
// �� �� ֵ:0-����;����-�������;
// ============================================================================
UINT8 SD_WriteDisk(UINT8 *pbyBuf,UINT32 dwSector,UINT32 dwCnt)
{   
    UINT8 bySta;
    UINT8 byLoop;
    UINT64 uSector;

    bySta  =SD_OK;
    uSector=(UINT64)dwSector;
    
    if(SDCardInfo.CardType!=STD_CAPACITY_SD_CARD_V1_1)
    {
        uSector<<=9;
    }
    if((UINT32)pbyBuf%4!=0)
    {
        for(byLoop=0;byLoop<dwCnt;byLoop++)
        {
            memcpy(SDIO_DATA_BUFFER,pbyBuf,CN_SDIO_DATABUF_LEN);
            //����sector��д����
            bySta=SD_WriteBlocks_DMA((UINT32*)SDIO_DATA_BUFFER,uSector+CN_SDIO_DATABUF_LEN*byLoop,CN_SDIO_DATABUF_LEN,1);
            pbyBuf+=CN_SDIO_DATABUF_LEN;
        }
    }
    else
    {
        //���sector��д����
        bySta=SD_WriteBlocks_DMA((UINT32*)pbyBuf,uSector,CN_SDIO_DATABUF_LEN,dwCnt);
    }
    return bySta;
} 
// ============================================================================
// ��������:SDMMC1�жϷ�����
// �������:��
// �� �� ֵ:��
// ============================================================================
void SDMMC1_IRQHandler(void)
{
    HAL_SD_IRQHandler(&SDCARD_Handler);
}
// ============================================================================
// ��������:DMA2������6�жϷ�����
// �������:��
// �� �� ֵ:��
// ============================================================================
void DMA2_Stream6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(SDCARD_Handler.hdmatx);
}
// ============================================================================
// ��������:DMA2������3�жϷ�����
// �������:��
// �� �� ֵ:��
// ============================================================================
void DMA2_Stream3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(SDCARD_Handler.hdmarx);
}
#else                                   //��ѵģʽ
// ============================================================================
// ��������:��SD��
// �������:buf:�����ݻ�����sector:������ַcnt:��������	
// �� �� ֵ:0,����;����,�������;
// ============================================================================
UINT8 SD_ReadDisk(UINT8* pbyBuf,UINT32 dwSector,UINT32 dwCnt)
{
    UINT8  bySta;
    UINT8  byLoop;
    UINT64 uSector;

    bySta  =SD_OK;
    uSector=(UINT64)dwSector;
    uSector<<=9;
    
    //�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
    INTX_DISABLE();
    if((UINT32)pbyBuf%4!=0)
    {
        for(byLoop=0;byLoop<dwCnt;byLoop++)
        {
            bySta=HAL_SD_ReadBlocks(&SDCARD_Handler,(UINT32*)SDIO_DATA_BUFFER,uSector+CN_SDIO_DATABUF_LEN*byLoop,CN_SDIO_DATABUF_LEN,1);//����sector�Ķ�����
            memcpy(pbyBuf,SDIO_DATA_BUFFER,CN_SDIO_DATABUF_LEN);
            pbyBuf+=CN_SDIO_DATABUF_LEN;
        }
    }
    else
    {
        bySta=HAL_SD_ReadBlocks(&SDCARD_Handler,(UINT32*)pbyBuf,uSector,CN_SDIO_DATABUF_LEN,dwCnt);//����sector�Ķ�����
    }
    //�������ж�
    INTX_ENABLE();
    return bySta;
}  
// ============================================================================
// ��������:дSD��
// �������:buf:д���ݻ�����sector:������ַcnt:��������	
// �� �� ֵ:0,����;����,�������;
// ============================================================================
UINT8 SD_WriteDisk(UINT8 *pbyBuf,UINT32 dwSector,UINT32 dwCnt)
{   
    UINT8  bySta;
    UINT8  byLoop;
    UINT64 uSector;

    bySta  =SD_OK;
    uSector=(UINT64)dwSector;
    uSector<<=9;
    //�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
    INTX_DISABLE();
    if((UINT32)pbyBuf%4!=0)
    {
        for(byLoop=0;byLoop<dwCnt;byLoop++)
        {
            memcpy(SDIO_DATA_BUFFER,pbyBuf,CN_SDIO_DATABUF_LEN);
            //����sector��д����
            bySta=HAL_SD_WriteBlocks(&SDCARD_Handler,(UINT32*)SDIO_DATA_BUFFER,uSector+CN_SDIO_DATABUF_LEN*byLoop,CN_SDIO_DATABUF_LEN,1);
            pbyBuf+=CN_SDIO_DATABUF_LEN;
        }
    }
    else
    {
        //���sector��д����
        bySta=HAL_SD_WriteBlocks(&SDCARD_Handler,(UINT32*)pbyBuf,uSector,CN_SDIO_DATABUF_LEN,dwCnt);
    }
    //�������ж�
	INTX_ENABLE();
    return bySta;
}
#endif
// ============================================================================

