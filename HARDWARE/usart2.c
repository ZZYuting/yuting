#include "sys.h"
#include "led.h"
#include "usart2.h"	  
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//Èç¹ûÊ¹ÓÃucos,Ôò°üÀ¨ÏÂÃæµÄÍ·ÎÄ¼ş¼´¿É.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos Ê¹ÓÃ	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   
//ÕıµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ĞŞ¸ÄÈÕÆÚ:2012/8/18
//°æ±¾£ºV1.5
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3ĞŞ¸ÄËµÃ÷ 
//Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
//¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
//Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
//ĞŞÕıÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
//V1.4ĞŞ¸ÄËµÃ÷
//1,ĞŞ¸Ä´®¿Ú³õÊ¼»¯IOµÄbug
//2,ĞŞ¸ÄÁËUSART2_RX_STA,Ê¹µÃ´®¿Ú×î´ó½ÓÊÕ×Ö½ÚÊıÎª2µÄ14´Î·½
//3,Ôö¼ÓÁËUSART_REC_LEN,ÓÃÓÚ¶¨Òå´®¿Ú×î´óÔÊĞí½ÓÊÕµÄ×Ö½ÚÊı(²»´óÓÚ2µÄ14´Î·½)
//4,ĞŞ¸ÄÁËEN_USART1_RXµÄÊ¹ÄÜ·½Ê½
//V1.5ĞŞ¸ÄËµÃ÷
//1,Ôö¼ÓÁË¶ÔUCOSIIµÄÖ§³Ö
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êı,¶ø²»ĞèÒªÑ¡Ôñuse MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////±ê×¼¿âĞèÒªµÄÖ§³Öº¯Êı                 
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
////ÖØ¶¨Òåfputcº¯Êı 
//int fputc(int ch, FILE *f)
//{      
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 
//    USART_SendData(USART2,(uint8_t)ch);   
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
 
#if EN_USART2_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
u8 USART2_RX_BUF[USART_REC_LEN],chaoshengbo[30]={0},q=0,c=1;     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓĞĞ§×Ö½ÚÊıÄ¿
u16 USART2_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	  

//³õÊ¼»¯IO ´®¿Ú1 
//bound:²¨ÌØÂÊ

void usart2_init(u32 bound){
    //GPIOç«¯å£è®¾ç½®
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 //pb10 pb11
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//USART1_TX //è¯¥I/Oåœ¨ä¸è¢«USARTé©±åŠ¨æ—¶ï¼Œå¿…é¡»é…ç½®æˆæ‚¬ç©ºè¾“å…¥(æˆ–å¼€æ¼çš„è¾“å‡ºé«˜)IN_FLOATINGï¼ˆåŸ å¤ç”¨æ¨æŒ½è¾“å‡ºï¼‰   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART1_RX	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//æµ®ç©ºè¾“å…¥
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_USART2,ENABLE);
   //usart2 NVIC é…ç½®

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//æŠ¢å ä¼˜å…ˆçº§3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//å­ä¼˜å…ˆçº§3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQé€šé“ä½¿èƒ½
	NVIC_Init(&NVIC_InitStructure);	//æ ¹æ®æŒ‡å®šçš„å‚æ•°åˆå§‹åŒ–VICå¯„å­˜å™¨
  
   //USART åˆå§‹åŒ–è®¾ç½®

	USART_InitStructure.USART_BaudRate = bound;//ä¸€èˆ¬è®¾ç½®ä¸º9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//å­—é•¿ä¸º8ä½æ•°æ®æ ¼å¼
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ä¸€ä¸ªåœæ­¢ä½
	USART_InitStructure.USART_Parity = USART_Parity_No;//æ— å¥‡å¶æ ¡éªŒä½
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//æ— ç¡¬ä»¶æ•°æ®æµæ§åˆ¶
	USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx;	//æ”¶å‘æ¨¡å¼
	
    USART_Init(USART2, &USART_InitStructure); //åˆå§‹åŒ–ä¸²å£
	//USART_HalfDuplexCmd(USART2, ENABLE);		//åŠåŒå·¥æ¨¡å¼
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//å¼€å¯ä¸­æ–­
    USART_Cmd(USART2, ENABLE);                    //ä½¿èƒ½ä¸²å£ 
	/* 	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);           //ä½¿èƒ½ä¸²å£1çš„DMAå‘é€  
		MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)Sendbuff,2);//DMA1é€šé“4,å¤–è®¾ä¸ºä¸²å£1,å­˜å‚¨å™¨ä¸ºSendBuff,é•¿åº¦5200.
		DMA_Cmd(DMA1_Channel4, ENABLE);  */
}


void USART2_IRQHandler(void)                	//ä¸²å£1ä¸­æ–­æœåŠ¡ç¨‹åº
	{
	{

	u8 Res2;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //æ¥æ”¶ä¸­æ–­(æ¥æ”¶åˆ°çš„æ•°æ®å¿…é¡»æ˜¯0x0d 0x0aç»“å°¾)
		{
		Res2=USART_ReceiveData(USART2);//(USART1->DR);	//è¯»å–æ¥æ”¶åˆ°çš„æ•°æ®
		if((USART2_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
			{
			if(q==1)
			{
				chaoshengbo[c]=Res2;
				if(c==25)
				{
					c=1;
					q=0;
					USART2_RX_STA|=0x8000;
					LED1=0;
				}
				c++;
			 }
			if(USART2_RX_STA&0x4000)//ÒÑ½ÓÊÕµ½ÁË0x18
				{if(Res2==0x30)	   //ÓÖ½ÓÊÕµ½0x30
					q=1;}
			if(Res2==0x18)
				USART2_RX_STA|=0x4000;
//			if(USART2_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
//				{
//				if(Res!=0x0a)
//					USART2_RX_STA=0;//½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
//				else 
//					USART2_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
//				}
//			else //»¹Ã»ÊÕµ½0X0D
//				{	
//				if(Res==0x0d)
//					USART2_RX_STA|=0x4000;
//				else
//					{
//					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
//					USART2_RX_STA++;
//						if(USART2_RX_STA>(USART_REC_LEN-1))
//							USART2_RX_STA=0;//½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ	  
//					}		 
//				}
			}   		 
     	} 
} 

#ifdef OS_TICKS_PER_SEC	 	//Èç¹ûÊ±ÖÓ½ÚÅÄÊı¶¨ÒåÁË,ËµÃ÷ÒªÊ¹ÓÃucosIIÁË.
	OSIntExit();  											 
#endif
} 
#endif	

void Send_turn2(u8 ch)
{
	USART2->DR=(u8)ch ;
	while((USART2->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø				
}
void zhixian(unsigned char velocity)
{
	unsigned char temp_sum=0;
	Send_turn2(0x55);
	Send_turn2(0x55);
	Send_turn2(0xaa);
	Send_turn2(0x01);
	Send_turn2(0x04);
	Send_turn2(0x26);
	Send_turn2(velocity);
	Send_turn2(0xf4);
	Send_turn2(velocity);
	Send_turn2(0xf4);
	temp_sum=0x55+0xaa+0x01+0x04+0x26+2*(velocity+0xf4);
	Send_turn2(temp_sum);
	while((USART2->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø
//	LED0=0;
	delay_ms(2);
}

void tingzhi()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x01);Send_turn2(0x01);
	Send_turn2(0x21);Send_turn2(0x01);Send_turn2(0x23);
}
void biaoding()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x01);Send_turn2(0x00);
	Send_turn2(0x36);Send_turn2(0x36);
}
void zhuanwan(char angle)
{
	if((int)angle<0)
	{
		Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x01);Send_turn2(0x04);Send_turn2(0x26);
		Send_turn2(0x80);Send_turn2(0xfa);Send_turn2(0x00);Send_turn2(0xfa);Send_turn2(0x9e);
		delay_ms(-(angle*40));
	}
	if(angle>0)
	{
		Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x01);Send_turn2(0x04);Send_turn2(0x26);
		Send_turn2(0x00);Send_turn2(0xfa);Send_turn2(0x80);Send_turn2(0xfa);Send_turn2(0x9e);
		delay_ms(angle*40);
	}
}
void jiasudu()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x01);Send_turn2(0x04);Send_turn2(0x27);
	Send_turn2(0x00);Send_turn2(0x01);Send_turn2(0x00);Send_turn2(0x01);Send_turn2(0x2d);
}
void new(unsigned char x,unsigned char y)
{
	
	unsigned char temp_sum,velocitylh=0,velocityll=0,velocityrh=0,velocityrl=0;
	unsigned short int velocityl=0,velocityr=0;
	jiasudu();
	if((y>=0x20)&&(y<=0x29))
	{	
		velocityl=0x8000+0x60*(0x2a-y);
		velocityr=0x8000+0x60*(0x2a-y);
	}
	else
	{
		velocityl=0x60*(y-0x2a);
		velocityr=0x60*(y-0x2a);
	}
//	if(y==0x2a)
//		velocityh=0x00;	
	if((x>=0x20)&&(x<=0x29))
		velocityr+=0x60*(0x2a-x);
	else
		velocityl+=0x60*(x-0x2a);
	if(y==0x2a)						 
	{
		if((x>=0x20)&&(x<=0x29))
		{
			velocityr=0x60*(0x2a-x);
			velocityl=0x8000+velocityr;
		}
		else
		{
			velocityl=0x60*(x-0x2a);
			velocityr=0x8000+velocityl;
		}
	}
	velocitylh=(unsigned char)(velocityl>>8);
	velocityll=(unsigned char)velocityl;
	velocityrh=(unsigned char)(velocityr>>8);
	velocityrl=(unsigned char)velocityr;
	Send_turn2(0x55);
	Send_turn2(0xaa);
	Send_turn2(0x01);
	Send_turn2(0x04);
	Send_turn2(0x26);
	Send_turn2(velocitylh);
	Send_turn2(velocityll);
	Send_turn2(velocityrh);
	Send_turn2(velocityrl);
	temp_sum=0x55+0xaa+0x01+0x04+0x26+velocitylh+velocityll+velocityrh+velocityrl;
	Send_turn2(temp_sum);
	while((USART2->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø
//	delay_ms(2);
}
void kaiqichaosheng()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x02);Send_turn2(0x01);
	Send_turn2(0x31);Send_turn2(0x01);Send_turn2(0x34);
}
void shinengsange()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x02);Send_turn2(0x04);
	Send_turn2(0x33);Send_turn2(0x01);Send_turn2(0xff);Send_turn2(0xff);
	Send_turn2(0xff);Send_turn2(0x36);
}
void chaxunsange()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x02);Send_turn2(0x03);
	Send_turn2(0x30);Send_turn2(0x90);Send_turn2(0x00);Send_turn2(0x04);
	Send_turn2(0x31);
}
		
void ult_init()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x02);Send_turn2(0x01);
	Send_turn2(0x31);Send_turn2(0x01);Send_turn2(0x34);
}

	u8 t;
	u8 len;
//u8 Ult_th=
void read_ult_th()
{
		if(USART2_RX_STA&0x8000)
		{				   
	/*		for(t=2;t<=25;t++)
			{
				USART_SendData(USART1, chaoshengbo[t]);//Ïò´®¿Ú1·¢ËÍÊı¾İ
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//µÈ´ı·¢ËÍ½áÊø
			}
//			for(t=0;t<3;t++)
//			{
////				USART_SendData(USART1, chaoshengbianhao[t]+0x30);
////				USART_SendData(USART1, 0x3d);
//				USART_SendData(USART1, chaoshengbo[chaoshengbianhao[t]]);
////				USART_SendData(USART1, 0x0a);
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//µÈ´ı·¢ËÍ½áÊø
//			}
//			USART_SendData(USART1, '\n');*/
			USART2_RX_STA=0;
		}
	
	ult_init();
	chaxunsange();
	delay_ms(50);
	if(!chaoshengbo[4]){ult_th_r=1;}		//			BEEP=1;
	else {ult_th_r=0;}						//			BEEP=0;
	if(!chaoshengbo[22]){ult_th_l=1;}		//			LED1=0;
	else {ult_th_l=0;}				 		//	 		LED1=1;
	if(chaoshengbo[25]<15)
	{
		ult_th_m=1;							//	  		LED0=0;	
	}	
	else
	{
		ult_th_m=0;							//			LED0=1;	
	}

/*		ult_th_m=1;
		ult_th_l=1;
		ult_th_r=1;*/
}
void read_ult_fangzhuang()
{
		if(USART2_RX_STA&0x8000)
			USART2_RX_STA=0;
	ult_init();
	chaxunsange();
	if(!chaoshengbo[4]){ult_th_r=1;}		//			BEEP=1;
	else {ult_th_r=0;}						//			BEEP=0;
	if(!chaoshengbo[22]){ult_th_l=1;}		//			LED1=0;
	else {ult_th_l=0;}				 		//	 		LED1=1;
	if(chaoshengbo[25]<15)
	{
		ult_th_m=1;							//	  		LED0=0;	
	}	
	else
	{
		ult_th_m=0;							//			LED0=1;	
	}
}

void stop()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x01);Send_turn2(0x01);
	Send_turn2(0x21);Send_turn2(0x01);Send_turn2(0x23);
}

void turn_left()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x01);Send_turn2(0x04);Send_turn2(0x26);
	Send_turn2(0x80);Send_turn2(0xfa);Send_turn2(0x00);Send_turn2(0xfa);Send_turn2(0x9e);
}

void turn_right()
{
	Send_turn2(0x55);Send_turn2(0xaa);Send_turn2(0x01);Send_turn2(0x04);Send_turn2(0x26);
	Send_turn2(0x00);Send_turn2(0xfa);Send_turn2(0x80);Send_turn2(0xfa);Send_turn2(0x9e);
}

void straight(unsigned char velocity)
{
	unsigned char temp_sum=0;
	Send_turn2(0x55);
	Send_turn2(0x55);
	Send_turn2(0xaa);
	Send_turn2(0x01);
	Send_turn2(0x04);
	Send_turn2(0x26);
	Send_turn2(velocity);
	Send_turn2(0xf4);
	Send_turn2(velocity);
	Send_turn2(0xf4);
	temp_sum=0x55+0xaa+0x01+0x04+0x26+2*(velocity+0xf4);
	Send_turn2(temp_sum);
	while((USART2->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø
//	LED0=0;
	delay_ms(2);
}
