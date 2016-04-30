/*
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   ��Ҫ�жϷ�������Main Interrupt Service Routines.
             ����ļ��ṩ�����е��쳣��������ģ���Լ������жϷ������
             This file provides template for all exceptions handler and peripherals interrupt service routine.*/

#include "stm32f10x_it.h"

#include "bsp_ov7725.h"
extern u8 Ov7725_vsync;

/*  Cortex-M3�������쳣�������Cortex-M3 Processor Exceptions Handlers  */

/** @brief  �������������ж��쳣This function handles NMI exception.
  * @param  None
  * @retval None*/
void NMI_Handler(void)
{
}

/**@brief  �����������Ӳ�����쳣This function handles Hard Fault exception.
  * @param  None
  * @retval None*/
void HardFault_Handler(void)
{
  while(1)
  {	//���Ϸ���ʱ����ѭ��Go to infinite loop when Hard Fault exception occurs
  }
}

/** @brief  ������������ڴ����This function handles Memory Manage exception.
  * @param  None
  * @retval None*/
void MemManage_Handler(void)
{
  while(1)
  {	//���Ϸ���ʱ����ѭ��Go to infinite loop when Hard Fault exception occurs
  }
}

/** @brief  ��������������߹����쳣This function handles Bus Fault exception.
  * @param  None
  * @retval None*/
void BusFault_Handler(void)
{
  while(1)
  {	//���Ϸ���ʱ����ѭ��Go to infinite loop when Hard Fault exception occurs
  }
}

/** @brief  ʹ�����������������쳣This function handles Usage Fault exception.
  * @param  None
  * @retval None*/
void UsageFault_Handler(void)
{
  while(1)
  { //���Ϸ���ʱ����ѭ��Go to infinite loop when Hard Fault exception occurs
  }
}

/** @brief  �����������SVCall����This function handles SVCall exception.
  * @param  None
  * @retval None*/
void SVC_Handler(void)
{
}

/** @brief  �������������Լ���쳣This function handles Debug Monitor exception.
  * @param  None
  * @retval None*/
void DebugMon_Handler(void)
{
}

/** @brief  �����������PendSVC����This function handles PendSVC exception.
  * @param  None
  * @retval None*/
void PendSV_Handler(void)
{
}

/** @brief  �����������SysTick�������This function handles SysTick Handler.
  * @param  None
  * @retval None*/
void SysTick_Handler(void)
{
}

// ov7725 ���ж� �������
void EXTI0_IRQHandler(void)
{
    if ( EXTI_GetITStatus(EXTI_Line0) != RESET ) 	//���EXTI_Line0��·�ϵ��ж������Ƿ��͵���NVIC 
    {
        if( Ov7725_vsync == 0 )
        {
            FIFO_WRST_L(); 	                      //����ʹFIFOд(����from����ͷ)ָ�븴λ
            FIFO_WE_H();	                        //����ʹFIFOд����
            
            Ov7725_vsync = 1;	   	
            FIFO_WE_H();                          //ʹFIFOд����
            FIFO_WRST_H();                        //����ʹFIFOд(����from����ͷ)ָ���˶�
        }
        else if( Ov7725_vsync == 1 )
        {
            FIFO_WE_L();                          //����ʹFIFOд��ͣ
            Ov7725_vsync = 2;
        }        
        EXTI_ClearITPendingBit(EXTI_Line0);		    //���EXTI_Line0��·�����־λ        
    }    
}

/** @brief  �����������PPP�ж�����This function handles PPP interrupt request.
  * @param  None
  * @retval None*/
/*void PPP_IRQHandler(void)
{
}*/
