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
//#include "stdint.h"
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

typedef struct {
	GPIO_TypeDef *CLK_Port;
	uint16_t CLK_Pin;
	GPIO_TypeDef *LATCH_Port;
	uint16_t LATCH_Pin;
	GPIO_TypeDef *DATA_Port;
	uint16_t DATA_Pin;
} hc595_t;
/*---------------------------------------------------*/
void hc595_int(hc595_t *conf);
void lcd_Init(void);
void lcd_Send_String(char *str);
void lcd_Clear(void);
void lcd_Put_Cur(int row,int col);
void screw_Done_Show(int c);
void screw_Set_Show(int c);
/*---------------------------------------------------*/

#endif /* LCD_SCREW_H_ */
