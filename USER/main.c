#include "stm32f10x.h"
#include "sys.h"
extern uint8_t Ov7725_vsync;
char result=4;
unsigned char flag=1,first=0;
void jiance(void);
void ceshi(void);
int main(void)
{	
	unsigned char i=0;
	usart1_init(72,9600);
	/* 液晶初始化 */
//	LCD_Init();	
  //while(1);
  //#if 1
	/* ov7725 gpio 初始化 */
//	Ov7725_GPIO_Config();	
	/* ov7725 寄存器配置初始化 */
//	while(Ov7725_Init() != SUCCESS);	
	delay_init();
	NVIC_Configuration();
	uart5_init(1000000);				//与舵机通信
	arm_init();	
//	VSYNC_Init();	
	Ov7725_vsync = 0;
	Key_GPIO_Config();
	LED_GPIO_Config();
	TIM3_GPIO_Config();
	zhili();
	l_0tai();
	while(1)
	{
		l_0mai_1();
		l_0mai_2();
		l_luo();
		r_tai();
		r_mai1();
		r_mai2();
		r_luo();
		l_tai();
	}
}
void jiance(void)
{
	if( Ov7725_vsync == 2 )
	{
		FIFO_PREPARE;  			/*FIFO准备*/					
		result=biaozhun();					/*采集并显示*/
		Ov7725_vsync = 0;			
	}
}
void ceshi(void)
{
	while(1)
	{
		TIM3_Mode_Config(100);
			delay_ms(500);
		TIM3_Mode_Config(200);
			delay_ms(500);
		TIM3_Mode_Config(300);
			delay_ms(500);
		TIM3_Mode_Config(400);
			delay_ms(500);
		TIM3_Mode_Config(500);
			delay_ms(500);
		TIM3_Mode_Config(600);
			delay_ms(500);
		TIM3_Mode_Config(500);
			delay_ms(500);
		TIM3_Mode_Config(400);
			delay_ms(500);
		TIM3_Mode_Config(300);
			delay_ms(500);
		TIM3_Mode_Config(200);
			delay_ms(500);
	}		
}