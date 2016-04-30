#include "sys.h"
#include "uart4.h"
#include "usart2.h"	
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	              
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 
u8 UART4_RX_BUF[USART4_REC_LEN];
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART4_RX_STA=0;       //接收状态标记	  

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
		 
		USART_InitStructure.USART_BaudRate = bound;            //UART4初始化
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART4, &USART_InitStructure);
   
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//鎶㈠崰浼樺厛绾?
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//瀛愪紭鍏堢骇3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ閫氶亾浣胯兘
		NVIC_Init(&NVIC_InitStructure);	//鏍规嵁鎸囧畾鐨勫弬鏁板垵濮嬪寲VIC瀵勫瓨鍣
   
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
		USART_Cmd(UART5, ENABLE);
}
void Send_turn4(u8 ch)
{
	UART4->DR=(u8)ch ;
	while((UART4->SR&0X40)==0);//等待发送结束				
}
void UART4_IRQHandler(void)                	//涓插彛1涓柇鏈嶅姟绋嬪簭
{
//	LED0=0;
	{
	u8 Res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //鎺ユ敹涓柇(鎺ユ敹鍒扮殑鏁版嵁蹇呴』鏄?x0d 0x0a缁撳熬)
		{
		Res =USART_ReceiveData(UART4);//(USART1->DR);	//璇诲彇鎺ユ敹鍒扮殑鏁版嵁
		
		if((UART4_RX_STA&0x8000)==0)//接收未完成
			{
			if(UART4_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)
					UART4_RX_STA=0;//接收错误,重新开始
				else{ 
					UART4_RX_STA|=0x8000;	//接收完成了
					UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res;
					flag_change();
					}
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)
					UART4_RX_STA|=0x4000;
				else
					{
					UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
					UART4_RX_STA++;
						if(UART4_RX_STA>(USART_REC_LEN-1))
							UART4_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     	} 
	} 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 