/*
 * DS3231.c
 *
 *  Created on: Mar 17, 2023
 *      Author: hoanganh
 */
#include "DS3231.h"

static uint8_t B2D(uint8_t bcd) //hàm để chuyển giá trị ở các bit hàng chục
{
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t D2B(uint8_t decimal) //hàm để chuyển giá trị ở các bit hàng chục
{
  return (((decimal / 10) << 4) | (decimal % 10));
}

bool ds1307_getTime(rtc_t* _rtc)
{
	uint8_t startAddr = DS3231_REG_TIME;
	uint8_t buffer[7] = {0,};
	if(HAL_I2C_Master_Transmit(_rtc->hi2c,DS3231_ADDR, &startAddr, 1, HAL_MAX_DELAY) != HAL_OK) return false;
	if(HAL_I2C_Master_Receive(_rtc->hi2c, DS3231_ADDR, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) return false;
	_rtc->Sec = B2D(buffer[0] & 0x7F);
	_rtc->Min = B2D(buffer[1] & 0x7F);
	_rtc->Hour = B2D(buffer[2] & 0x3F);
	_rtc->DayOfWeek = buffer[3] & 0x07;
	_rtc->Date = B2D(buffer[4] & 0x3F);
	_rtc->Month = B2D(buffer[5] & 0x1F);
	_rtc->Year = B2D(buffer[6]);

	return true;
}

bool ds1307_setTime(rtc_t* _rtc)
{
	uint8_t startAddr = DS3231_REG_TIME;
	uint8_t buffer[8] = {startAddr,D2B(_rtc->Sec),D2B(_rtc->Min),D2B(_rtc->Hour),_rtc->DayOfWeek,D2B(_rtc->Date),D2B(_rtc->Month),D2B(_rtc->Year)};
	if(HAL_I2C_Master_Transmit(_rtc->hi2c, DS3231_ADDR, buffer, sizeof(buffer),HAL_MAX_DELAY) != HAL_OK) return false;

	return true;
}



