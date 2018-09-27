// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: IIC���ʵ�����
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "softiic.h"
#include "delay.h"

// ============================================================================
//IIC���в�������
void  SoftIIC_Init(void);                   //��ʼ��IIC��IO��				 
void  SoftIIC_Start(void);				    //����IIC��ʼ�ź�
void  SoftIIC_Stop(void);	  			    //����IICֹͣ�ź�
void  SoftIIC_Ack(void);					//IIC����ACK�ź�
void  SoftIIC_NAck(void);				    //IIC������ACK�ź�
void  SoftIIC_Send_Byte(UINT8);			    //IIC����һ���ֽ�
UINT8 SoftIIC_Read_Byte(UINT8);             //IIC��ȡһ���ֽ�
UINT8 SoftIIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
// ============================================================================
// ��������:IIC��ʼ��
// �������:��
// �� �� ֵ:��
// ============================================================================
void SoftIIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   
    
    //PH4,5��ʼ������
    GPIO_Initure.Pin    = GPIO_PIN_6|GPIO_PIN_9;
    GPIO_Initure.Mode   = GPIO_MODE_OUTPUT_PP;      //�������
    GPIO_Initure.Pull   = GPIO_PULLUP;              //����
    GPIO_Initure.Speed  = GPIO_SPEED_FAST;          //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    MACRO_IIC_SDA = 1;
    MACRO_IIC_SCL = 1;  
}
// ============================================================================
// ��������:����IIC��ʼ�ź�
// �������:��
// �� �� ֵ:��
// ============================================================================
void SoftIIC_Start(void)
{
	MACRO_SDA_OUT();    
	MACRO_IIC_SDA=1;	  	  
	MACRO_IIC_SCL=1;
	delay_us(4);
 	MACRO_IIC_SDA=0;
	delay_us(4);
	MACRO_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}
// ============================================================================
// ��������:����IICֹͣ�ź�
// �������:��
// �� �� ֵ:��
// ============================================================================
void SoftIIC_Stop(void)
{
	MACRO_SDA_OUT();
	MACRO_IIC_SCL=0;
	MACRO_IIC_SDA=0;
 	delay_us(4);
	MACRO_IIC_SCL=1; 
	MACRO_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}

// ============================================================================
// ��������:�ȴ�Ӧ���źŵ���
// �������:��
// �� �� ֵ:1-ʧ��,0-�ɹ�
// ============================================================================
UINT8 SoftIIC_Wait_Ack(void)
{
	UINT8 byTimeOut;
    
	MACRO_SDA_IN();      //SDA����Ϊ����  
	MACRO_IIC_SDA=1;
    delay_us(1);	   
	MACRO_IIC_SCL=1;
    delay_us(1);	

    byTimeOut = 0;
    
	while(MACRO_READ_SDA)
	{
		byTimeOut++;
        
		if(byTimeOut>250)
		{
			SoftIIC_Stop();
			return 1;
		}
	}
	MACRO_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
// ============================================================================
// ��������:����ACKӦ��
// �������:��
// �� �� ֵ:��
// ============================================================================
void SoftIIC_Ack(void)
{
	MACRO_IIC_SCL=0;
	MACRO_SDA_OUT();
	MACRO_IIC_SDA=0;
	delay_us(2);
	MACRO_IIC_SCL=1;
	delay_us(2);
	MACRO_IIC_SCL=0;
}
// ============================================================================
// ��������:������ACKӦ��		  
// �������:��
// �� �� ֵ:��
// ============================================================================
void SoftIIC_NAck(void)
{
	MACRO_IIC_SCL=0;
	MACRO_SDA_OUT();
	MACRO_IIC_SDA=1;
	delay_us(2);
	MACRO_IIC_SCL=1;
	delay_us(2);
	MACRO_IIC_SCL=0;
}	
// ============================================================================
// ��������:IIC����һ���ֽ�
// �������:��
// �� �� ֵ:1-��Ӧ��,0-��Ӧ��(���شӻ�����Ӧ��)	
// ============================================================================
void SoftIIC_Send_Byte(UINT8 byTxData)
{                        
    UINT8 byLoop;   
    
	MACRO_SDA_OUT(); 
    
    //����ʱ�ӿ�ʼ���ݴ���
    MACRO_IIC_SCL=0;
    
    for(byLoop=0;byLoop<8;byLoop++)
    {              
        MACRO_IIC_SDA=(byTxData&0x80)>>7;
        byTxData<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		MACRO_IIC_SCL=1;
		delay_us(2); 
		MACRO_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	  
// ============================================================================
// ��������:��1���ֽ�
// �������:��
// �� �� ֵ:��
// ============================================================================
UINT8 SoftIIC_Read_Byte(UINT8 byAck)
{
	UINT8 byLoop,byRecv=0;
    
    //SDA����Ϊ����
	MACRO_SDA_IN();
    
    for(byLoop=0;byLoop<8;byLoop++)
	{
        MACRO_IIC_SCL=0; 
        delay_us(2);
		MACRO_IIC_SCL=1;
        byRecv<<=1;
        if(MACRO_READ_SDA)
        {
            byRecv++;   
        }
		delay_us(1); 
    }	
    
    if(!byAck)
    {
        SoftIIC_NAck();//����nACK
    }
    else
    {
        SoftIIC_Ack(); //����ACK   
    }
    
    return byRecv;
}
// ============================================================================

