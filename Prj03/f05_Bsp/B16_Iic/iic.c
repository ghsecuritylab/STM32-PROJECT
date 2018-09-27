// ============================================================================
// Copyright (C) 2017-2018  All Rights Reserved
// ģ������: IICʵ�����
// ģ��汾: V1.00
// ������Ա: GuXY
// ����ʱ��: 2017-04-18
// ============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "iic.h"
#include "delay.h"

// ============================================================================
#define I2C1_ADDRESS1           ( 0x3A       )
#define I2C1_SPEED              ( 100000     )                
// ============================================================================
I2C_HandleTypeDef I2C1_Handler;
// ============================================================================
//IIC���в�������
void  IIC1_MspInit(void);                   //��ʼ��IIC��IO��				 
// ============================================================================
// ��������:IIC��ʼ��
// �������:��
// �� �� ֵ:��
// ============================================================================
void IIC1_Init(void)
{
    if(HAL_I2C_GetState(&I2C1_Handler) == HAL_I2C_STATE_RESET)
    {
        I2C1_Handler.Instance             = I2C1;
        I2C1_Handler.Init.ClockSpeed      = I2C1_SPEED;
        I2C1_Handler.Init.DutyCycle       = I2C_DUTYCYCLE_2;
        I2C1_Handler.Init.OwnAddress1     = I2C1_ADDRESS1;
        I2C1_Handler.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
        I2C1_Handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        I2C1_Handler.Init.OwnAddress2     = 0;
        I2C1_Handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        I2C1_Handler.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

        IIC1_MspInit();
        HAL_I2C_Init(&I2C1_Handler);
    }
}
// ============================================================================
// ��������:��ʼ��IIC��IO��			
// �������:��
// �� �� ֵ:��
// ============================================================================
void IIC1_MspInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;  
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStruct.Pin       = GPIO_PIN_6;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Enable I2C clock */
    __HAL_RCC_I2C1_CLK_ENABLE();
    
    /* Force the I2C peripheral clock reset */  
    __HAL_RCC_I2C1_FORCE_RESET(); 

    /* Release the I2C peripheral clock reset */  
    __HAL_RCC_I2C1_RELEASE_RESET(); 

    /* Enable and set I2Cx Interrupt to a lower priority */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

    /* Enable and set I2Cx Interrupt to a lower priority */
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
}
// ============================================================================
// ��������:IIC1д����
// �������:��
// �� �� ֵ:��
// ============================================================================
void IIC1_Write_Byte(UINT8 byAddr, UINT8 byReg, UINT8 byValue)
{
    HAL_I2C_Mem_Write(&I2C1_Handler, byAddr, (UINT16)byReg, I2C_MEMADD_SIZE_8BIT, &byValue, 1, 100); 
}
// ============================================================================
// ��������:IIC1������
// �������:��
// �� �� ֵ:����ȡ��ֵ
// ============================================================================
UINT8 IIC1_Read_Byte(UINT8 byAddr, UINT8 byReg)
{
    UINT8 byValue=0;
    
    HAL_I2C_Mem_Read(&I2C1_Handler, byAddr, (UINT16)byReg, I2C_MEMADD_SIZE_8BIT, &byValue, 1, 1000); 

    return byValue;
}
// ============================================================================
// ��������:IIC1д�������
// �������:��
// �� �� ֵ:��
// ============================================================================
void IIC1_Write_Bytes(UINT8 byAddr, UINT8 byReg, UINT16 wMemAdr,UINT8 *pbyBuf,UINT16 wLen)
{
    HAL_I2C_Mem_Write(&I2C1_Handler, byAddr, (UINT16)byReg, wMemAdr, pbyBuf, wLen, 1000);
}
// ============================================================================
// ��������:IIC1���������
// �������:��
// �� �� ֵ:����ȡ��ֵ
// ============================================================================
void IIC1_Read_Bytes(UINT8 byAddr, UINT8 byReg,UINT16 wMemAdr,UINT8 *pbyBuf,UINT16 wLen)
{
    HAL_I2C_Mem_Read(&I2C1_Handler, byAddr, (UINT16)byReg, wMemAdr, pbyBuf, wLen, 1000);
}
// ============================================================================

