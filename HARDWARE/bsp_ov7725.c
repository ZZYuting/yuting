#include "bsp_ov7725.h"
#include "bsp_sccb.h"
#include "bsp_ili9341_lcd.h"

typedef struct Reg
{
	uint8_t Address; //�Ĵ�����ַ
	uint8_t Value;   //�Ĵ���ֵ
}Reg_Info;

/* �Ĵ����������� */
Reg_Info Sensor_Config[] =
{
	{CLKRC,     0x00}, /*ʱ������clock config*/
	{COM7,      0x46}, /*QVGA RGB565 */
    {HSTART,    0x3f},
	{HSIZE,     0x50},
	{VSTRT,     0x03},
	{VSIZE,     0x78},
	{HREF,      0x00},
	{HOutSize,  0x50},
	{VOutSize,  0x78},
	{EXHCH,     0x00},
	/*DSP���� DSP control*/
	{TGT_B,     0x7f},
	{FixGain,   0x09},
	{AWB_Ctrl0, 0xe0},
	{DSP_Ctrl1, 0xff},
	{DSP_Ctrl2, 0x20},
	{DSP_Ctrl3,	0x00},
	{DSP_Ctrl4, 0x00},
	/*AGC AEC AWB*/
	{COM8,		0xf0},
	{COM4,	    0x81}, /*���໷�������Pll AEC CONFIG*/
	{COM6,		0xc5},
	{COM9,		0x21},
	{BDBase,	0xFF},
	{BDMStep,	0x01},
	{AEW,		0x34},
	{AEB,		0x3c},
	{VPT,		0xa1},
	{EXHCL,		0x00},
	{AWBCtrl3,  0xaa},
	{COM8,		0xff},
	{AWBCtrl1,  0x5d},

	{EDGE1,		0x0a},
	{DNSOff,	0x01},
	{EDGE2,		0x01},
	{EDGE3,		0x01},

	{MTX1,		0x5f},
	{MTX2,		0x53},
	{MTX3,		0x11},
	{MTX4,		0x1a},
	{MTX5,		0x3d},
	{MTX6,		0x5a},
	{MTX_Ctrl,  0x1e},

	{BRIGHT,	0x00},
	{CNST,		0x25},
	{USAT,		0x65},
	{VSAT,		0x65},
	{UVADJ0,	0x81},
	{SDE,		0x06},	
    /*٤������GAMMA config*/
	{GAM1,		  0x0c},
	{GAM2,		  0x16},
	{GAM3,		  0x2a},
	{GAM4,		  0x4e},
	{GAM5,		  0x61},
	{GAM6,		  0x6f},
	{GAM7,		  0x7b},
	{GAM8,		  0x86},
	{GAM9,		  0x8e},
	{GAM10,		  0x97},
	{GAM11,		  0xa4},
	{GAM12,		  0xaf},
	{GAM13,		  0xc5},
	{GAM14,		  0xd7},
	{GAM15,		  0xe8},
	{SLOP,		  0x20},

	{HUECOS,	  0x80},
	{HUESIN,	  0x80},
	{DSPAuto,	  0xff},
	{DM_LNL,	  0x00},
	{BDBase,	  0x99},
	{BDMStep,	  0x03},
	{LC_RADI,	  0x00},
	{LC_COEF,	  0x13},
	{LC_XC,		  0x08},
	{LC_COEFB,    0x14},
	{LC_COEFR,    0x17},
	{LC_CTR,	  0x05},
	
	{COM3,		  0xd0},/*ˮƽ����Horizontal mirror image*/
	/*ҹ���Զ�֡���ʿ���ģʽnight mode auto frame rate control*/
	{COM5,		  0xf5},	 /*��ҹ�ӻ����£��Զ�����֡�ʣ���֤���նȻ�������*/
	//{COM5,		0x31},	/*ҹ�ӻ���֡�ʲ���*/
};

u8 OV7725_REG_NUM = sizeof(Sensor_Config)/sizeof(Sensor_Config[0]);	//�ṹ�������Ա��Ŀ

uint8_t Ov7725_vsync = 0;//֡ͬ���źű�־�����жϺ�����main��������ʹ��

/* ��������FIFO_GPIO_Config
 * ����  ��FIFO GPIO����
 * ����  ����
 * ���  ����
 * ע��  ���� */
static void FIFO_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | 
	                          RCC_APB2Periph_GPIOB | 
	                          RCC_APB2Periph_GPIOC | 
	                          RCC_APB2Periph_GPIOD , ENABLE);
	
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		/* 1W LED �ƿ��� */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		//GPIO_ResetBits(GPIOA, GPIO_Pin_8);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);	
		/*PD3(FIFO_WEN--FIFOдʹ��)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOD, &GPIO_InitStructure);	
		/*PB5(FIFO_WRST--FIFOд��λ)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/*PA2(FIFO_RRST--FIFO����λ) PA3(FIFO_OE--FIFO���ʹ��)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
		/*PC5(FIFO_RCLK-FIFO��ʱ��)*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /*PB8-PB15(FIFO_DATA--FIFO�������)*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    FIFO_CS_L();	  					/*����ʹFIFO���ʹ��*/
    FIFO_WE_H();   						/*����ʹFIFOд����*/
}

void Ov7725_GPIO_Config(void)
{
	SCCB_GPIO_Config();
	FIFO_GPIO_Config();
}

/* ��������VSYNC_GPIO_Configuration
 * ����  ��OV7725 GPIO����
 * ����  ����
 * ���  ����
 * ע��  ���� */
static void VSYNC_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  /*PA0---VSYNC*/
	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* ��������VSYNC_NVIC_Configuration
 * ����  ��VSYNC�ж�����
 * ����  ����
 * ���  ����
 * ע��  ����*/
static void VSYNC_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* ��������VSYNC_EXTI_Configuration
 * ����  ��OV7725 VSYNC�жϹܽ�����
 * ����  ����
 * ���  ����
 * ע��  ����*/
/*               ___                            ___
 * VSYNC:     __|   |__________________________|   |__     
 */
static void VSYNC_EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ; /*�����ش���*/
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_GenerateSWInterrupt(EXTI_Line0);	/*�жϹҵ� EXTI_Line0  ��*/
}

/************************************************
 * ��������VSYNC_Init
 * ����  ��OV7725 VSYNC�ж��������
 * ����  ����
 * ���  ����
 * ע��  ����
 ************************************************/
void VSYNC_Init(void)
{
    VSYNC_GPIO_Configuration();
    VSYNC_EXTI_Configuration();
    VSYNC_NVIC_Configuration();
}

/* ��������Sensor_Init
 * ����  ��Sensor��ʼ��
 * ����  ����
 * ���  ������1�ɹ�������0ʧ��
 * ע��  ����*/
ErrorStatus Ov7725_Init(void)
{
	uint16_t i = 0;
	uint8_t Sensor_IDCode = 0;	
	
	//DEBUG("ov7725 Register Config Start......");
	
	if( 0 == SCCB_WriteByte ( 0x12, 0x80 ) ) /*��λsensor */
	{
		//DEBUG("sccb write data error");		
		return ERROR ;
	}	

	if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, 0x0b ) )	 /* ��ȡsensor ID��*/
	{
		//DEBUG("read id faild");		
		return ERROR;
	}
	//DEBUG("Sensor ID is 0x%x", Sensor_IDCode);	
	
	if(Sensor_IDCode == OV7725_ID)
	{
		for( i = 0 ; i < OV7725_REG_NUM ; i++ )
		{
			if( 0 == SCCB_WriteByte(Sensor_Config[i].Address, Sensor_Config[i].Value) )
			{                
				//DEBUG("write reg faild", Sensor_Config[i].Address);
				return ERROR;
			}
		}
	}
	else
	{
		return ERROR;
	}
	//DEBUG("ov7725 Register Config Success");
	
	return SUCCESS;
}
//����320  �ߣ�240
void ImagDisp(void)
{
	uint16_t i, j;
	uint16_t Camera_Data;
	for(i = 0; i < 240; i++)
	{
		for(j = 0; j < 320; j++)
		{
			READ_FIFO_PIXEL(Camera_Data);		// ��FIFO����һ��rgb565���ص�Camera_Data����
			LCD_WR_Data(Camera_Data);
		}
	}
}
typedef char bool;
/*char biaozhun(void)
{	
	short int baohu,i,j,ktemp,xtemp,x[3],y[3],xielvs[5][2];
	unsigned short int Camera_Data,B,G,R,white,xielvi;
	char black,xielv;
	bool On_Off,useful;
	useful=0;
	ktemp=0;
	On_Off=0;
	xielv=0;
	xielvi=41;
	white=0xffff;
	for(i=0;i<240;++i)
	{
		if(ktemp==3||ktemp==2)
		{
		  On_Off=0;
			x[2]=(x[0]+x[1])/2; y[2]=y[0];
			if(x[1]-x[0]>150)useful=1;
			else On_Off=1;
			baohu=(x[0]+x[1])/2;
			if(x[2]<70) white=0xf800;
			else if(x[2]<120) white=0x7800;
			else if(x[2]<170) ;
			else if(x[2]<180) white=0x000f;
			else if(x[2]<320) white=0x001f;
			else;
		}
		else if(ktemp==1&&xielv<5)
		{
			if(i==xielvi)
			{
				  xielvi+=70;
			    xielvs[xielv][0]=x[0];
			    xielvs[xielv][1]=y[0];
					++xielv;
			}
		}
		else if(i==40) On_Off=1;
		else;
		if(i>200) On_Off=0;
		ktemp=0;
		black=0;
		for(j=0;j<320;++j)
		{
			READ_FIFO_PIXEL(Camera_Data);
			B=Camera_Data&0x001f;
			G=(Camera_Data>>5)&0x003f;
			R=(Camera_Data>>11)&0x001f;
			//if(B>16||G>32||R>16)
			if(B>10||G>25||R>10)
			{
			  LCD_WR_Data(white);
//				Mat[i/4][j/4+10]=255;
				if(black>10&&On_Off)
				{
					if(ktemp<3)
					{
				    x[ktemp]=(j+xtemp)/2;
					  y[ktemp]=i;
					}
	          black=0;
					++ktemp;
				} 	
			}
			else 
			{	
				LCD_WR_Data(0);
//				Mat[i/4][j/4+10]=0;
				if(On_Off)
				{
			      if(!black)xtemp=j;
				    ++black;
				}  
			}
		}
	}
	if(useful)
	{
	  if(baohu<70)  return  5;
	  else if(baohu<120)  return 4;
	  else if(baohu<170)  return 3;
	  else if(baohu<180)  return 2;
	  else if(baohu<320)  return 1;
	  else return 3;
	}
	else if(xielv>1)
	{
		--xielv;
		if(xielv==1)
		{
			if(xielvs[1][0]-xielvs[0][0]>7) return 4;
			else if(xielvs[1][0]-xielvs[0][0]<-7) return 2;
			else return 3;
		}
		else if(xielv==2)
		{
			if(xielvs[0][0]-xielvs[1][0]<xielvs[1][0]-xielvs[2][0]-2) return 2;
			else if(xielvs[1][0]-xielvs[0][0]>7&&xielvs[2][0]-xielvs[0][0]>10) return 4;
			else if(xielvs[1][0]-xielvs[0][0]<-7&&xielvs[2][0]-xielvs[0][0]<-10) return 2;
			else return 3;
		}
		else return 3;
	}
	else return 3;
}*/
/*����1��
char biaozhun(void)
{	
	short int baohu,i,j,ktemp,xtemp,x[3][3],y[3][3],xielvs[5][2];
	unsigned short int Camera_Data,B,G,R,white,xielvi;
	char black,rang,xielv;
	bool On_Off,useful;
	useful=0;
	ktemp=0;
	On_Off=0;
	xielv=0;
	xielvi=78;
	white=0xffff;
	rang=0;
	for(i=0;i<240;++i)
	{
		if(ktemp==3||ktemp==2)
		{
			if(rang>2) printf("rang>2");
			x[rang][2]=(x[rang][0]+x[rang][1])/2; y[rang][2]=y[rang][0];
			baohu=(x[rang][0]+x[rang][1])/2;
			if(x[rang][2]<70) white=0xf800;
			else if(x[rang][2]<130) white=0x7800;
			else if(x[rang][2]<170) ;
			else if(x[rang][2]<180) white=0x000f;
			else if(x[rang][2]<320) white=0x001f;
			else;
			if(x[rang][1]-x[rang][0]>150)
			{ 
				useful=1;
		    On_Off=0;
				rang++;
			}
		}
		else if(ktemp==1&&xielv<5)
		{
			if(i==xielvi)
			{
				  xielvi+=50;
			    xielvs[xielv][0]=x[rang][0];
			    xielvs[xielv][1]=y[rang][0];
					++xielv;
			}
		}
		else if(i==77||i==159) On_Off=1;
		else;
		if(i>220) On_Off=0;
		ktemp=0;
		black=0;
		for(j=0;j<320;++j)
		{
			READ_FIFO_PIXEL(Camera_Data);
			B=Camera_Data&0x001f;
			G=(Camera_Data>>5)&0x003f;
			R=(Camera_Data>>11)&0x001f;
			//if(B>16||G>32||R>16)
			if(B>10||G>25||R>10)
			{
			  LCD_WR_Data(white);
//				Mat[i/4][j/4+10]=255;
				if(black>10&&On_Off)
				{
					if(ktemp<3)
					{
				    x[rang][ktemp]=(j+xtemp)/2;
					  y[rang][ktemp]=i;
					}
	          black=0;
					++ktemp;
				} 	
			}
			else 
			{	
				LCD_WR_Data(0);
//				Mat[i/4][j/4+10]=0;
				if(On_Off)
				{
			      if(!black)xtemp=j;
				    ++black;
				}  
			}
		}
	}
	if(useful)
	{
		if(rang>=2)
		{
			if((x[0][2]>x[1][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/3))	
			{
				printf("�ҹ�"); return 4;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/3))
			{
				printf("���"); return 1;
			}
			else { printf("ֱ��");return 3;}
		}
	  else if(baohu<70)   {printf("ͼ����ҹ�"); return 5;}
	  else if(baohu<130)  {printf("ͼ���ҹ�"); return 4;}
	  else if(baohu<170)  {printf("ͼ��ֱ��"); return 3;}
	  else if(baohu<180)  {printf("ͼ�����"); return 2;}
	  else if(baohu<320)  {printf("ͼ������"); return 1;}
	  else {printf("ͼ��δ�ж�"); return 3;}
	}
	else if(xielv>1)
	{
		--xielv;
		if(xielv==1)
		{
			if(xielvs[1][0]-xielvs[0][0]>7) { printf("һ���ҹ�");return 4; }
			else if(xielvs[1][0]-xielvs[0][0]<-7) { printf("һ�����");return 1; }
			else { printf("һ��ֱ��");return 3; }
		}
		else if(xielv==2)
		{
			if(xielvs[0][0]-xielvs[1][0]<xielvs[1][0]-xielvs[2][0]-2) { printf("������");return 1; }
			else if(xielvs[1][0]-xielvs[0][0]>7&&xielvs[2][0]-xielvs[0][0]>10) { printf("һ���ҹ�");return 4; }
			else if(xielvs[1][0]-xielvs[0][0]<-7&&xielvs[2][0]-xielvs[0][0]<-10) { printf("һ�����");return 2; }
			else { printf("һ��ֱ��"); return 3; }
		}
		else { printf("б��ֱ��");return 3; }
	}
	else {printf("б��δ�ж�");return 3;}
}
/*����2��
char biaozhun(void)
{	
	short int baohu,i,j,ktemp,xtemp,x[3][3],y[3][3],xielvs[5][2];
	unsigned short int Camera_Data,B,G,R,white,xielvi;
	char black,rang,xielv;
	bool On_Off,useful;
	useful=0;
	ktemp=0;
	On_Off=0;
	xielv=0;
	xielvi=78;
	white=0xffff;
	rang=0;
	for(i=0;i<240;++i)
	{
		if(ktemp==3||ktemp==2)
		{
			if(rang>2) printf("rang>2");
			x[rang][2]=(x[rang][0]+x[rang][1])/2; y[rang][2]=y[rang][0];
			baohu=(x[rang][0]+x[rang][1])/2;
			if(x[rang][2]<70) white=0xf800;
			else if(x[rang][2]<130) white=0x7800;
			else if(x[rang][2]<170) ;
			else if(x[rang][2]<180) white=0x000f;
			else if(x[rang][2]<320) white=0x001f;
			else;
			if(x[rang][1]-x[rang][0]>150)
			{ 
				useful=1;
		    On_Off=0;
				rang++;
			}
		}
		else if(ktemp==1&&xielv<5)
		{
			if(i==xielvi)
			{
				  xielvi+=50;
			    xielvs[xielv][0]=x[rang][0];
			    xielvs[xielv][1]=y[rang][0];
					++xielv;
			}
		}
		else if(i==77||i==159) On_Off=1;
		else;
		if(i>220) On_Off=0;
		ktemp=0;
		black=0;
		for(j=0;j<320;++j)
		{
			READ_FIFO_PIXEL(Camera_Data);
			B=Camera_Data&0x001f;
			G=(Camera_Data>>5)&0x003f;
			R=(Camera_Data>>11)&0x001f;
			//if(B>16||G>32||R>16)
			if(B>10||G>25||R>10)
			{
			  LCD_WR_Data(white);
//				Mat[i/4][j/4+10]=255;
				if(black>10&&On_Off)
				{
					if(ktemp<3)
					{
				    x[rang][ktemp]=(j+xtemp)/2;
					  y[rang][ktemp]=i;
					}
	          black=0;
					++ktemp;
				} 	
			}
			else 
			{	
				LCD_WR_Data(0);
//				Mat[i/4][j/4+10]=0;
				if(On_Off)
				{
			      if(!black)xtemp=j;
				    ++black;
				}  
			}
		}
	}
	if(useful)
	{
		if(rang>=2)
		{
			if((x[0][2]>x[1][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/3))	
			{
				printf("�ҹ�"); return 4;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/3))
			{
				printf("���"); return 1;
			}
			else { printf("ֱ��");return 3;}
		}
	  else if(baohu<70)   {printf("ͼ����ҹ�"); return 5;}
	  else if(baohu<130)  {printf("ͼ���ҹ�"); return 4;}
	  else if(baohu<170)  {printf("ͼ��ֱ��"); return 3;}
	  else if(baohu<180)  {printf("ͼ�����"); return 2;}
	  else if(baohu<320)  {printf("ͼ������"); return 1;}
	  else {printf("ͼ��δ�ж�"); return 3;}
	}
	else if(xielv>1)
	{
		--xielv;
		if(xielv==1)
		{
			if(xielvs[1][0]-xielvs[0][0]>7) { printf("һ���ҹ�");return 4; }
			else if(xielvs[1][0]-xielvs[0][0]<-7) { printf("һ�����");return 1; }
			else { printf("һ��ֱ��");return 3; }
		}
		else if(xielv==2)
		{
			if(xielvs[0][0]-xielvs[1][0]<xielvs[1][0]-xielvs[2][0]-2) { printf("������");return 1; }
			else if(xielvs[1][0]-xielvs[0][0]>7&&xielvs[2][0]-xielvs[0][0]>10) { printf("һ���ҹ�");return 4; }
			else if(xielvs[1][0]-xielvs[0][0]<-7&&xielvs[2][0]-xielvs[0][0]<-10) { printf("һ�����");return 2; }
			else { printf("һ��ֱ��"); return 3; }
		}
		else { printf("б��ֱ��");return 3; }
	}
	else {printf("б��δ�ж�");return 3;}
}
char biaozhun(void)
{	
	short int baohu,i,j,ktemp,xtemp,x[3][3],y[3][3],xielvs[5][2];
	unsigned short int Camera_Data,B,G,R,white,xielvi;
	char black,rang,xielv;
	bool On_Off,useful;
	useful=0;
	ktemp=0;
	On_Off=0;
	xielv=0;
	xielvi=67;
	white=0xffff;
	rang=0;
	for(i=0;i<240;++i)
	{
		if(ktemp==3||ktemp==2)
		{
			if(rang>3) printf("rang>2");
			x[rang][2]=(x[rang][0]+x[rang][1])/2; y[rang][2]=y[rang][0];
			baohu=(x[rang][0]+x[rang][1])/2;
			if(x[rang][2]<70) white=0xf800;
			else if(x[rang][2]<135) white=0x7800;
			else if(x[rang][2]<170) ;
			else if(x[rang][2]<180) white=0x000f;
			else if(x[rang][2]<190) white=0x001f;
			else if(x[rang][2]<320) white=0xf81f;
			else;
			if(x[rang][1]-x[rang][0]>150)
			{ 
				useful=1;
		    On_Off=0;
				rang++;
				printf("x[%d][2]=%d\n",rang,x[rang][2]);
			}
		}
		else if(ktemp==1&&xielv<5)
		{
			if(i==xielvi)
			{
				  xielvi+=66;
			    xielvs[xielv][0]=x[rang][0];
			    xielvs[xielv][1]=y[rang][0];
				  printf("xielvs[%d][0]=%d,xielvs[%d][1]=%d\n",xielv,xielvs[xielv][0],xielv,xielvs[xielv][1]);
				  ++xielv;
			}
		}
		else if(i==66||i==126||i==186) On_Off=1;
		else;
		if(i>220) On_Off=0;
		ktemp=0;
		black=0;
		for(j=0;j<320;++j)
		{
			READ_FIFO_PIXEL(Camera_Data);
			B=Camera_Data&0x001f;
			G=(Camera_Data>>5)&0x003f;
			R=(Camera_Data>>11)&0x001f;
			//if(B>16||G>32||R>16)
			if(B>=12||G>23||R>=12)
			{
			  LCD_WR_Data(white);
//				Mat[i/4][j/4+10]=255;
				if(black>10&&On_Off)
				{
					if(ktemp<3)
					{
				    x[rang][ktemp]=(j+xtemp)/2;
					  y[rang][ktemp]=i;
					}
	          black=0;
					++ktemp;
				} 
				//else	printf("��=%d",black);			
			}
			else 
			{	
				LCD_WR_Data(0);
//				Mat[i/4][j/4+10]=0;
				if(On_Off)
				{
			      if(!black)xtemp=j;
				    ++black;
				}  
			}
		}
	}
	if(useful)
	{
		if(rang==2)
		{
			if((x[0][2]>x[1][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/2))	
			{
				printf("2�ҹ�"); return 6;
			}
			else if((x[0][2]>x[1][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/3))	
			{
				printf("2�ҹ�"); return 5;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/2))
			{
				printf("2���"); return 1;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/3))
			{
				printf("2���"); return 2;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/4))
			{
				printf("2���"); return 3;
			}
			else { printf("2ֱ��");return 4;}
		}
		else if(rang==3)
		{
			if((x[0][2]>x[2][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/2)&&((x[0][2]-x[2][2])>(y[2][2]-y[0][2])/2))	
			{
				printf("3�ҹ�"); return 6;
			}
			else if((x[0][2]>x[2][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/3)&&((x[0][2]-x[2][2])>(y[2][2]-y[0][2])/3))	
			{
				printf("3�ҹ�"); return 5;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/4)&&((x[2][2]-x[1][2])>(y[2][2]-y[1][2])/3))
			{
				printf("3���"); return 1;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/4)&&((x[2][2]-x[1][2])>(y[2][2]-y[1][2])/4))
			{
				printf("3���"); return 2;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/5)&&((x[2][2]-x[1][2])>(y[2][2]-y[1][2])/4))
			{
				printf("3���"); return 3;
			}
			else { printf("3ֱ��");return 4;}
		}
	  else if(baohu<90)   {printf("ͼ����ҹ�"); return 6;}
	  else if(baohu<135)  {printf("ͼ���ҹ�"); return 5;}
	  else if(baohu<170)  {printf("ͼ��ֱ��"); return 4;}
	  else if(baohu<180)  {printf("ͼ��С���"); return 3;}
	  else if(baohu<190)  {printf("ͼ�����"); return 2;}
		else if(baohu<320)  {printf("ͼ������"); return 1;}
	  else {printf("ͼ��δ�ж�"); return 4;}
	}
	else if(xielv>1)
	{
		--xielv;
		if(xielv==1)
		{
			if(xielvs[1][0]-xielvs[0][0]>7) { printf("һ���ҹ�");return 6; }
			else if(xielvs[1][0]-xielvs[0][0]<-7) { printf("һ�����");return 2; }
			else { printf("һ��ֱ��");return 4; }
		}
		else if(xielv==2)
		{
			if(xielvs[0][0]-xielvs[1][0]<xielvs[1][0]-xielvs[2][0]-2) { printf("������");return 1; }
			else if(xielvs[1][0]-xielvs[0][0]>7&&xielvs[2][0]-xielvs[0][0]>12) { printf("һ���ҹ�");return 6; }
			else if(xielvs[1][0]-xielvs[0][0]<-7&&xielvs[2][0]-xielvs[0][0]<-12) { printf("һ�����");return 2; }
			else { printf("һ��ֱ��"); return 4; }
		}
		else { printf("б��ֱ��");return 4; }
	}
	else {printf("б��δ�ж�");return 4;}
}*//*
char biaozhun(void)
{	
	short int baohu,i,j,ktemp,xtemp,x[3][3],y[3][3],xielvs[5][2];
	unsigned short int Camera_Data,B,G,R,white,xielvi;
	char black,rang,xielv;
	bool On_Off,useful;
	useful=0;
	ktemp=0;
	On_Off=0;
	xielv=0;
	xielvi=67;
	white=0xffff;
	rang=0;
	for(i=0;i<240;++i)
	{
		if(ktemp==3||ktemp==2)
		{
			if(rang>3) printf("rang>2");
			x[rang][2]=(x[rang][0]+x[rang][1])/2; y[rang][2]=y[rang][0];
			baohu=(x[rang][0]+x[rang][1])/2;
			if(x[rang][2]<100) white=0xf800;
			else if(x[rang][2]<140) white=0x7800;
			else if(x[rang][2]<180) ;
			else if(x[rang][2]<190) white=0x000f;
			else if(x[rang][2]<210) white=0x001f;
			else if(x[rang][2]<320) white=0xf81f;
			else;
			if(x[rang][1]-x[rang][0]>150)
			{ 
				useful=1;
		    On_Off=0;
				rang++;
				printf("x[%d][2]=%d\n",rang,x[rang][2]);
			}
		}
		else if(ktemp==1&&xielv<5)
		{
			if(i==xielvi)
			{
				  xielvi+=66;
			    xielvs[xielv][0]=x[rang][0];
			    xielvs[xielv][1]=y[rang][0];
				  printf("xielvs[%d][0]=%d,xielvs[%d][1]=%d\n",xielv,xielvs[xielv][0],xielv,xielvs[xielv][1]);
				  ++xielv;
			}
		}
		else if(i==66||i==126||i==186) On_Off=1;
		else;
		if(i>220) On_Off=0;
		ktemp=0;
		black=0;
		for(j=0;j<320;++j)
		{
			READ_FIFO_PIXEL(Camera_Data);
			B=Camera_Data&0x001f;
			G=(Camera_Data>>5)&0x003f;
			R=(Camera_Data>>11)&0x001f;
			//if(B>16||G>32||R>16)
			if(B>=12||G>23||R>=12)
			{
			  LCD_WR_Data(white);
//				Mat[i/4][j/4+10]=255;
				if(black>10&&On_Off)
				{
					if(ktemp<3)
					{
				    x[rang][ktemp]=(j+xtemp)/2;
					  y[rang][ktemp]=i;
					}
	          black=0;
					++ktemp;
				} 
				//else	printf("��=%d",black);			
			}
			else 
			{	
				LCD_WR_Data(0);
//				Mat[i/4][j/4+10]=0;
				if(On_Off)
				{
			      if(!black)xtemp=j;
				    ++black;
				}  
			}
		}
	}
	if(useful)
	{
		if(baohu<100)   {printf("ͼ����ҹ�"); return 6;}
	  else if(baohu<140)  {printf("ͼ���ҹ�"); return 5;}
	  else if(baohu<190)  {printf("ͼ��ֱ��"); return 4;}
	  else if(baohu<200)  {printf("ͼ��С���"); return 3;}
	  else if(baohu<210)  {printf("ͼ�����"); return 2;}
		else if(baohu<320)  {printf("ͼ������"); return 1;}
		else if(rang==2)
		{
			if((x[0][2]>x[1][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/2))	
			{
				printf("2�ҹ�"); return 6;
			}
			else if((x[0][2]>x[1][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/3))	
			{
				printf("2�ҹ�"); return 5;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/2))
			{
				printf("2���"); return 1;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/3))
			{
				printf("2���"); return 2;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/4))
			{
				printf("2���"); return 3;
			}
			else { printf("2ֱ��");return 4;}
		}
		else if(rang==3)
		{
			if((x[0][2]>x[2][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/2)&&((x[0][2]-x[2][2])>(y[2][2]-y[0][2])/2))	
			{
				printf("3�ҹ�"); return 6;
			}
			else if((x[0][2]>x[2][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/3)&&((x[0][2]-x[2][2])>(y[2][2]-y[0][2])/3))	
			{
				printf("3�ҹ�"); return 5;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/4)&&((x[2][2]-x[1][2])>(y[2][2]-y[1][2])/3))
			{
				printf("3���"); return 1;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/4)&&((x[2][2]-x[1][2])>(y[2][2]-y[1][2])/4))
			{
				printf("3���"); return 2;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/5)&&((x[2][2]-x[1][2])>(y[2][2]-y[1][2])/4))
			{
				printf("3���"); return 3;
			}
			else { printf("3ֱ��");return 4;}
		}
	  else {printf("ͼ��δ�ж�"); return 4;}
	}
	else if(xielv>1)
	{
		--xielv;
		if(xielv==1)
		{
			if(xielvs[1][0]-xielvs[0][0]>7) { printf("һ���ҹ�");return 6; }
			else if(xielvs[1][0]-xielvs[0][0]<-7) { printf("һ�����");return 2; }
			else { printf("һ��ֱ��");return 4; }
		}
		else if(xielv==2)
		{
			if(xielvs[0][0]-xielvs[1][0]<xielvs[1][0]-xielvs[2][0]-2) { printf("������");return 1; }
			else if(xielvs[1][0]-xielvs[0][0]>7&&xielvs[2][0]-xielvs[0][0]>12) { printf("һ���ҹ�");return 6; }
			else if(xielvs[1][0]-xielvs[0][0]<-7&&xielvs[2][0]-xielvs[0][0]<-12) { printf("һ�����");return 2; }
			else { printf("һ��ֱ��"); return 4; }
		}
		else { printf("б��ֱ��");return 4; }
	}
	else {printf("б��δ�ж�");return 4;}
}*/
char biaozhun(void)
{	
	short int baohu,i,j,ktemp,xtemp,x[3][3],y[3][3],xielvs[5][2];
	unsigned short int Camera_Data,B,G,R,white,xielvi;
	char black,rang,xielv;
	bool On_Off,useful;
	useful=0;
	ktemp=0;
	On_Off=0;
	xielv=0;
	//xielvi=67;
	xielvi=78;
	white=0xffff;
	rang=0;
	for(i=0;i<240;++i)
	{
		if(ktemp==3||ktemp==2)
		{
			if(rang>3) printf("rang>2");
			x[rang][2]=(x[rang][0]+x[rang][1])/2; y[rang][2]=y[rang][0];
			baohu=(x[rang][0]+x[rang][1])/2;
			if(x[rang][2]<100) white=0xf800;
			else if(x[rang][2]<135) white=0x7800;
			else if(x[rang][2]<180) ;
			else if(x[rang][2]<200) white=0x000f;
			else if(x[rang][2]<210) white=0x001f;
			else if(x[rang][2]<320) white=0xf81f;
			else;
			if(x[rang][1]-x[rang][0]>150)
			{ 
				useful=1;
		    On_Off=0;
				rang++;
				printf("x[%d][2]=%d\n",rang,x[rang][2]);
			}
		}
		else if(ktemp==1&&xielv<5)
		{
			if(i==xielvi)
			{
				  xielvi+=66;
			    xielvs[xielv][0]=x[rang][0];
			    xielvs[xielv][1]=y[rang][0];
				  printf("xielvs[%d][0]=%d,xielvs[%d][1]=%d\n",xielv,xielvs[xielv][0],xielv,xielvs[xielv][1]);
				  ++xielv;
			}
		}
		//else if(i==66||i==126||i==186) On_Off=1;
		else if(i==77||i==130||i==183) On_Off=1;
		else;
		if(i>220) On_Off=0;
		ktemp=0;
		black=0;
		for(j=0;j<320;++j)
		{
			READ_FIFO_PIXEL(Camera_Data);
			B=Camera_Data&0x001f;
			G=(Camera_Data>>5)&0x003f;
			R=(Camera_Data>>11)&0x001f;
			//if(B>16||G>32||R>16)
			if(B>=12||G>=25||R>=12)
			{
			  LCD_WR_Data(white);
//				Mat[i/4][j/4+10]=255;
				if(black>7&&On_Off)
				{
					if(ktemp<3)
					{
				    x[rang][ktemp]=(j+xtemp)/2;
					  y[rang][ktemp]=i;
					}
	          black=0;
					++ktemp;
				} 
				//else	printf("��=%d",black);			
			}
			else 
			{	
				LCD_WR_Data(0);
//				Mat[i/4][j/4+10]=0;
				if(On_Off)
				{
			      if(!black)xtemp=j;
				    ++black;
				}  
			}
		}
	}
	if(useful)
	{
		if(rang==2)
		{
			if(baohu<120)   {printf("2ͼ����ҹ�"); return 5;}
			else if(baohu>220)  {printf("ͼ��С���"); return 1;}
			else if(baohu>190)  {printf("ͼ��С���"); return 2;}
			else ;
			if((x[0][2]>x[1][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/2))	
			{
				printf("2�ҹ�"); return 6;
			}
			else if((x[0][2]>x[1][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/3))	
			{
				printf("2�ҹ�"); return 5;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/2))
			{
				printf("2���"); return 1;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/3))
			{
				printf("2���"); return 2;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/4))
			{
				printf("2���"); return 3;
			}
			else { printf("goto ZZU"); goto ZZU; }
		}
		else if(rang==3)
		{
			if(baohu<120)   {printf("2ͼ����ҹ�"); return 5;}
			else if(baohu>220)  {printf("ͼ��С���"); return 1;}
			else if(baohu>190)  {printf("ͼ��С���"); return 2;}
			else ;
			if((x[0][2]>x[2][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/2)&&((x[0][2]-x[2][2])>(y[2][2]-y[0][2])/2))	
			{
				printf("3�ҹ�"); return 6;
			}
			else if((x[0][2]>x[2][2])&&((x[0][2]-x[1][2])>(y[1][2]-y[0][2])/3)&&((x[0][2]-x[2][2])>(y[2][2]-y[0][2])/3))	
			{
				printf("3�ҹ�"); return 5;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/3)&&(((x[2][2]-x[1][2])>(y[2][2]-y[1][2])*2)/5))
			{
				printf("3���"); return 1;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/4)&&(((x[2][2]-x[1][2])>(y[2][2]-y[1][2])*2)/5))
			{
				printf("3���"); return 2;
			}
			else if((x[0][2]<x[1][2])&&((x[1][2]-x[0][2])>(y[1][2]-y[0][2])/4)&&((x[2][2]-x[1][2])>(y[2][2]-y[1][2])/4))
			{
				printf("3���"); return 3;
			}
			else { printf("goto ZZU"); goto ZZU; }
		}
		else ;
	  ZZU: if(baohu<100)   {printf("ͼ����ҹ�"); return 6;}
	  else if(baohu<135)  {printf("ͼ���ҹ�"); return 5;}
	  else if(baohu<180)  {printf("ͼ��ֱ��"); return 4;}
	  else if(baohu<190)  {printf("ͼ��С���"); return 3;}
	  else if(baohu<220)  {printf("ͼ�����"); return 2;}
		else if(baohu<320)  {printf("ͼ������"); return 1;}
	  else {printf("ͼ��δ�ж�"); return 4;}
	}
	else if(xielv>1)
	{
		--xielv;
		if(xielv==1)
		{
			if(xielvs[1][0]-xielvs[0][0]>7) { printf("һ���ҹ�");return 6; }
			else if(xielvs[1][0]-xielvs[0][0]<-7) { printf("һ�����");return 2; }
			else { printf("һ��ֱ��");return 4; }
		}
		else if(xielv==2)
		{
			if(xielvs[0][0]-xielvs[1][0]<xielvs[1][0]-xielvs[2][0]-2) { printf("������");return 1; }
			else if(xielvs[1][0]-xielvs[0][0]>7&&xielvs[2][0]-xielvs[0][0]>12) { printf("һ���ҹ�");return 6; }
			else if(xielvs[1][0]-xielvs[0][0]<-7&&xielvs[2][0]-xielvs[0][0]<-12) { printf("һ�����");return 2; }
			else { printf("һ��ֱ��"); return 4; }
		}
		else { printf("б��ֱ��");return 4; }
	}
	else {printf("б��δ�ж�");return 4;}
}