#include "sys.h"
#include "usart3.h"	
#include "led.h"  
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
//2,�޸���USART3_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
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
//	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 
//    USART_SendData(USART3,(uint8_t)ch);   
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
 
#if EN_USART3_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	  

//��ʼ��IO ����1 
//bound:������

void usart3_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 //pb10 pb11
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//USART1_TX //该I/O在不被USART驱动时，必须配置成悬空输入(或开漏的输出高)IN_FLOATING（原 复用推挽输出）   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //USART1_RX	 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
   //Usart3 NVIC 配置

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
    USART_Init(USART3, &USART_InitStructure); //初始化串口
	//USART_HalfDuplexCmd(USART3, ENABLE);		//半双工模式
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 
	/* 	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);           //使能串口1的DMA发送  
		MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)Sendbuff,2);//DMA1通道4,外设为串口1,存储器为SendBuff,长度5200.
		DMA_Cmd(DMA1_Channel4, ENABLE);  */
}

u16 ARMED=0;
void USART3_IRQHandler(void)                	//串口1中断服务程序
{
//	LED0=0;
	{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		
		if((USART3_RX_STA&0x8000)==0)//����δ���
			{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)
					USART3_RX_STA=0;//���մ���,���¿�ʼ
				else{ 
					USART3_RX_STA|=0x8000;	//���������
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res;
					flag_change();
					}
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)
					USART3_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
						if(USART3_RX_STA>(USART_REC_LEN-1))
							USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     	} 
	} 

#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	
void Send_turn3(u8 ch)
{
	USART3->DR=(u8)ch ;
	while((USART3->SR&0X40)==0);//�ȴ����ͽ���				
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
