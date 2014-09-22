#ifndef _12864_H__
#define _12864_H__
#include "stm32f10x.h"

void Lcd_Init(void);
void LCD12864_Set_XY( u8 X, u8 Y );
void LCD12864_Clr_Scr(void);
void LCD12864_Write_Number(u8 s);
void LCD12864_Write_String(u8 X, u8 Y, uc8  *s);
void LCD12864_Display_Img(u8 const *img);
static void Send_Byte(u8 bbyte);
static void Write_Char(u8 start, u8 ddata);
#endif
