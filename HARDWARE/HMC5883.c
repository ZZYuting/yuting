//******��ȡMPU6050����****************************************
#include <HMC5883.h>
#include "delay.h"
//#include <math.h>
short Hx,Hy,Hz;//��������x��y��z��ǿ�� 
unsigned char BUF3[9];//�������ݻ�����

void HMC5883_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void HMC_delay(void)
{
		
   u8 i=30; //?a��??����?��??��?��?��	��??-2a��?��?�̨���?5?1?��D�䨨?
   while(i) 
   { 
     i--; 
   }  
}

void HMCdelay5ms(void)
{
		
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}
/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
bool HMC_Start(void)
{
	SDA_H1;
	SCL_H1;
	HMC_delay();
	if(!SDA_read1)return false;	//SDA???a�̨���????������???|,��?3?
	SDA_L1;
	HMC_delay();
	if(SDA_read1) return false;	//SDA???a??��????������??3?�䨪,��?3?
	SDA_L1;
	HMC_delay();
	return true;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void HMC_Stop(void)
{
	SCL_L1;
	HMC_delay();
	SDA_L1;
	HMC_delay();
	SCL_H1;
	HMC_delay();
	SDA_H1;
	HMC_delay();
} 
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void HMC_Ack(void)
{	
	SCL_L1;
	HMC_delay();
	SDA_L1;
	HMC_delay();
	SCL_H1;
	HMC_delay();
	SCL_L1;
	HMC_delay();
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void HMC_NoAck(void)
{	
	SCL_L1;
	HMC_delay();
	SDA_H1;
	HMC_delay();
	SCL_H1;
	HMC_delay();
	SCL_L1;
	HMC_delay();
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
bool HMC_WaitAck(void) 	 //����???a:=1��DACK,=0?TACK
{
	SCL_L1;
	HMC_delay();
	SDA_H1;			
	HMC_delay();
	SCL_H1;
	HMC_delay();
	if(SDA_read1)
	{
      SCL_L1;
	  HMC_delay();
      return false;
	}
	SCL_L1;
	HMC_delay();
	return true;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void HMC_SendByte(u8 SendByte) //��y?Y�䨮????��?�̨�??//
{
    u8 i=8;
    while(i--)
    {
        SCL_L1;
        HMC_delay();
      if(SendByte&0x80)
        SDA_H1;  
      else 
        SDA_L1;   
        SendByte<<=1;
        HMC_delay();
		SCL_H1;
        HMC_delay();
    }
    SCL_L1;
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
unsigned char HMC_RadeByte(void)  //��y?Y�䨮????��?�̨�??//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H1;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L1;
      HMC_delay();
	  SCL_H1;
      HMC_delay();	
      if(SDA_read1)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L1;
    return ReceiveByte;
} 

//ZRX          
//���ֽ�д��*******************************************

bool HMCSingle_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!HMC_Start())return false;
    HMC_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!HMC_WaitAck()){HMC_Stop(); return false;}
    HMC_SendByte(REG_Address );   //���õ���ʼ��ַ      
    HMC_WaitAck();	
    HMC_SendByte(REG_data);
    HMC_WaitAck();   
    HMC_Stop(); 
    HMCdelay5ms();
    return true;
}

//���ֽڶ�ȡ*****************************************
unsigned char HMCSingle_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   unsigned char REG_data;     	
	if(!HMC_Start())return false;
    HMC_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!HMC_WaitAck()){HMC_Stop(); return false;}
    HMC_SendByte((u8) REG_Address);   //���õ���ʼ��ַ      
    HMC_WaitAck();
    HMC_Start();
    HMC_SendByte(SlaveAddress+1);
    HMC_WaitAck();

	REG_data= HMC_RadeByte();
    HMC_NoAck();
    HMC_Stop();
    //return true;
	return REG_data;

}						      

void READ_HMC5883(void)
{
     int i;
     unsigned address;
     HMCSingle_Write(0x3c,0x02,0x00);
		 delay_ms(7);
     for(i=0;i<=5;i++)
     {
        address=0x03+i;
        BUF3[i]=HMCSingle_Read(0x3C,address) ; 
     }
     Hx=(BUF3[0]<<8)+BUF3[1];
     Hz=(BUF3[2]<<8)+BUF3[3];
     Hy=(BUF3[4]<<8)+BUF3[5];                 
}
