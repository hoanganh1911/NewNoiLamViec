/*
 * lcd_screw.h
 *
 *  Created on: Feb 27, 2023
 *      Author: hoanganh1911
 */

#ifndef LCD_SCREW_H_
#define LCD_SCREW_H_
/*---------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"
/*---------------------------------------------------*/
/* Định nghĩa các chân kết nối từ LCD vào các ngõ ra của IC HC595( D0 <-> 0 ....D7 <-> 7 )*/

/* Các chân điều khiển */
#define EN_PIN 0	// Chân enable kích hoạt cạnh lên
#define RS_PIN 2 	// Chân này dùng để xác định dữ liệu gửi vào là dữ liệu hay lệnh ( 0 : lệnh ; 1 : dữ liệu )

/* Các chân truyền dữ liệu */
#define D4_PIN 3
#define D5_PIN 4
#define D6_PIN 5
#define D7_PIN 6

/* Chân nền */
#define BL_PIN 7
/*---------------------------------------------------*/
void hc595_int(GPIO_TypeDef *GPIO_CLK,uint32_t GPIO_PIN_CLK,GPIO_TypeDef *GPIO_DATA,uint32_t GPIO_PIN_DATA,GPIO_TypeDef *GPIO_LATCH,uint32_t GPIO_PIN_LATCH);
void lcd_Init(void);
void lcd_Send_String(char *str);
void lcd_Clear(void);
void lcd_Put_Cur(int row,int col);
void screw_Done_Show(int c);
void screw_Set_Show(int c);
/*---------------------------------------------------*/

#endif /* LCD_SCREW_H_ */