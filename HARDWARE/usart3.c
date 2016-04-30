#include "sys.h"
#include "usart3.h"	
#include "led.h"  
////////////////////////////////////////////////////////////////////////////////// 	 
//Èç¹ûÊ¹ÓÃucos,Ôò°üÀ¨ÏÂÃæµÄÍ·ÎÄ¼þ¼´¿É.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos Ê¹ÓÃ	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ÐÞ¸ÄÈÕÆÚ:2012/8/18
//°æ±¾£ºV1.5
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖÝÊÐÐÇÒíµç×Ó¿Æ¼¼ÓÐÏÞ¹«Ë¾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3ÐÞ¸ÄËµÃ÷ 
//Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
//¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
//Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
//ÐÞÕýÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
//V1.4ÐÞ¸ÄËµÃ÷
//1,ÐÞ¸Ä´®¿Ú³õÊ¼»¯IOµÄbug
//2,ÐÞ¸ÄÁËUSART3_RX_STA,Ê¹µÃ´®¿Ú×î´ó½ÓÊÕ×Ö½ÚÊýÎª2µÄ14´Î·½
//3,Ôö¼ÓÁËUSART_REC_LEN,ÓÃÓÚ¶¨Òå´®¿Ú×î´óÔÊÐí½ÓÊÕµÄ×Ö½ÚÊý(²»´óÓÚ2µÄ14´Î·½)
//4,ÐÞ¸ÄÁËEN_USART1_RXµÄÊ¹ÄÜ·½Ê½
//V1.5ÐÞ¸ÄËµÃ÷
//1,Ôö¼ÓÁË¶ÔUCOSIIµÄÖ§³Ö
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êý,¶ø²»ÐèÒªÑ¡Ôñuse MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////±ê×¼¿âÐèÒªµÄÖ§³Öº¯Êý                 
//struct __FILE 
//{ 
//	int handle; 

//}; 

//FILE __stdout;       
////¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
////ÖØ¶¨Òåfputcº¯Êý 
//int fputc(int ch, FILE *f)
//{      
//	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 
//    USART_SendData(USART3,(uint8_t)ch);   
//	return ch;
//}
//#endif 

/*Ê¹ÓÃmicroLibµÄ·½·¨*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART3_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
u8 USART3_RX_BUF[USART_REC_LEN];     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓÐÐ§×Ö½ÚÊýÄ¿
u16 USART3_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	  

//³õÊ¼»¯IO ´®¿Ú1 
//bound:²¨ÌØÂÊ

void usart3_init(u32 bound){
    //GPIOç«¯å£è®¾ç½®
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 //pb10 pb11
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//USART1_TX //è¯¥I/Oåœ¨ä¸è¢«USARTé©±åŠ¨æ—¶ï¼Œå¿…é¡»é…ç½®æˆæ‚¬ç©ºè¾“å…¥(æˆ–å¼€æ¼çš„è¾“å‡ºé«˜)IN_FLOATINGï¼ˆåŽŸ å¤ç”¨æŽ¨æŒ½è¾“å‡ºï¼‰   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //USART1_RX	 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//æµ®ç©ºè¾“å…¥
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
   //Usart3 NVIC é…ç½®

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//æŠ¢å ä¼˜å…ˆçº§3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//å­ä¼˜å…ˆçº§3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQé€šé“ä½¿èƒ½
	NVIC_Init(&NVIC_InitStructure);	//æ ¹æ®æŒ‡å®šçš„å‚æ•°åˆå§‹åŒ–VICå¯„å­˜å™¨
  
   //USART åˆå§‹åŒ–è®¾ç½®

	USART_InitStructure.USART_BaudRate = bound;//ä¸€èˆ¬è®¾ç½®ä¸º9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//å­—é•¿ä¸º8ä½æ•°æ®æ ¼å¼
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ä¸€ä¸ªåœæ­¢ä½
	USART_InitStructure.USART_Parity = USART_Parity_No;//æ— å¥‡å¶æ ¡éªŒä½
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//æ— ç¡¬ä»¶æ•°æ®æµæŽ§åˆ¶
	USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx;	//æ”¶å‘æ¨¡å¼
	
    USART_Init(USART3, &USART_InitStructure); //åˆå§‹åŒ–ä¸²å£
	//USART_HalfDuplexCmd(USART3, ENABLE);		//åŠåŒå·¥æ¨¡å¼
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//å¼€å¯ä¸­æ–­
    USART_Cmd(USART3, ENABLE);                    //ä½¿èƒ½ä¸²å£ 
	/* 	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);           //ä½¿èƒ½ä¸²å£1çš„DMAå‘é€  
		MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)Sendbuff,2);//DMA1é€šé“4,å¤–è®¾ä¸ºä¸²å£1,å­˜å‚¨å™¨ä¸ºSendBuff,é•¿åº¦5200.
		DMA_Cmd(DMA1_Channel4, ENABLE);  */
}

u16 ARMED=0;
void USART3_IRQHandler(void)                	//ä¸²å£1ä¸­æ–­æœåŠ¡ç¨‹åº
{
//	LED0=0;
	{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //æŽ¥æ”¶ä¸­æ–­(æŽ¥æ”¶åˆ°çš„æ•°æ®å¿…é¡»æ˜¯0x0d 0x0aç»“å°¾)
		{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//è¯»å–æŽ¥æ”¶åˆ°çš„æ•°æ®
		
		if((USART3_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
			{
			if(USART3_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
				{
				if(Res!=0x0a)
					USART3_RX_STA=0;//½ÓÊÕ´íÎó,ÖØÐÂ¿ªÊ¼
				else{ 
					USART3_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res;
					flag_change();
					}
				}
			else //»¹Ã»ÊÕµ½0X0D
				{	
				if(Res==0x0d)
					USART3_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
						if(USART3_RX_STA>(USART_REC_LEN-1))
							USART3_RX_STA=0;//½ÓÊÕÊý¾Ý´íÎó,ÖØÐÂ¿ªÊ¼½ÓÊÕ	  
					}		 
				}
			}   		 
     	} 
	} 

#ifdef OS_TICKS_PER_SEC	 	//Èç¹ûÊ±ÖÓ½ÚÅÄÊý¶¨ÒåÁË,ËµÃ÷ÒªÊ¹ÓÃucosIIÁË.
	OSIntExit();  											 
#endif
} 
#endif	
void Send_turn3(u8 ch)
{
	USART3->DR=(u8)ch ;
	while((USART3->SR&0X40)==0);//µÈ´ý·¢ËÍ½áÊø				
}
void flag_change(void)
{
	if((USART3_RX_BUF[0]=='A')&&(USART3_RX_BUF[1]=='T'))
	{
		if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='0')){State=ON;}
		else if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='1')){State=OFF;}
		else if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='2')){}
		else if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='3')){}
		else if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='4')){}
		else if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='5')){}
		else if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='6')){}			
		else if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='7')){}			
		else if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='8')){}
		else if((USART3_RX_BUF[2]=='0')&&(USART3_RX_BUF[3]=='9')){}
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='0')){}
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='1')){}
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='2')){}			
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='3')){}
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='4')){}
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='5')){}
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='6')){}			
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='7')){}			
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='8')){}
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='9')){}			
		else if((USART3_RX_BUF[2]=='1')&&(USART3_RX_BUF[3]=='0')){}
		else if((USART3_RX_BUF[2]=='2')&&(USART3_RX_BUF[3]=='2')){}
		else if((USART3_RX_BUF[2]=='2')&&(USART3_RX_BUF[3]=='2')){}			
		else if((USART3_RX_BUF[2]=='2')&&(USART3_RX_BUF[3]=='3')){}
		else if((USART3_RX_BUF[2]=='2')&&(USART3_RX_BUF[3]=='4')){}
		else if((USART3_RX_BUF[2]=='2')&&(USART3_RX_BUF[3]=='5')){}
		else if((USART3_RX_BUF[2]=='2')&&(USART3_RX_BUF[3]=='6')){}			
		else if((USART3_RX_BUF[2]=='2')&&(USART3_RX_BUF[3]=='7')){}			
		else if((USART3_RX_BUF[2]=='2')&&(USART3_RX_BUF[3]=='8')){}
		else if((USART3_RX_BUF[2]=='2')&&(USART3_RX_BUF[3]=='9')){}	
	}
}
