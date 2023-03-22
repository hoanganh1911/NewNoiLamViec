/*
 * DS3231.h
 *
 *  Created on: Mar 17, 2023
 *      Author: hoanganh
 */

#ifndef DS3231_H_
#define DS3231_H_
/*----------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "stdbool.h"
/*----------------------------------------*/
#define DS3231_ADDR 0x68 << 1
#define DS3231_REG_TIME	0x00
typedef enum
{
  SUNDAY = 1,
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY
} DayOfWeek;
typedef struct
{
	uint8_t Year;
	uint8_t Month;
	uint8_t Date;
	uint8_t DayOfWeek;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
	I2C_HandleTypeDef *hi2c;
}rtc_t;
#endif /* DS3231_H_ */
