// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: CANͨ������
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "can.h"
#include "usart.h"
#include "delay.h"

// ============================================================================
CAN_HandleTypeDef   CAN1_Handler;   //CAN1���
CanTxMsgTypeDef     TxMessage;      //������Ϣ
CanRxMsgTypeDef     RxMessage;      //������Ϣ
// ============================================================================
// ��������:CAN��ʼ��
// �������: ������=Fpclk1/((tbs1+tbs2+1)*brp); ������:45M/((6+8+1)*6)=500Kbps
// �� �� ֵ:0-��ʼ��OK;����-��ʼ��ʧ��; 
// ============================================================================
UINT8 CAN1_Mode_Init(void)
{
    CAN_FilterConfTypeDef  CAN1_FilerConf;
    
    CAN1_Handler.Instance       = CAN1; 
    CAN1_Handler.pTxMsg         = &TxMessage;           //������Ϣ
    CAN1_Handler.pRxMsg         = &RxMessage;           //������Ϣ
    CAN1_Handler.Init.Prescaler = 6;                    //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN1_Handler.Init.Mode      = CAN_MODE_LOOPBACK;    //ģʽ���� 
    CAN1_Handler.Init.SJW       = CAN_SJW_1TQ;          //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.BS1       = CAN_BS1_8TQ;          //tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.BS2       = CAN_BS2_6TQ;          //tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TTCM      = DISABLE;              //��ʱ�䴥��ͨ��ģʽ 
    CAN1_Handler.Init.ABOM      = DISABLE;              //����Զ����߹���
    CAN1_Handler.Init.AWUM      = DISABLE;              //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN1_Handler.Init.NART      = ENABLE;               //��ֹ�����Զ����� 
    CAN1_Handler.Init.RFLM      = DISABLE;              //���Ĳ�����,�µĸ��Ǿɵ� 
    CAN1_Handler.Init.TXFP      = DISABLE;              //���ȼ��ɱ��ı�ʶ������ 
	
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK)
    {
        return 1;   //��ʼ��
    }
    
    CAN1_FilerConf.FilterIdHigh         = 0X0000;           //32λID
    CAN1_FilerConf.FilterIdLow          = 0X0000;
    CAN1_FilerConf.FilterMaskIdHigh     = 0X0000;           //32λMASK
    CAN1_FilerConf.FilterMaskIdLow      = 0X0000;  
    CAN1_FilerConf.FilterFIFOAssignment = CAN_FILTER_FIFO0; //������0������FIFO0
    CAN1_FilerConf.FilterNumber         = 0;                //������0
    CAN1_FilerConf.FilterMode           = CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale          = CAN_FILTERSCALE_32BIT;
    CAN1_FilerConf.FilterActivation     = ENABLE;           //�����˲���0
    CAN1_FilerConf.BankNumber           = 14;
	
    if(HAL_CAN_ConfigFilter(&CAN1_Handler,&CAN1_FilerConf)!=HAL_OK)
    {
        return 2;//�˲�����ʼ��
    }
	
    return 0;
}
// ============================================================================
// ��������:CAN�ײ��������������ã�ʱ�����ã��ж�����,HAL_CAN_Init()��������
// �������:hcan:CAN���
// �� �� ֵ:
// ============================================================================
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                
    __HAL_RCC_GPIOA_CLK_ENABLE();			   
	
    GPIO_Initure.Pin        = GPIO_PIN_11|GPIO_PIN_12;  
    GPIO_Initure.Mode       = GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull       = GPIO_PULLUP;              //����
    GPIO_Initure.Speed      = GPIO_SPEED_FAST;          //����
    GPIO_Initure.Alternate  = GPIO_AF9_CAN1;            //����ΪCAN1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);                 //��ʼ��
    
#if CAN1_RX0_INT_ENABLE
    //FIFO0��Ϣ�����ж�����.      
    __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);
    //CAN1->IER|=1<<1;		//FIFO0��Ϣ�����ж�����.
    //��ռ���ȼ�1�������ȼ�2
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn,1,2);   
    //ʹ���ж�
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);          
#endif	
}

#if CAN1_RX0_INT_ENABLE                         
// ============================================================================
// ��������:CAN�жϷ�����
// �������:
// �� �� ֵ:
// ============================================================================
void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN1_Handler);
}
// ============================================================================
// ��������:CAN�жϷ�����
// �������:CAN���
// �� �� ֵ:
// ============================================================================
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
    UINT8 byLoop;

    byLoop=0;
    //���¿���FIF00��Ϣ�Һ��ж�
    __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);
    
    printf("id:%d\r\n" ,CAN1_Handler.pRxMsg->StdId);
    printf("ide:%d\r\n",CAN1_Handler.pRxMsg->IDE);
    printf("rtr:%d\r\n",CAN1_Handler.pRxMsg->RTR);
    printf("len:%d\r\n",CAN1_Handler.pRxMsg->DLC);
    for(byLoop=0;byLoop<8;byLoop++)
    {
        printf("rxbuf[%d]:%d\r\n",byLoop,CAN1_Handler.pRxMsg->Data[byLoop]);
    }
}
#endif	
// ============================================================================
// ��������:����һ������
// �������:
// �� �� ֵ:0-�ɹ�;����-ʧ��;
// ============================================================================
UINT8 CAN1_Send_Msg(tagCanTxMsg *ptCanTxMsg)
{	
    UINT16 wLoop;
    
    CAN1_Handler.pTxMsg->StdId = ptCanTxMsg->dwMsgID;        //��׼��ʶ��
    CAN1_Handler.pTxMsg->ExtId = ptCanTxMsg->dwMsgID;        //��չ��ʶ��(29λ)
    CAN1_Handler.pTxMsg->IDE   = ptCanTxMsg->dwIDE;          //ʹ�ñ�׼֡-CAN_ID_STD;��չ֡-CAN_ID_EXT
    CAN1_Handler.pTxMsg->RTR   = ptCanTxMsg->dwRTR;          //����֡-CAN_RTR_DATA
    CAN1_Handler.pTxMsg->DLC   = ptCanTxMsg->dwDLC;          //���ݳ���             
    for(wLoop=0;wLoop<(ptCanTxMsg->dwDLC);wLoop++)
    {
        CAN1_Handler.pTxMsg->Data[wLoop]=ptCanTxMsg->pbyMsg[wLoop];
    }
    if(HAL_CAN_Transmit(&CAN1_Handler,10)!=HAL_OK)
    {
        return 1;  
    }
    return 0;       
}
// ============================================================================
// ��������:�������ݲ�ѯ
// �������:���ݻ�����;	 
// �� �� ֵ:0-�����ݱ��յ�;����-���յ����ݳ���;
// ============================================================================
UINT8 CAN1_Receive_Msg(UINT8 *pbyBuf)
{		   		   
 	UINT32 dwLoop;
    
    if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,0)!=HAL_OK)
    {
        return 0;//�������ݣ���ʱʱ������Ϊ0    
    }
    for(dwLoop=0;dwLoop<CAN1_Handler.pRxMsg->DLC;dwLoop++)
    {
        pbyBuf[dwLoop]=CAN1_Handler.pRxMsg->Data[dwLoop];
    }
	return CAN1_Handler.pRxMsg->DLC;	
}
// ============================================================================

