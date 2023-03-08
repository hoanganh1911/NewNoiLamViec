/*
 * pn532.c
 *
 *  Created on: Feb 28, 2023
 *      Author: hoanganh
 */

#include "pn532.h"

pn532_t *hpn532;


void pn532_spi_init(pn532_t *obj)
{
	hpn532 = obj;
}

void pn532_begin()
{
	HAL_GPIO_WritePin(hpn532->_ssGPIO, hpn532->_ssPIN, 0);
	PN532_Delay(1000);

	// Gửi để đồng bộ chứ ko biết để làm gì ...
	pn532_packetbuffer[0]= PN532_COMMAND_GETFIRMWAREVERSION;
	pn532_sendCommandCheckAck(pn532_packetbuffer,1,1000);
	HAL_GPIO_WritePin(hpn532->_ssGPIO, hpn532->_ssPIN, 1);
}
bool pn532_sendCommandCheckAck(uint8_t *cmd, uint8_t cmdlen, uint16_t timeout)
{
	pn532_writecommand(cmd, cmdlen);
	if(!pn532_waitready(timeout))
	{
		return false;
	}
}
bool pn532_isready()
{
	HAL_GPIO_WritePin(hpn532->_ssGPIO, hpn532->_ssPIN, 0);
	PN532_Delay(10);
	pn532_spi_write(PN532_SPI_STATEREAD);
	uint8_t x = pn532_spi_read();
	HAL_GPIO_WritePin(hpn532->_ssGPIO, hpn532->_ssPIN, 1);
}



void pn532_writecommand(uint8_t *cmd,uint8_t cmdlen)
{
	uint8_t checksum;
	cmdlen++;

	//Thêm hàm debug
	HAL_GPIO_WritePin(hpn532->_ssGPIO, hpn532->_ssPIN, 0);
	PN532_Delay(10);
    pn532_spi_write(PN532_SPI_DATAWRITE);


}
void pn532_spi_write(uint8_t c)
{
	HAL_GPIO_WritePin(hpn532->_ssGPIO, hpn532->_ssPIN, 0);
	HAL_SPI_Transmit(&(hpn532->_hspi),(uint8_t*)c, 1, 1000);
	HAL_GPIO_WritePin(hpn532->_ssGPIO, hpn532->_ssPIN, 1);
}
uint8_t pn532_spi_read()
{
	uint8_t x;
	HAL_GPIO_WritePin(hpn532->_ssGPIO, hpn532->_ssPIN, 0);
	HAL_SPI_Receive(&(hpn532->_hspi),(uint8_t*)x, 1, 1000);
	HAL_GPIO_WritePin(hpn532->_ssGPIO, hpn532->_ssPIN, 1);
	return x;
}



