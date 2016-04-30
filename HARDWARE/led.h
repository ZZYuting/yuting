#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
//Mini STM32开发板
//LED驱动代码			 
//正点原子@ALIENTEK
//2012/2/27

//LED端口定义
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	
#define fire PAin(0)
#define voice PAin(1)
#define smoke PAin(4)
#define hongwai PAin(5)
#define wireless2 PAin(6)
#define wireless3 PAin(7)
#define wireless4 PAin(12)

//蜂鸣器端口定义
#define BEEP PBout(8)	// BEEP,蜂鸣器接口		   

void LED_Init(void);//初始化
void sensor_init(void);
void BEEP_Init(void);		 				    
#endif
