// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: ���ڴ�ӡ������
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================

#include "usart.h"
#include "delay.h"
// ============================================================================
//ʹ��os,���������ͷ�ļ�
#if SYSTEM_SUPPORT_OS
#include "includes.h"					
#endif

// ============================================================================
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (UINT8) ch;      
	return ch;
}
#endif 

// ============================================================================
#if CN_USART1_RX_EN   //���ʹ���˽���
//����1�жϷ������,ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
UINT8  aRxBuffer[CN_BUFSIZE_RX];//HAL��ʹ�õĴ��ڽ��ջ���
UINT8  USART_RX_BUF[CN_USART_RX_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
UINT16 USART_RX_STA=0;          // ����״̬���	
                                // bit15��	������ɱ�־
                                // bit14��	���յ�0x0d
                                // bit13~0�����յ�����Ч�ֽ���Ŀ

UART_HandleTypeDef UART1_Handler; //UART���

// ============================================================================
// ��������:��ʼ��IO ����1 
// �������:Bound-������
// �� �� ֵ:��
// ============================================================================
void uart_init(UINT32 dwBound)
{	
	//UART��ʼ������
	UART1_Handler.Instance          = USART1;					//USART1
	UART1_Handler.Init.BaudRate     = dwBound;				    //������
	UART1_Handler.Init.WordLength   = UART_WORDLENGTH_8B;       //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits     = UART_STOPBITS_1;	        //һ��ֹͣλ
	UART1_Handler.Init.Parity       = UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl    = UART_HWCONTROL_NONE;      //��Ӳ������
	UART1_Handler.Init.Mode         = UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					            //ʹ��UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (UINT8 *)aRxBuffer, CN_BUFSIZE_RX);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}
// ============================================================================
// ��������:UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����,�˺����ᱻHAL_UART_Init()����
// �������:huart-���ھ��
// �� �� ֵ:��
// ============================================================================
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			        //ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			        //ʹ��USART1ʱ��
	
		GPIO_Initure.Pin        = GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode       = GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull       = GPIO_PULLUP;			//����
		GPIO_Initure.Speed      = GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate  = GPIO_AF7_USART1;	    //����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	        //��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			        //PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	        //��ʼ��PA10
		
#if CN_USART1_RX_EN
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}

}
// ============================================================================
// ��������:������ɻص�����
// �������:
// �� �� ֵ:
// ============================================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)
				{
                    USART_RX_STA |= 0x4000;
                }
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF] = aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA > (CN_USART_RX_LEN-1))
					{
                        USART_RX_STA=0;//�������ݴ���,���¿�ʼ����    
                    }
				}		 
			}
		}
	}
}
// ============================================================================
// ��������:����1�жϷ������
// �������:��
// �� �� ֵ:��
// ============================================================================
void USART1_IRQHandler(void)                	
{ 
	UINT32 dwTimeout=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif

	//����HAL���жϴ����ú���
	HAL_UART_IRQHandler(&UART1_Handler);	
	
	dwTimeout = 0;
    //�ȴ�����
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)
	{
        dwTimeout++;
        //��ʱ����
        if(dwTimeout > HAL_MAX_DELAY)
        {
            break;      
        }
	}
     
	dwTimeout = 0;
    //һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	while(HAL_UART_Receive_IT(&UART1_Handler, (UINT8 *)aRxBuffer, CN_BUFSIZE_RX) != HAL_OK)
	{
        dwTimeout++; 
        //��ʱ����
        if(dwTimeout>HAL_MAX_DELAY)
        {
            break; 
        }
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
    OSIntExit();  											 
#endif
} 
#endif	

/*�����������ֱ�Ӱ��жϿ����߼�д���жϷ������ڲ���*/
// ============================================================================
// ��������:����1�жϷ������
// �������:
// �� �� ֵ:
// ============================================================================
//void USART1_IRQHandler(void)                	
//{ 
//	UINT8 Res;
//#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
//	OSIntEnter();    
//#endif
//	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else USART_RX_STA|=0x8000;	//��������� 
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(CN_USART_RX_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}   		 
//	}
//	HAL_UART_IRQHandler(&UART1_Handler);	
//#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
//	OSIntExit();  											 
//#endif
//} 
//#endif	

