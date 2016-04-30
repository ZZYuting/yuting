#include "sys.h"
#include "uart5.h"
#include "usart2.h"	
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	              
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

void uart5_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;         //PC12 UART5_TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
   
				
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PD.12 SP3485输出使能
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化PD.12
		GPIO_SetBits(GPIOD,GPIO_Pin_12);						 //PD.12 输出高
   
		USART_InitStructure.USART_BaudRate = bound;            //UART5初始化
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART5, &USART_InitStructure);
   
    USART_Cmd(UART5, ENABLE);
}

void UART5_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif

} 
void Send_turn5(u8 ch)
{
	UART5->DR=(u8)ch ;
	while((UART5->SR&0X40)==0);//等待发送结束				
}
void SetServoLimit(unsigned char id,unsigned short int cw_limit,unsigned short int ccw_limit)
{
	unsigned short int temp_ccw=0;
	unsigned short int temp_cw=0;
	unsigned char temp_ccw_h=0;
	unsigned char temp_ccw_l=0;
	unsigned char temp_cw_h=0;
	
	unsigned char temp_cw_l=0;
	unsigned char temp_sum=0;
	if(ccw_limit>1023)
	{
		temp_ccw=1023;
	}
	else
	{
		temp_ccw=ccw_limit;
	}
	if(cw_limit>1023)
	{
		temp_cw=1023;
	}
	else
	{
		temp_cw=cw_limit;
	}
	temp_ccw_h=(unsigned char)(temp_ccw>>8);
	temp_ccw_l=(unsigned char)temp_ccw;
	temp_cw_h=(unsigned char)(temp_cw>>8);
	temp_cw_l=(unsigned char)temp_cw;
	//UART5|=(SR&0X40);
	Send_turn5(0xff);
	Send_turn5(0xff);
	Send_turn5(id);
	Send_turn5(7);
	Send_turn5(0x03);
	Send_turn5(0x06);
	Send_turn5(temp_cw_l);
	Send_turn5(temp_cw_h);
	Send_turn5(temp_ccw_l);
	Send_turn5(temp_ccw_h);
	temp_sum=id+7+0x03+0x06+temp_cw_l+temp_cw_h+temp_ccw_l+temp_ccw_h;
	temp_sum=~temp_sum;
	Send_turn5(temp_sum);
	while((UART5->SR&0X40)==0);//??????
	delay_ms(2);
}
void SetServoPosition(unsigned char id,unsigned short int position,unsigned short int velocity)
{
	unsigned short int temp_velocity=0;
	unsigned short int temp_position=0;
	unsigned char temp_velocity_h=0;
	unsigned char temp_velocity_l=0;
	unsigned char temp_position_h=0;
	unsigned char temp_position_l=0;
	unsigned char temp_sum=0;
	if(velocity>1023)
	{
		temp_velocity=1023;
	}
	else
	{
		temp_velocity=velocity;
	}
	if(position>1023)
	{
		temp_position=1023;
	}
	else
	{
		temp_position=position;
	}
	temp_velocity_h=(unsigned char)(temp_velocity>>8);
	temp_velocity_l=(unsigned char)temp_velocity;
	temp_position_h=(unsigned char)(temp_position>>8);
	temp_position_l=(unsigned char)temp_position;
//	UART5|=(SR&0X40);
	Send_turn5(0xff);
	Send_turn5(0xff);
	Send_turn5(id);
	Send_turn5(7);
	Send_turn5(0x03);
	Send_turn5(0x1e);
	Send_turn5(temp_position_l);
	Send_turn5(temp_position_h);
	Send_turn5(temp_velocity_l);
	Send_turn5(temp_velocity_h);
	temp_sum=id+7+0x03+0x1e+temp_position_l+temp_position_h+temp_velocity_l+temp_velocity_h;
	temp_sum=~temp_sum;
	Send_turn5(temp_sum);
	while((UART5->SR&0X40)==0);//??????
	delay_ms(2);
}
void together(unsigned char id_to1,unsigned short int position_to1,unsigned short int velocity_to1,unsigned char id_to2,unsigned short int position_to2,unsigned short int velocity_to2)
{
    unsigned short int temp_velocity_1=0;
	unsigned short int temp_position_1=0;
	   unsigned char temp_velocity_h_1=0;
   	   unsigned char temp_velocity_l_1=0;
	   unsigned char temp_position_h_1=0;
	   unsigned char temp_position_l_1=0;
	unsigned short int temp_velocity_2=0;
	unsigned short int temp_position_2=0;
	   unsigned char temp_velocity_h_2=0;
   	   unsigned char temp_velocity_l_2=0;
	   unsigned char temp_position_h_2=0;
	   unsigned char temp_position_l_2=0;
	unsigned char temp_sum=0;
	temp_position_1=position_to1;
	temp_velocity_1=velocity_to1;
	temp_position_2=position_to2;
	temp_velocity_2=velocity_to2;

	temp_velocity_h_1=(unsigned char)(temp_velocity_1>>8);
	temp_velocity_l_1=(unsigned char)temp_velocity_1;
	temp_position_h_1=(unsigned char)(temp_position_1>>8);
	temp_position_l_1=(unsigned char)temp_position_1;

	temp_velocity_h_2=(unsigned char)(temp_velocity_2>>8);
	temp_velocity_l_2=(unsigned char)temp_velocity_2;
	temp_position_h_2=(unsigned char)(temp_position_2>>8);
	temp_position_l_2=(unsigned char)temp_position_2;

    Send_turn5(0xff);
	Send_turn5(0xff);
	Send_turn5(0xFE);
	Send_turn5(14);	   // ???????
	Send_turn5(0x83);
	Send_turn5(0x1e);
	Send_turn5(4);
	
	Send_turn5(id_to1);
	Send_turn5(temp_position_l_1);
	Send_turn5(temp_position_h_1);
	Send_turn5(temp_velocity_l_1);
	Send_turn5(temp_velocity_h_1);

	Send_turn5(id_to2);
	Send_turn5(temp_position_l_2);
	Send_turn5(temp_position_h_2);
	Send_turn5(temp_velocity_l_2);
	Send_turn5(temp_velocity_h_2);

	temp_sum=0xFE+14+ 0x83 +0x1e + 4+ id_to1+id_to2+temp_position_l_1+temp_position_h_1 +temp_velocity_l_1 +temp_velocity_h_1 + temp_position_l_2+temp_position_h_2 +temp_velocity_l_2 +temp_velocity_h_2;
	temp_sum=~temp_sum;
	Send_turn5(temp_sum);
	while((UART5->SR&0X40)==0);//??????
	delay_ms(2);
}
void Action_write(unsigned char id,unsigned short int position,unsigned short int velocity)
{
	unsigned short int temp_velocity=0;
	unsigned short int temp_position=0;
	unsigned char temp_velocity_h=0;
	unsigned char temp_velocity_l=0;
	unsigned char temp_position_h=0;
	unsigned char temp_position_l=0;
	unsigned char temp_sum=0;	
	temp_velocity=velocity;
	temp_position=position;
	temp_velocity_h=(unsigned char)(temp_velocity>>8);
	temp_velocity_l=(unsigned char)temp_velocity;
	temp_position_h=(unsigned char)(temp_position>>8);
	temp_position_l=(unsigned char)temp_position;
	Send_turn5(0xff);
	Send_turn5(0xff);
	Send_turn5(id);
	Send_turn5(7);
	Send_turn5(0x04);
	Send_turn5(0x1e);
	Send_turn5(temp_position_l);
	Send_turn5(temp_position_h);
	Send_turn5(temp_velocity_l);
	Send_turn5(temp_velocity_h);
	temp_sum=id+7+0x04+0x1e+temp_position_l+temp_position_h+temp_velocity_l+temp_velocity_h;
	temp_sum=~temp_sum;
	Send_turn5(temp_sum);
	while((UART5->SR&0X40)==0);//??????
	delay_ms(2);
}
void Action()
{
	unsigned char temp_sum=0;	
	Send_turn5(0xff);
	Send_turn5(0xff);
	Send_turn5(0xFE);
	Send_turn5(2);
	Send_turn5(0x05);
	temp_sum=(unsigned char)(0xFE+0x02+0x05);
	temp_sum=~temp_sum;
	Send_turn5(temp_sum);
	while((UART5->SR&0X40)==0);//??????
	delay_ms(2);
}
void ping(unsigned char id)
{
	unsigned char temp_sum=0;
	Send_turn5(0xff);
	Send_turn5(0xff);
	Send_turn5(id);
	Send_turn5(0x02);
	Send_turn5(0x01);
	temp_sum=id+0x02+0x01;
	temp_sum=~temp_sum;
	Send_turn5(temp_sum);
	while((UART5->SR&0X40)==0);//??????
	delay_ms(2);		
}

/*void SetServoVelocity(unsigned char id,signed short int velocity)
{
	unsigned char temp_sign=0;
	unsigned short int temp_velocity=0;
	unsigned char temp_value_h=0;
	unsigned char temp_value_l=0;
	unsigned char temp_sum=0;
	if(velocity<0)
	{
		temp_velocity=-velocity;
		temp_sign=1;
	}
	else
	{
		temp_velocity=velocity;
		temp_sign=0;
	}
	if(temp_velocity>1023)
	{
		temp_velocity=1023;
	}
	temp_velocity|=(temp_sign<<10);
	temp_value_h=(unsigned char)(temp_velocity>>8);
	temp_value_l=(unsigned char)temp_velocity;
	//UART5|=(SR&0X40);
	Send_turn5(0xff);
	Send_turn5(0xff);
	Send_turn5(id);
	Send_turn5(5);
	Send_turn5(0x03);
	Send_turn5(0x20);
	Send_turn5(temp_value_l);
	Send_turn5(temp_value_h);
	temp_sum=id+5+0x03+0x20+temp_value_l+temp_value_h;
	temp_sum=~temp_sum;
	Send_turn5(temp_sum);
	while((UART5->SR&0X40)==0);//??????
	delay_ms(2);

}	  */
void arm_init(void)
{
	SetServoLimit(14,0,1023);
	SetServoLimit(13,0,1023);
	SetServoLimit(12,0,1023);
	SetServoLimit(11,0,1023);
	SetServoLimit(4,0,1023);
	SetServoLimit(3,0,1023);
	SetServoLimit(2,0,1023);
	SetServoLimit(1,0,1023);
}