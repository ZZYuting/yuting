#ifndef _HM_HMC5883_H_
#define _HM_HMC5883_H_
#include <stm32f10x.h>
#include "stdbool.h"
#define SCL_H1         GPIOB->BSRR = GPIO_Pin_2
#define SCL_L1        GPIOB->BRR  = GPIO_Pin_2 
   
#define SDA_H1         GPIOB->BSRR = GPIO_Pin_1
#define SDA_L1         GPIOB->BRR  = GPIO_Pin_1

#define SCL_read1     GPIOB->IDR  & GPIO_Pin_2
#define SDA_read1      GPIOB->IDR  & GPIO_Pin_1
//hmc5883 address
//*****************************************
#define WD_DEVICE_ADDR  0x3C
#define HxH_DATD_ADDR 0x03
#define HxL_DATD_ADDR 0x04
#define HyH_DATD_ADDR 0x07
#define HyL_DATD_ADDR 0x08
#define HzH_DATD_ADDR 0x05
#define Hzl_DATD_ADDR 0x06
extern short Hx,Hy,Hz;
void HMC5883_init(void);
void HMC_delay(void);
void HMCdelay5ms(void);
bool HMC_Start(void);
void HMC_Stop(void);
void HMC_Ack(void);
void HMC_NoAck(void);
bool HMC_WaitAck(void);
void HMC_SendByte(u8 SendByte);
unsigned char HMC_RadeByte(void);
bool HMCSingle_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char HMCSingle_Read(unsigned char SlaveAddress,unsigned char REG_Address);
void READ_HMC5883(void);
#endif



