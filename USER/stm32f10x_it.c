/*
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   主要中断服务例程Main Interrupt Service Routines.
             这个文件提供了所有的异常处理程序和模板以及外设中断服务程序
             This file provides template for all exceptions handler and peripherals interrupt service routine.*/

#include "stm32f10x_it.h"

#include "bsp_ov7725.h"
extern u8 Ov7725_vsync;

/*  Cortex-M3处理器异常处理程序Cortex-M3 Processor Exceptions Handlers  */

/** @brief  这个函数处理敝中断异常This function handles NMI exception.
  * @param  None
  * @retval None*/
void NMI_Handler(void)
{
}

/**@brief  这个函数处理硬故障异常This function handles Hard Fault exception.
  * @param  None
  * @retval None*/
void HardFault_Handler(void)
{
  while(1)
  {	//故障发生时无限循环Go to infinite loop when Hard Fault exception occurs
  }
}

/** @brief  这个函数处理内存管理This function handles Memory Manage exception.
  * @param  None
  * @retval None*/
void MemManage_Handler(void)
{
  while(1)
  {	//故障发生时无限循环Go to infinite loop when Hard Fault exception occurs
  }
}

/** @brief  这个函数处理总线故障异常This function handles Bus Fault exception.
  * @param  None
  * @retval None*/
void BusFault_Handler(void)
{
  while(1)
  {	//故障发生时无限循环Go to infinite loop when Hard Fault exception occurs
  }
}

/** @brief  使用这个函数处理故障异常This function handles Usage Fault exception.
  * @param  None
  * @retval None*/
void UsageFault_Handler(void)
{
  while(1)
  { //故障发生时无限循环Go to infinite loop when Hard Fault exception occurs
  }
}

/** @brief  这个函数处理SVCall例外This function handles SVCall exception.
  * @param  None
  * @retval None*/
void SVC_Handler(void)
{
}

/** @brief  这个函数处理调试监控异常This function handles Debug Monitor exception.
  * @param  None
  * @retval None*/
void DebugMon_Handler(void)
{
}

/** @brief  这个函数处理PendSVC例外This function handles PendSVC exception.
  * @param  None
  * @retval None*/
void PendSV_Handler(void)
{
}

/** @brief  这个函数处理SysTick处理程序This function handles SysTick Handler.
  * @param  None
  * @retval None*/
void SysTick_Handler(void)
{
}

// ov7725 场中断 服务程序
void EXTI0_IRQHandler(void)
{
    if ( EXTI_GetITStatus(EXTI_Line0) != RESET ) 	//检查EXTI_Line0线路上的中断请求是否发送到了NVIC 
    {
        if( Ov7725_vsync == 0 )
        {
            FIFO_WRST_L(); 	                      //拉低使FIFO写(数据from摄像头)指针复位
            FIFO_WE_H();	                        //拉高使FIFO写允许
            
            Ov7725_vsync = 1;	   	
            FIFO_WE_H();                          //使FIFO写允许
            FIFO_WRST_H();                        //允许使FIFO写(数据from摄像头)指针运动
        }
        else if( Ov7725_vsync == 1 )
        {
            FIFO_WE_L();                          //拉低使FIFO写暂停
            Ov7725_vsync = 2;
        }        
        EXTI_ClearITPendingBit(EXTI_Line0);		    //清除EXTI_Line0线路挂起标志位        
    }    
}

/** @brief  这个函数处理PPP中断请求This function handles PPP interrupt request.
  * @param  None
  * @retval None*/
/*void PPP_IRQHandler(void)
{
}*/
