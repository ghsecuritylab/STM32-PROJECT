// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: ethernetͨ��PHY����
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "phy.h"
#include "delay.h"
#include "malloc.h"
#include "lwip_comm.h"

// ============================================================================
UINT8 *g_pbyRxBuf; 					//��̫���ײ���������buffersָ�� 
UINT8 *g_pbyTxBuf; 					//��̫���ײ���������buffersָ��
ETH_HandleTypeDef   ETH_Handler;    //��̫�����
ETH_DMADescTypeDef *DMARxDscrTab;	//��̫��DMA�������������ݽṹ��ָ��
ETH_DMADescTypeDef *DMATxDscrTab;	//��̫��DMA�������������ݽṹ��ָ�� 

// ============================================================================
// ��������:PHYоƬ��ʼ�� 
// �������:��
// �� �� ֵ:0,�ɹ�;����,ʧ��
// ============================================================================
UINT8 PHY_Init(void)
{      
    UINT8 byMACAddr[6];
    
    INTX_DISABLE();                         //�ر������жϣ���λ���̲��ܱ���ϣ�
//    PCF8574_WriteBit(ETH_RESET_IO,1);       //Ӳ����λ
    delay_ms(100);
//    PCF8574_WriteBit(ETH_RESET_IO,0);       //��λ����
    delay_ms(100);
    INTX_ENABLE();                          //���������ж�

    byMACAddr[0] = lwipdev.mac[0]; 
	byMACAddr[1] = lwipdev.mac[1]; 
	byMACAddr[2] = lwipdev.mac[2];
	byMACAddr[3] = lwipdev.mac[3];   
	byMACAddr[4] = lwipdev.mac[4];
	byMACAddr[5] = lwipdev.mac[5];
        
	ETH_Handler.Instance             = ETH;
    ETH_Handler.Init.MACAddr         = byMACAddr;                   //MAC��ַ  
    ETH_Handler.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;  //ʹ����Э��ģʽ 
    ETH_Handler.Init.Speed           = ETH_SPEED_100M;              //�ٶ�100M,�����������Э��ģʽ�������þ���Ч
    ETH_Handler.Init.DuplexMode      = ETH_MODE_FULLDUPLEX;         //ȫ˫��ģʽ�������������Э��ģʽ�������þ���Ч
    ETH_Handler.Init.MediaInterface  = ETH_MEDIA_INTERFACE_MII;     //MII�ӿ�  
    ETH_Handler.Init.RxMode          = ETH_RXPOLLING_MODE;          //��ѯ����ģʽ 
    ETH_Handler.Init.ChecksumMode    = ETH_CHECKSUM_BY_HARDWARE;    //Ӳ��֡У��  
    ETH_Handler.Init.PhyAddress      = DP83848_PHY_ADDRESS;         //PHYоƬ��ַ  

    if(HAL_ETH_Init(&ETH_Handler)==HAL_OK)
    {
        return 0;   //�ɹ�
    }
    else
    {
        return 1;  //ʧ��
    }
}

// ============================================================================
// ��������:ETH�ײ�������ʱ��ʹ�ܣ���������,�˺����ᱻHAL_ETH_Init()����
// �������:heth:��̫�����
// �� �� ֵ:
// ============================================================================
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable GPIOs clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE(); 

       
    /*������������ MII�ӿ� 
    ETH_MII_TX_CLK -------------------> PC3
    ETH_MII_TX_EN --------------------> PG11
    ETH_MII_TXD0 ---------------------> PG13
    ETH_MII_TXD1 ---------------------> PG14
    ETH_MII_TXD2 ---------------------> PC2
    ETH_MII_TXD3 ---------------------> PB8
    ETH_MII_RXD0 ---------------------> PC4
    ETH_MII_RXD1 ---------------------> PC5
    ETH_MII_RXD2 ---------------------> PH6
    ETH_MII_RXD3 ---------------------> PH7
    ETH_MII_RX_ER --------------------> PI10 (not configured)        
    ETH_MII_RX_DV --------------------> PA7
    ETH_MII_RX_CLK -------------------> PA1
    ETH_MII_COL ----------------------> PH3  (not configured)
    ETH_MII_CRS ----------------------> PA0  (not configured)
    ETH_MDC --------------------------> PC1
    ETH_MDIO -------------------------> PA2
    
    ETH_PPS_OUT ----------------------> PB5  (not configured)
    */
    /* Configure PA1, PA2 and PA7 */
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL; 
    GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PB5 and PB8 */
    GPIO_InitStructure.Pin = GPIO_PIN_5 | GPIO_PIN_8;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* Configure PC1, PC2, PC3, PC4 and PC5 */
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PG11, PG14 and PG13 */
    GPIO_InitStructure.Pin =  GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
    
    /* Configure PH6, PH7 */
    GPIO_InitStructure.Pin =  GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

    /* Enable ETHERNET clock  */
    __HAL_RCC_ETH_CLK_ENABLE();
      
//    HAL_NVIC_SetPriority(ETH_IRQn,1,0);         //�����ж����ȼ�Ӧ�ø�һ��
//    HAL_NVIC_EnableIRQ(ETH_IRQn);
}
// ============================================================================
// ��������:��ȡPHY�Ĵ���ֵ
// �������:
// �� �� ֵ:
// ============================================================================
UINT32 PHY_ReadPHY(UINT16 wReg)
{
    UINT32 dwRegVal;
    
    HAL_ETH_ReadPHYRegister(&ETH_Handler,wReg,&dwRegVal);
    
    return dwRegVal;
}
// ============================================================================
// ��������:��PHYоƬָ���Ĵ���д��ֵ
// �������:reg:Ҫд��ļĴ���value:Ҫд���ֵ
// �� �� ֵ:
// ============================================================================
void PHY_WritePHY(UINT16 wReg,UINT16 wValue)
{
    UINT32 dwTemp;

    dwTemp = (UINT32)wValue;
    
    HAL_ETH_ReadPHYRegister(&ETH_Handler,wReg,&dwTemp);
}
// ============================================================================
// ��������:�õ�PHYоƬ���ٶ�ģʽ
// �������:
// �� �� ֵ:001:10M��˫��101:10Mȫ˫��010:100M��˫��110:100Mȫ˫��,����:����.
// ============================================================================
UINT8 PHY_Get_Speed(void)
{
	UINT8 bySpeed;
    
	bySpeed = ((PHY_ReadPHY(31)&0x1C)>>2); //��PHY��31�żĴ����ж�ȡ�����ٶȺ�˫��ģʽ

    return bySpeed;
}

extern void lwip_pkt_handle(void);		//��lwip_comm.c���涨��
// ============================================================================
// ��������:�жϷ�����
// �������:
// �� �� ֵ:
// ============================================================================
void ETH_IRQHandler(void)
{
    while(ETH_GetRxPktSize(ETH_Handler.RxDesc))   
    {
        lwip_pkt_handle();//������̫�����ݣ����������ύ��LWIP
    }
    //����жϱ�־λ
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_NIS);    //���DMA�жϱ�־λ
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler,ETH_DMA_IT_R);      //���DMA�����жϱ�־λ
}
// ============================================================================
// ��������:��ȡ���յ���֡����
// �������:DMARxDesc:����DMA������
// �� �� ֵ:���յ���֡����
// ============================================================================
UINT32  ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDesc)
{
    UINT32 dwFrameLength;

    dwFrameLength = 0;
    
    if(((DMARxDesc->Status&ETH_DMARXDESC_OWN)==(uint32_t)RESET) &&
      (( DMARxDesc->Status&ETH_DMARXDESC_ES )==(uint32_t)RESET) &&
      (( DMARxDesc->Status&ETH_DMARXDESC_LS )!=(uint32_t)RESET)) 
    {
        dwFrameLength=((DMARxDesc->Status&ETH_DMARXDESC_FL)>>ETH_DMARXDESC_FRAME_LENGTHSHIFT);
    }
    return dwFrameLength;
}
// ============================================================================
// ��������:ΪETH�ײ����������ڴ�
// �������:
// �� �� ֵ:0,����;����,ʧ��
// ============================================================================
UINT8 ETH_Mem_Malloc(void)
{ 
	DMARxDscrTab = mymalloc(SRAMIN,ETH_RXBUFNB*sizeof(ETH_DMADescTypeDef));//�����ڴ�
	DMATxDscrTab = mymalloc(SRAMIN,ETH_TXBUFNB*sizeof(ETH_DMADescTypeDef));//�����ڴ�  
	g_pbyRxBuf = mymalloc(SRAMIN,ETH_RX_BUF_SIZE*ETH_RXBUFNB);	//�����ڴ�
	g_pbyTxBuf = mymalloc(SRAMIN,ETH_TX_BUF_SIZE*ETH_TXBUFNB);	//�����ڴ�
	
	if(!(UINT32)&DMARxDscrTab||!(UINT32)&DMATxDscrTab||!(UINT32)&g_pbyRxBuf||!(UINT32)&g_pbyTxBuf)
	{
		ETH_Mem_Free();
		return 1;	//����ʧ��
	}	
	return 0;		//����ɹ�
}
// ============================================================================
// ��������:�ͷ�ETH �ײ�����������ڴ�
// �������:
// �� �� ֵ:
// ============================================================================
void ETH_Mem_Free(void)
{ 
	myfree(SRAMIN,DMARxDscrTab);//�ͷ��ڴ�
	myfree(SRAMIN,DMATxDscrTab);//�ͷ��ڴ�
	myfree(SRAMIN,g_pbyRxBuf);	//�ͷ��ڴ�
	myfree(SRAMIN,g_pbyTxBuf);	//�ͷ��ڴ�  
}
// ============================================================================

