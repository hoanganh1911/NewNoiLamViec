/*
 * TFT.h
 *
 *  Created on: Mar 21, 2023
 *      Author: hoanganh
 */

#ifndef TFT_H_
#define TFT_H_

/*--------------------------------------------------------------------------------------*/
#include "main.h"
#include "stdbool.h"
/*--------------------------------------------------------------------------------------*/
#define  BLACK 		0x0000
#define  BLUE 		0x001F
#define  RED 		0x0F800
#define  GREEN 		0x07E0
#define  CYAN 		0x07FF
#define  MAGENTA 	0xF81F
#define  YELLOW 	0xFFE0
#define  WHITE 		0xFFFF
/*--------------------------------------------------------------------------------------*/
#define  WIDTH    ((uint16_t)320)
#define  HEIGHT   ((uint16_t)480)
/*-------------------------------------User_Setting-------------------------------------*/
#define RD_PORT 		TFT_RD_GPIO_Port
#define RD_PIN  		TFT_RD_Pin
#define WR_PORT 		TFT_WR_GPIO_Port
#define WR_PIN  		TFT_WR_Pin
#define CD_PORT 		TFT_RS_GPIO_Port   	// RS PORT
#define CD_PIN  		TFT_RS_Pin     		// RS PIN
#define CS_PORT 		TFT_CS_GPIO_Port
#define CS_PIN  		TFT_CS_Pin
#define RESET_PORT 		TFT_RST_GPIO_Port
#define RESET_PIN  		TFT_RST_Pin

#define D0_PORT 		TFT_DB0_GPIO_Port
#define D0_PIN 			TFT_DB0_Pin
#define D1_PORT 		TFT_DB1_GPIO_Port
#define D1_PIN 			TFT_DB1_Pin
#define D2_PORT 		TFT_DB2_GPIO_Port
#define D2_PIN 			TFT_DB2_Pin
#define D3_PORT 		TFT_DB3_GPIO_Port
#define D3_PIN 			TFT_DB3_Pin
#define D4_PORT 		TFT_DB4_GPIO_Port
#define D4_PIN 			TFT_DB4_Pin
#define D5_PORT 		TFT_DB5_GPIO_Port
#define D5_PIN 			TFT_DB5_Pin
#define D6_PORT 		TFT_DB6_GPIO_Port
#define D6_PIN 			TFT_DB6_Pin
#define D7_PORT 		TFT_DB7_GPIO_Port
#define D7_PIN 			TFT_DB7_Pin
/*--------------------------------------------------------------------------------------*/
#define write_8(dt) { 												\
	GPIOA->BSRR = 0x0F80 << 16;                                  	\
	GPIOB->BSRR = 0x7800 << 16; 								 	\
	GPIOA->BSRR = (dt & (1 << 0) << 11) | (dt & (1 << 1) << 9) 		\
				| (dt & (1 << 2) << 7) 	| (dt & (1 << 3) << 5);		\
	GPIOB->BSRR = (dt & (1 << 4) << 11) | (dt & (1 << 5) << 9)		\
				| (dt & (1 << 6) << 7)  | (dt & (1 << 7) << 5);		\
}
#define read_8()	(((GPIOA->IDR & (1 << 11 )) >> 11)	|	((GPIOA->IDR & (1 << 10 ))  >> 9) 	\
					| ((GPIOA->IDR & (1 << 9  )) >>  7)	|	((GPIOA->IDR & (1 << 8  )) 	>> 5) 	\
					| ((GPIOB->IDR & (1 << 15 )) >> 11)	|	((GPIOB->IDR & (1 << 14 ))	>> 9) 	\
					| ((GPIOB->IDR & (1 << 13 )) >> 7)	| 	((GPIOB->IDR & (1 << 12 ))  >> 5))
/*--------------------------------------------------------------------------------------*/
#define WRITE_DELAY { WR_ACTIVE2; }
#define READ_DELAY  { RD_ACTIVE4; }
#define TFTLCD_DELAY 0xFFFF
#define TFTLCD_DELAY8 0x7F
/*--------------------------------------------------------------------------------------*/
#define CD_COMMAND 		PIN_LOW(CD_PORT, CD_PIN)
#define write8(x)     	{ write_8(x); WRITE_DELAY; WR_STROBE; WR_IDLE; }
#define write16(x)    	{ uint8_t h = (x)>>8, l = x; write8(h); write8(l); }
#define WriteCmd(x)  	{ CD_COMMAND; write16(x); CD_DATA; }
#define WriteData(x) 	{ write16(x); }
/*--------------------------------------------------------------------------------------*/
void TFT_init(void);
#endif /* TFT_H_ */
