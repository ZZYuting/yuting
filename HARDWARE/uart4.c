#include "sys.h"
#include "uart4.h"
#include "usart2.h"	
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	              
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 
u8 UART4_RX_BUF[USART4_REC_LEN];
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART4_RX_STA=0;       //����״̬���	  

void uart4_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;         //PC10 UART4_TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//PC11 UART4_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
		 
		USART_InitStructure.USART_BaudRate = bound;            //UART4��ʼ��
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART4, &USART_InitStructure);
   
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先�?
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存�
   
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
		USART_Cmd(UART5, ENABLE);
}
void Send_turn4(u8 ch)
{
	UART4->DR=(u8)ch ;
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���				
}
void UART4_IRQHandler(void)                	//串口1中断服务程序
{
//	LED0=0;
	{
	u8 Res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须�?x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(UART4);//(USART1->DR);	//读取接收到的数据
		
		if((UART4_RX_STA&0x8000)==0)//����δ���
			{
			if(UART4_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)
					UART4_RX_STA=0;//���մ���,���¿�ʼ
				else{ 
					UART4_RX_STA|=0x8000;	//���������
					UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res;
					flag_change();
					}
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)
					UART4_RX_STA|=0x4000;
				else
					{
					UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
					UART4_RX_STA++;
						if(UART4_RX_STA>(USART_REC_LEN-1))
							UART4_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     	} 
	} 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 