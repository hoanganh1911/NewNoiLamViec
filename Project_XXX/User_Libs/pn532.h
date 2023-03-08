/*
 * pn532.h
 *
 *  Created on: Feb 28, 2023
 *      Author: hoanganh
 */

#ifndef PN532_H_
#define PN532_H_
/*----------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "stdbool.h"
/*----------------------------------------*/
#define PN532_PREAMBLE                   0x00
#define PN532_COMMAND_GETFIRMWAREVERSION 0x02
#define PN532_SPI_STATEREAD              0x02
#define PN532_SPI_DATAWRITE

/*----------------------------------------*/

#define PN532_Delay(ms) HAL_Delay(ms)
#define PN532_PACKBUFFSIZE 64


static uint8_t pn532_packetbuffer[PN532_PACKBUFFSIZE];
/*----------------------------------------*/
typedef struct{
	SPI_HandleTypeDef _hspi;

	GPIO_TypeDef *_clkGPIO;
	uint16_t _clkPIN;
	GPIO_TypeDef *_misoGPIO;
	uint16_t _misoPIN;
	GPIO_TypeDef *_mosiGPIO;
	uint16_t _mosiPIN;
	GPIO_TypeDef *_ssGPIO;
	uint8_t _ssPIN;

	uint8_t _uid[7];
	uint8_t _uidLen;
	uint8_t _key[6];
	uint8_t _inListedTag;
}pn532_t;

void pn532_spi_init(pn532_t *obj);
void pn532_begin(void);
bool pn532_sendCommandCheckAck(uint8_t *cmd, uint8_t cmdlen, uint16_t timeout);
void pn532_writecommand(uint8_t *cmd,uint8_t cmdlen);
void pn532_spi_write(uint8_t c);
bool pn532_isready(void);
uint8_t pn532_spi_read(void);
#endif /* PN532_H_ */
