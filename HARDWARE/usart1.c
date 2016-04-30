#include "sys.h"
#include "usart1.h"
#include "delay.h"
//int x=0,y=0,f=0;
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//Mini STM32¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ĞŞ¸ÄÈÕÆÚ:2010/5/27
//°æ±¾£ºV1.3
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÕıµãÔ­×Ó 2009-2019
//All rights reserved
//********************************************************************************
//V1.3ĞŞ¸ÄËµÃ÷ 
//Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
//¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
//Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
//ĞŞÕıÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êı,¶ø²»ĞèÒªÑ¡Ôñuse MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//±ê×¼¿âĞèÒªµÄÖ§³Öº¯Êı                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
_sys_exit(int x) 
{ 
	x = x; 
} 
//ÖØ¶¨Òåfputcº¯Êı 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

//////////////////////////////////////////////////////////////////

#ifdef EN_USART1_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
u8 USART_RX_BUF[64];     //½ÓÊÕ»º³å,×î´ó64¸ö×Ö½Ú.
//½ÓÊÕ×´Ì¬
//bit7£¬½ÓÊÕÍê³É±êÖ¾
//bit6£¬½ÓÊÕµ½0x0d
//bit5~0£¬½ÓÊÕµ½µÄÓĞĞ§×Ö½ÚÊıÄ¿
u8 USART_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	  
  
void USART1_IRQHandler(void)
{
	u8 res;	    
	if(USART1->SR&(1<<5))//½ÓÊÕµ½Êı¾İ
	{	 
		res=USART1->DR; 
		if((USART_RX_STA&0x80)==0)//½ÓÊÕÎ´Íê³É
		{
			if(USART_RX_STA&0x40)//½ÓÊÕµ½ÁË0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
				else USART_RX_STA|=0x80;	//½ÓÊÕÍê³ÉÁË 
			}else //»¹Ã»ÊÕµ½0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x40;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3F]=res;
					USART_RX_STA++;
					if(USART_RX_STA>63)USART_RX_STA=0;//½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ	  
				}		 
			}
		}  		 									     
	}  											 
} 
#endif										 
//³õÊ¼»¯IO ´®¿Ú1
//pclk2:PCLK2Ê±ÖÓÆµÂÊ(Mhz)
//bound:²¨ÌØÂÊ
//CHECK OK
//091209
void usart1_init(u32 pclk2,u32 bound)
{ 
	NVIC_InitTypeDef NVIC_InitStructure; 	
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//µÃµ½USARTDIV
	mantissa=temp;				 //µÃµ½ÕûÊı²¿·Ö
	fraction=(temp-mantissa)*16; //µÃµ½Ğ¡Êı²¿·Ö	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //Ê¹ÄÜPORTA¿ÚÊ±ÖÓ  
	RCC->APB2ENR|=1<<14;  //Ê¹ÄÜ´®¿ÚÊ±ÖÓ 
	GPIOA->CRH&=0XFFFFF00F; 
	GPIOA->CRH|=0X000008B0;//IO×´Ì¬ÉèÖÃ
		  
	RCC->APB2RSTR|=1<<14;   //¸´Î»´®¿Ú1
	RCC->APB2RSTR&=~(1<<14);//Í£Ö¹¸´Î»	   	   
	//²¨ÌØÂÊÉèÖÃ
 	USART1->BRR=mantissa; // ²¨ÌØÂÊÉèÖÃ	 
	USART1->CR1|=0X200C;  //1Î»Í£Ö¹,ÎŞĞ£ÑéÎ».
#ifdef EN_USART1_RX		  //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
	//Ê¹ÄÜ½ÓÊÕÖĞ¶Ï
	USART1->CR1|=1<<8;    //PEÖĞ¶ÏÊ¹ÄÜ
	USART1->CR1|=1<<5;    //½ÓÊÕ»º³åÇø·Ç¿ÕÖĞ¶ÏÊ¹ÄÜ	    	
//	MY_NVIC_Init(3,3,USART1_IRQChannel,2);//×é2£¬×îµÍÓÅÏÈ¼¶ 
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//æŠ¢å ä¼˜å…ˆçº§3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//å­ä¼˜å…ˆçº§3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQé€šé“ä½¿èƒ½
	NVIC_Init(&NVIC_InitStructure);	//æ ¹æ®æŒ‡å®šçš„å‚æ•°åˆå§‹åŒ–VICå¯„å­˜å™¨
 
#endif
//	SetServoLimit(12,0,1023);
}

