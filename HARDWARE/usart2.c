#include "sys.h"
#include "led.h"
#include "usart2.h"	  
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART2_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#if 1
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 

//}; 

//FILE __stdout;       
////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 
////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{      
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 
//    USART_SendData(USART2,(uint8_t)ch);   
//	return ch;
//}
//#endif 

/*ʹ��microLib�ķ���*/
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
 
#if EN_USART2_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART_REC_LEN],chaoshengbo[30]={0},q=0,c=1;     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���	  

//��ʼ��IO ����1 
//bound:������

void usart2_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 //pb10 pb11
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//USART1_TX //该I/O在不被USART驱动时，必须配置成悬空输入(或开漏的输出高)IN_FLOATING（原 复用推挽输出）   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART1_RX	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_USART2,ENABLE);
   //usart2 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
    USART_Init(USART2, &USART_InitStructure); //初始化串口
	//USART_HalfDuplexCmd(USART2, ENABLE);		//半双工模式
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);                    //使能串口 
	/* 	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);           //使能串口1的DMA发送  
		MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)Sendbuff,2);//DMA1通道4,外设为串口1,存储器为SendBuff,长度5200.
		DMA_Cmd(DMA1_Channel4, ENABLE);  */
}


void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	{

	u8 Res2;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res2=USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		if((USART2_RX_STA&0x8000)==0)//����δ���
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
			if(USART2_RX_STA&0x4000)//�ѽ��յ���0x18
				{if(Res2==0x30)	   //�ֽ��յ�0x30
					q=1;}
			if(Res2==0x18)
				USART2_RX_STA|=0x4000;
//			if(USART2_RX_STA&0x4000)//���յ���0x0d
//				{
//				if(Res!=0x0a)
//					USART2_RX_STA=0;//���մ���,���¿�ʼ
//				else 
//					USART2_RX_STA|=0x8000;	//��������� 
//				}
//			else //��û�յ�0X0D
//				{	
//				if(Res==0x0d)
//					USART2_RX_STA|=0x4000;
//				else
//					{
//					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
//					USART2_RX_STA++;
//						if(USART2_RX_STA>(USART_REC_LEN-1))
//							USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//					}		 
//				}
			}   		 
     	} 
} 

#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	

void Send_turn2(u8 ch)
{
	USART2->DR=(u8)ch ;
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���				
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
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���
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
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���
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
				USART_SendData(USART1, chaoshengbo[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
//			for(t=0;t<3;t++)
//			{
////				USART_SendData(USART1, chaoshengbianhao[t]+0x30);
////				USART_SendData(USART1, 0x3d);
//				USART_SendData(USART1, chaoshengbo[chaoshengbianhao[t]]);
////				USART_SendData(USART1, 0x0a);
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
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
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���
//	LED0=0;
	delay_ms(2);
}
