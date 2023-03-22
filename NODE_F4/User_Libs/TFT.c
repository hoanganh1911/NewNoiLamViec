/*
 * TFT.c
 *
 *  Created on: Mar 21, 2023
 *      Author: hoanganh
 */

#include "TFT.h"
/*--------------------------------------------------------------------------------------*/
void PIN_LOW (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}
void PIN_HIGH (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}
void PIN_INPUT (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
void PIN_OUTPUT (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
/*--------------------------------------------------------------------------------------*/
#define RD_ACTIVE  		PIN_LOW(RD_PORT, RD_PIN)
#define RD_IDLE    		PIN_HIGH(RD_PORT, RD_PIN)
#define RD_OUTPUT  		PIN_OUTPUT(RD_PORT, RD_PIN)
#define WR_ACTIVE  		PIN_LOW(WR_PORT, WR_PIN)
#define WR_IDLE    		PIN_HIGH(WR_PORT, WR_PIN)
#define WR_OUTPUT  		PIN_OUTPUT(WR_PORT, WR_PIN)
#define CD_COMMAND 		PIN_LOW(CD_PORT, CD_PIN)
#define CD_DATA    		PIN_HIGH(CD_PORT, CD_PIN)
#define CD_OUTPUT  		PIN_OUTPUT(CD_PORT, CD_PIN)
#define CS_ACTIVE  		PIN_LOW(CS_PORT, CS_PIN)
#define CS_IDLE    		PIN_HIGH(CS_PORT, CS_PIN)
#define CS_OUTPUT  		PIN_OUTPUT(CS_PORT, CS_PIN)
#define RESET_ACTIVE  	PIN_LOW(RESET_PORT, RESET_PIN)
#define RESET_IDLE    	PIN_HIGH(RESET_PORT, RESET_PIN)
#define RESET_OUTPUT  	PIN_OUTPUT(RESET_PORT, RESET_PIN)

#define WR_ACTIVE2  	{WR_ACTIVE; WR_ACTIVE;}
#define WR_ACTIVE4  	{WR_ACTIVE2; WR_ACTIVE2;}
#define WR_ACTIVE8  	{WR_ACTIVE4; WR_ACTIVE4;}
#define RD_ACTIVE2  	{RD_ACTIVE; RD_ACTIVE;}
#define RD_ACTIVE4  	{RD_ACTIVE2; RD_ACTIVE2;}
#define RD_ACTIVE8  	{RD_ACTIVE4; RD_ACTIVE4;}
#define RD_ACTIVE16 	{RD_ACTIVE8; RD_ACTIVE8;}
#define WR_IDLE2  		{WR_IDLE; WR_IDLE;}
#define WR_IDLE4  		{WR_IDLE2; WR_IDLE2;}
#define RD_IDLE2  		{RD_IDLE; RD_IDLE;}
#define RD_IDLE4  		{RD_IDLE2; RD_IDLE2;}

#define WR_STROBE 		{ WR_ACTIVE; WR_IDLE; }         //PWLW=TWRL=50ns
#define RD_STROBE 		RD_IDLE, RD_ACTIVE, RD_ACTIVE, RD_ACTIVE   //PWLR=TRDL=150ns

#define write8(x)     	{ write_8(x); WRITE_DELAY; WR_STROBE; WR_IDLE; }
#define write16(x)    	{ uint8_t h = (x)>>8, l = x; write8(h); write8(l); }
#define READ_8(dst)   	{ RD_STROBE; READ_DELAY; dst = read_8(); RD_IDLE; RD_IDLE; } // read 250ns after RD_ACTIVE goes low
#define READ_16(dst)  	{ uint8_t hi; READ_8(hi); READ_8(dst); dst |= (hi << 8); }

#define CTL_INIT()   	{ RD_OUTPUT; WR_OUTPUT; CD_OUTPUT; CS_OUTPUT; RESET_OUTPUT; }
#define WriteCmd(x)  	{ CD_COMMAND; write16(x); CD_DATA; }
#define WriteData(x) 	{ write16(x); }
/*--------------------------------------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
void TFT_Delay_us (uint32_t time)
{
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim3))<time);
}
void TFT_Delay_ms(uint32_t time)
{
	for(int i=0;i<1000;i++)
		TFT_Delay_us(1);
}
/*--------------------------------------------------------------------------------------*/
void LED_ON(void)
{
	HAL_GPIO_WritePin(TFT_LED_GPIO_Port, TFT_LED_Pin, 1);
}
void LED_OFF(void)
{
	HAL_GPIO_WritePin(TFT_LED_GPIO_Port, TFT_LED_Pin, 0);
}
void setReadDir (void)
{
	PIN_INPUT(D0_PORT, D0_PIN);
	PIN_INPUT(D1_PORT, D1_PIN);
	PIN_INPUT(D2_PORT, D2_PIN);
	PIN_INPUT(D3_PORT, D3_PIN);
	PIN_INPUT(D4_PORT, D4_PIN);
	PIN_INPUT(D5_PORT, D5_PIN);
	PIN_INPUT(D6_PORT, D6_PIN);
	PIN_INPUT(D7_PORT, D7_PIN);
}

void setWriteDir (void)
{
	PIN_OUTPUT(D0_PORT, D0_PIN);
	PIN_OUTPUT(D1_PORT, D1_PIN);
	PIN_OUTPUT(D2_PORT, D2_PIN);
	PIN_OUTPUT(D3_PORT, D3_PIN);
	PIN_OUTPUT(D4_PORT, D4_PIN);
	PIN_OUTPUT(D5_PORT, D5_PIN);
	PIN_OUTPUT(D6_PORT, D6_PIN);
	PIN_OUTPUT(D7_PORT, D7_PIN);
}

static uint16_t read16bits(void)
{
    uint16_t ret;
    uint8_t lo;
    READ_8(ret);
    READ_8(lo);
    return (ret << 8) | lo;
}
uint16_t readReg(uint16_t reg,int8_t index)
{
    uint16_t ret;
    CS_ACTIVE;
    setReadDir();
    WriteCmd(reg);
    TFT_Delay_us(1);
    do
    {
    	ret = read16bits();
    }
    while (--index >= 0);
    RD_IDLE;
    CS_IDLE;
    setWriteDir();
    return ret;
}
uint32_t readReg40(uint16_t reg)
{
    uint16_t h = readReg(reg, 0);
    uint16_t m = readReg(reg, 1);
    uint16_t l = readReg(reg, 2);
    return ((uint32_t) h << 24) | (m << 8) | (l >> 8);
}
uint32_t ReadID()
{
	return readReg40(0xEF);
}
void TFT_init()
{
	HAL_TIM_Base_Start(&htim3);
	LED_ON();
	uint32_t read = ReadID();

	WriteCmd(0x01)	//Soft Reset
	TFT_Delay_us(150);
	WriteCmd(0x28); //Display Off
	TFT_Delay_us(1);
	WriteCmd(0x3A); //Pixel read=565, write=565.
	WriteData(0x55);
	TFT_Delay_us(1);
	WriteCmd(0xB0); // Disable Protect for cmds B1-DF, E0-EF, F0-FF
	WriteData(0x00);
	TFT_Delay_us(1);
	WriteCmd(0xC1); // Display Timing [10 10 02 02]
	WriteData(0x10);
	WriteData(0x10);
	WriteData(0x02);
	WriteData(0x02);
	TFT_Delay_us(1);
	WriteCmd(0xC0); // Panel Drive [00 35 00 00 01 02 REV=0,GS=0,SS=0
	WriteData(0x00);
	WriteData(0x35);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x01);
	WriteData(0x02);
	TFT_Delay_us(1);
	WriteCmd(0xC5); // Frame Rate [04]
	WriteData(0x04);
	TFT_Delay_us(1);
	WriteCmd(0xD2); // Power Setting [01 44]
	WriteData(0x01);
	WriteData(0x04);
	TFT_Delay_us(1);
	WriteCmd(0xCA); // DGC LUT ???
	WriteData(0x00);
	TFT_Delay_us(1);
	WriteCmd(0xEA);
	WriteData(0x80);
	TFT_Delay_us(1);
	WriteCmd(0x11);
	TFT_Delay_us(150);
	WriteCmd(0x29);
	TFT_Delay_us(1);
}
