/*
 * lcd_screw.c
 *
 *  Created on: Feb 27, 2023
 *      Author: hoanganh1911
 */
/* Hàm truyền dữ liệu : Mỗi lần gửi bit từ MSB -> LSB */
/*---------------------------------------------------*/
#include "lcd_screw.h"
/*---------------------------------------------------*/
hc595_t *_hc595;
void hc595_int(hc595_t *conf)
{
	_hc595 = conf;
}
void hc595_trans(uint8_t c)
{
	for(int i = 0;i<8;i++)
	{
		uint8_t bit_trans = (c & (0x80>>i))>>(7-i);
		HAL_GPIO_WritePin(_hc595 -> DATA_Port, _hc595 -> DATA_Pin, bit_trans);
		HAL_GPIO_WritePin(_hc595 -> CLK_Port, _hc595 -> CLK_Pin, 1);
		HAL_GPIO_WritePin(_hc595 -> CLK_Port, _hc595 -> CLK_Pin, 0);
	}
	HAL_GPIO_WritePin(_hc595 -> LATCH_Port, _hc595 -> LATCH_Pin, 1);
	HAL_GPIO_WritePin(_hc595 -> LATCH_Port, _hc595 -> LATCH_Pin, 0);
}
void lcd_Send_Cmd(char cmd)
{

	char data_u, data_l; // vi du 0x30
	uint8_t data_t[4];
	data_u = (cmd >> 4) & 0x0f; // data_u =0x03
	data_l = (cmd & 0x0f); // data_l = 0x00

	data_t[0] = (data_u & 0x01) << D4_PIN | (data_u & 0x02 ) << (D5_PIN-1)  | (data_u & 0x04) << (D6_PIN-2)  | (data_u & 0x08) << (D7_PIN-3) | (1<<EN_PIN)| (0<<RS_PIN)  | (1<< BL_PIN);
	data_t[1] = (data_u & 0x01) << D4_PIN | (data_u & 0x02 ) << (D5_PIN-1)  | (data_u & 0x04) << (D6_PIN-2)  | (data_u & 0x08) << (D7_PIN-3) | (0<<EN_PIN)| (0<<RS_PIN)  | (1<< BL_PIN);
	data_t[2] = (data_l & 0x01) << D4_PIN | (data_l & 0x02 ) << (D5_PIN-1)  | (data_l & 0x04) << (D6_PIN-2)  | (data_l & 0x08) << (D7_PIN-3) | (1<<EN_PIN)| (0<<RS_PIN)  | (1<< BL_PIN);
	data_t[3] = (data_l & 0x01) << D4_PIN | (data_l & 0x02 ) << (D5_PIN-1)  | (data_l & 0x04) << (D6_PIN-2)  | (data_l & 0x08) << (D7_PIN-3) | (0<<EN_PIN)| (0<<RS_PIN)  | (1<< BL_PIN);
	for(int i = 0;i<4;i++)
	{
		hc595_trans(data_t[i]);
	}
}
void lcd_Send_Data(char data)
{
	char data_u,data_l;
	uint8_t data_t[4];
	data_u = (data >> 4) & 0x0f;
	data_l = (data & 0x0f);

	data_t[0] = (data_u & 0x01) << D4_PIN | (data_u & 0x02 ) << (D5_PIN-1)  | (data_u & 0x04) << (D6_PIN-2)  | (data_u & 0x08) << (D7_PIN-3) | (1<<EN_PIN)| (1<<RS_PIN)  | (1<< BL_PIN);
	data_t[2] = (data_l & 0x01) << D4_PIN | (data_l & 0x02 ) << (D5_PIN-1)  | (data_l & 0x04) << (D6_PIN-2)  | (data_l & 0x08) << (D7_PIN-3) | (1<<EN_PIN)| (1<<RS_PIN)  | (1<< BL_PIN);
	data_t[3] = (data_l & 0x01) << D4_PIN | (data_l & 0x02 ) << (D5_PIN-1)  | (data_l & 0x04) << (D6_PIN-2)  | (data_l & 0x08) << (D7_PIN-3) | (0<<EN_PIN)| (1<<RS_PIN)  | (1<< BL_PIN);

	for(int i = 0;i<4;i++)
	{
		hc595_trans(data_t[i]);

	}
	//HAL_I2C_Master_Transmit(&hi2c1, 0x27 << 1,(uint8_t *)data_t, 4, 100);
}
void lcd_Clear(void)
{
	lcd_Send_Cmd(0x01);
	HAL_Delay(2);
}
void lcd_Put_Cur(int row,int col)
{
	switch(row)
	{
		case 0:
			col |= 0x80; //0x80 1 000 000x
			break;
		case 1:
			col |= 0xC0; // 0xC1 1 100 000x
			break;
	}
	//Gửi lệnh ứng với vị trí cần xuất hiện con trỏ Set DDRAM address tr24
	lcd_Send_Cmd(col);
}

void lcd_Init(void)
{
	// Khởi tạo 4 bit
	HAL_Delay(50);
	lcd_Send_Cmd(0x30);
	HAL_Delay(5);
	lcd_Send_Cmd(0x30);
	HAL_Delay(1);
	lcd_Send_Cmd(0x30);
	HAL_Delay(10);
	lcd_Send_Cmd(0x20);
	HAL_Delay(10);
	// Khởi tạo hiển thị
	//00 00 101 00
	lcd_Send_Cmd(0x28); //function set bảng 6/tr23: set data interface 4bits 2lines 5x8 bit
	HAL_Delay(1);
	lcd_Send_Cmd(0x08); //display on/off control
	HAL_Delay(1);
	lcd_Send_Cmd(0x01); //clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_Send_Cmd(0x06); //entry mode set: i/d=1 increment s=0
	HAL_Delay(1);
	lcd_Send_Cmd(0x0C); // cho phep hien thi man hinh
}
void lcd_Send_String(char *str)
{
	while(*str) lcd_Send_Data(*str++);
}
void screw_Done_Show(int c)
{
	char str[100];
	sprintf(str,"Screw done:%d",c);
	lcd_Put_Cur(0, 0);
	lcd_Send_String(str);
}
void screw_Set_Show(int c)
{
	char str[100];
	sprintf(str,"Screw set:%d",c);
	lcd_Put_Cur(1, 2);
	lcd_Send_String(str);
}

