#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
//Mini STM32������
//LED��������			 
//����ԭ��@ALIENTEK
//2012/2/27

//LED�˿ڶ���
#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	
#define fire PAin(0)
#define voice PAin(1)
#define smoke PAin(4)
#define hongwai PAin(5)
#define wireless2 PAin(6)
#define wireless3 PAin(7)
#define wireless4 PAin(12)

//�������˿ڶ���
#define BEEP PBout(8)	// BEEP,�������ӿ�		   

void LED_Init(void);//��ʼ��
void sensor_init(void);
void BEEP_Init(void);		 				    
#endif
