/*
 * W25Q.h
 *
 *  Created on: Mar 8, 2023
 *      Author: hoanganh
 */

#ifndef W25Q_H_
#define W25Q_H_
/*----------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdio.h"
/*----------------------------------------*/
#define W25_ENABLE_RESET 0x66
#define W25_RESET 0x99
#define W25_READ 0x03
#define W25_FAST_READ 0x0B
#define W25_GET_JEDEC_ID 0x9f
#define W25_UNIQUE_ID 0x4B
#define W25_WRITE_DISABLE  0x04
#define W25_WRITE_ENABLE 0x06
#define W25_SECTOR_ERASE 0x20
#define W25_BLOCK_ERASE  0xD8
#define W25_CHIP_ERASE 0xC7
#define W25_READ_STATUS_1  0x05
#define W25_READ_STATUS_2  0x35
#define W25_READ_STATUS_3  0x15
#define W25_WRITE_STATUS_1 0x01
#define W25_WRITE_STATUS_2 0x31
#define W25_WRITE_STATUS_3 0x11
#define W25_PAGE_PROGRAMM  0x02
/*----------------------------------------*/
typedef struct
{

	// Hardware setting
	GPIO_TypeDef* 		CS_port;
	uint16_t			CS_Pin;
	SPI_HandleTypeDef*	hSPIx;

	// Module setting
	uint16_t  			PageSize;
	uint32_t  			PageCount;
	uint32_t  			SectorSize;
	uint32_t  			SectorCount;
	uint32_t  			BlockSize;
	uint32_t  			BlockCount;
	uint32_t  			NumKB;
	uint8_t   			SR1;
	uint8_t   			SR2;
	uint8_t   			SR3;
	uint8_t				high_cap;
	uint8_t   			StatusRegister1;
	uint8_t   			StatusRegister2;
	uint8_t   			StatusRegister3;
} w25q_t;
/*----------------------------------------*/










#endif /* W25Q_H_ */
