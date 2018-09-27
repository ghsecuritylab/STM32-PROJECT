// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: ����ͨ������
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================

#include "rs485.h"
#include "delay.h"

// ============================================================================
UART_HandleTypeDef USART1_Handler;
UART_HandleTypeDef USART2_Handler;
UART_HandleTypeDef USART3_Handler;
// ============================================================================
UINT8  USART1_DatRxBuf[CN_USART1_RX_BUF_LEN];//�û�ʹ�ý������ݻ���
UINT8  USART1_DatRxBuf[CN_USART2_RX_BUF_LEN];//�û�ʹ�ý������ݻ���
UINT8  USART1_DatRxBuf[CN_USART3_RX_BUF_LEN];//�û�ʹ�ý������ݻ���
// ============================================================================
UINT16 g_wUsart1RxCnt;
UINT16 g_wUsart2RxCnt;
UINT16 g_wUsart3RxCnt;
// ============================================================================
void RS485_TX_Set(UINT8 byMode);
// ============================================================================
// ��������:��ʼ������
// �������:Bound-������
// �� �� ֵ:��
// ============================================================================
void uart_init(UINT8 byUsartNum,UINT32 dwBound)
{	
    g_wUsart1RxCnt = 0;
    g_wUsart2RxCnt = 0;
    g_wUsart3RxCnt = 0;
    
    switch(byUsartNum)
    {
        case 1:
            __HAL_RCC_USART1_CLK_ENABLE();                              //ʹ��USARTʱ��
            //UART��ʼ������
            USART1_Handler.Instance          = USART1;                   //USART
            USART1_Handler.Init.BaudRate     = dwBound;                  //������
            USART1_Handler.Init.WordLength   = UART_WORDLENGTH_8B;       //�ֳ�Ϊ8λ���ݸ�ʽ
            USART1_Handler.Init.StopBits     = UART_STOPBITS_1;          //һ��ֹͣλ
            USART1_Handler.Init.Parity       = UART_PARITY_NONE;         //����żУ��λ
            USART1_Handler.Init.HwFlowCtl    = UART_HWCONTROL_NONE;      //��Ӳ������
            USART1_Handler.Init.Mode         = UART_MODE_TX_RX;          //�շ�ģʽ
            HAL_UART_Init(&USART1_Handler);                              //ʹ��USART
            HAL_NVIC_EnableIRQ(USART1_IRQn);                            //ʹ��USART1�ж�ͨ��
            HAL_NVIC_SetPriority(USART1_IRQn,3,1);                      //��ռ���ȼ�3�������ȼ�1    
            __HAL_UART_ENABLE_IT(&USART1_Handler,UART_IT_RXNE);
            break;
        case 2:
            __HAL_RCC_USART2_CLK_ENABLE();                              //ʹ��USARTʱ��
            //UART��ʼ������
            USART2_Handler.Instance          = USART2;                   //USART
            USART2_Handler.Init.BaudRate     = dwBound;                  //������
            USART2_Handler.Init.WordLength   = UART_WORDLENGTH_8B;       //�ֳ�Ϊ8λ���ݸ�ʽ
            USART2_Handler.Init.StopBits     = UART_STOPBITS_1;          //һ��ֹͣλ
            USART2_Handler.Init.Parity       = UART_PARITY_NONE;         //����żУ��λ
            USART2_Handler.Init.HwFlowCtl    = UART_HWCONTROL_NONE;      //��Ӳ������
            USART2_Handler.Init.Mode         = UART_MODE_TX_RX;          //�շ�ģʽ
            HAL_UART_Init(&USART2_Handler);                              //ʹ��USART
            HAL_NVIC_EnableIRQ(USART2_IRQn);                            //ʹ��USART2�ж�ͨ��
            HAL_NVIC_SetPriority(USART2_IRQn,3,2);                      //��ռ���ȼ�3�������ȼ�2
            __HAL_UART_ENABLE_IT(&USART2_Handler,UART_IT_RXNE);
            break;
        case 3:
            __HAL_RCC_USART3_CLK_ENABLE();                              //ʹ��USARTʱ��
	//UART��ʼ������
            USART3_Handler.Instance          = USART3;                   //USART
            USART3_Handler.Init.BaudRate     = dwBound;                  //������
            USART3_Handler.Init.WordLength   = UART_WORDLENGTH_8B;       //�ֳ�Ϊ8λ���ݸ�ʽ
            USART3_Handler.Init.StopBits     = UART_STOPBITS_1;          //һ��ֹͣλ
            USART3_Handler.Init.Parity       = UART_PARITY_NONE;         //����żУ��λ
            USART3_Handler.Init.HwFlowCtl    = UART_HWCONTROL_NONE;      //��Ӳ������
            USART3_Handler.Init.Mode         = UART_MODE_TX_RX;          //�շ�ģʽ
            HAL_UART_Init(&USART3_Handler);                              //ʹ��USART
            HAL_NVIC_EnableIRQ(USART3_IRQn);                            //ʹ��USART3�ж�ͨ��
            HAL_NVIC_SetPriority(USART3_IRQn,3,3);                      //��ռ���ȼ�3�������ȼ�3
            __HAL_UART_ENABLE_IT(&USART3_Handler,UART_IT_RXNE);
            break;
        default:
            break;
    }
}
// ============================================================================
// ��������:����1�жϷ������
// �������:
// �� �� ֵ:
// ============================================================================
void USART1_IRQHandler(void)                	
{
    UINT8 byDataTemp; 
	
    if((__HAL_UART_GET_FLAG(&USART1_Handler, UART_FLAG_RXNE)!=RESET))  //����
    {       
         HAL_UART_Receive(&USART1_Handler,&byDataTemp,1,1000); 
         if(g_wUsart1RxCnt<CN_USART1_RX_BUF_LEN) 
	{
            USART1_DatRxBuf[g_wUsart1RxCnt] = byDataTemp;   
            g_wUsart1RxCnt++;           
         }   
         else
         {
            g_wUsart1RxCnt=0;
         }   
	}
}
// ============================================================================
// ��������:����2�жϷ������
// �������:
// �� �� ֵ:
// ============================================================================
void USART2_IRQHandler(void)                	
{
    UINT8 byDataTemp; 

    if((__HAL_UART_GET_FLAG(&USART2_Handler, UART_FLAG_RXNE)!=RESET))  //����
	{
         HAL_UART_Receive(&USART2_Handler,&byDataTemp,1,1000); 
         if(g_wUsart2RxCnt<CN_USART2_RX_BUF_LEN) 
		{
            USART1_DatRxBuf[g_wUsart2RxCnt] = byDataTemp;   
            g_wUsart2RxCnt++;           
         }   
         else
			{
            g_wUsart2RxCnt=0;
         }  
    } 
			}
// ============================================================================
// ��������:����3�жϷ������
// �������:
// �� �� ֵ:
// ============================================================================
void USART3_IRQHandler(void)                	
{ 
    UINT8 byDataTemp; 

    if((__HAL_UART_GET_FLAG(&USART3_Handler, UART_FLAG_RXNE)!=RESET))  //����
			{	
         HAL_UART_Receive(&USART3_Handler,&byDataTemp,1,1000); 
         if(g_wUsart3RxCnt<CN_USART3_RX_BUF_LEN) 
				{
            USART1_DatRxBuf[g_wUsart3RxCnt] = byDataTemp;   
            g_wUsart3RxCnt++;           
                }
				else
				{
            g_wUsart3RxCnt=0;
			}
		}
	}
// ============================================================================
// ��������:����1��������
// �������:�������׵�ַ�����͵��ֽ��� 
// �� �� ֵ:
// ============================================================================
void Usart1_Send_Data(UINT8 *pbyBuf,UINT8 byLen) 
{ 
    RS485_TX_Set(1);     //����Ϊ����ģʽ 
    HAL_UART_Transmit(&USART1_Handler,pbyBuf,byLen,1000);
    g_wUsart1RxCnt=0;      
    RS485_TX_Set(0);     //����Ϊ����ģʽ   
}
// ============================================================================
// ��������:����1��������
// �������:���ջ����׵�ַ�����������ݳ��� 
// �� �� ֵ:
// ============================================================================
void Usart1_Receive_Data(UINT8 *pbyBuf,UINT8 *pbyLen) 
{ 
  UINT8 byRxLen; 
  UINT8 byLoop; 

  byLoop  = 0; 
 *pbyLen  = 0;        
  byRxLen = g_wUsart1RxCnt; 
	
  //�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս��� 
  delay_ms(10);
  
  //���յ�������,�ҽ�������� 
  if((byRxLen == g_wUsart1RxCnt) && byRxLen)
	{
    for(byLoop=0;byLoop<byRxLen;byLoop++) 
        {
      pbyBuf[byLoop] = USART1_DatRxBuf[byLoop];   
	}
     
    //��¼�������ݳ��� 
    *pbyLen = g_wUsart1RxCnt; 
    
    //���� 
    g_wUsart1RxCnt=0;    
        }
	}
// ============================================================================
// ��������:��������RS485��˫��ʱ������/����ģʽѡ��
// �������:0,����;1,����. 
// �� �� ֵ:
// ============================================================================
void RS485_TX_Set(UINT8 byMode) 
{ 
    MACRO_PB_Out(13) = byMode;  //DE0
    MACRO_PG_Out(0 ) = byMode;  //DE0
    MACRO_PB_Out(12) = 1;       //HDPLX0
    MACRO_PG_Out(1 ) = 1;       //HDPLX0
} 
// ============================================================================

