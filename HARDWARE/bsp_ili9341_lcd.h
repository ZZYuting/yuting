#ifndef __BSP_ILI9341_LCD_H
#define	__BSP_ILI9341_LCD_H

#include "stm32f10x.h"

/***************************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-BORSRAM1 连接 TFT，地址范围为0X6000 0000 ~ 0X63FF FFFF
FSMC_A16 接LCD的DC(寄存器/数据选择)脚
寄存器基地址 = 0X60000000
RAM基地址 = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
当选择不同的地址线时，地址要重新计算  
****************************************************************************************/

#define Bank1_LCD_C    ((u32)0x60000000)	   //Disp Reg ADDR
#define Bank1_LCD_D    ((u32)0x60020000)     //Disp Data ADDR       // A16 PD11

/*选定LCD指定寄存器*/
#define LCD_WR_REG(index)    ((*(__IO u16 *) (Bank1_LCD_C)) = ((u16)index))
/*往LCD GRAM写入数据*/
#define LCD_WR_Data(val)       ((*(__IO u16 *) (Bank1_LCD_D)) = ((u16)(val)))

#define LCD_ILI9341_CMD(index)       LCD_WR_REG(index)
#define LCD_ILI9341_Parameter(val)	 LCD_WR_Data(val)

void LCD_Init(void);
uint16_t LCD_RD_data(void);

#endif /* __BSP_ILI9341_LCD_H */
