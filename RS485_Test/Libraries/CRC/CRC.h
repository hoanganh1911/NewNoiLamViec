/*
 * modbus_crc.h
 *
 *  Created on: Sep 16, 2022
 *      Author: arunr
 */

#ifndef CRC_H_
#define CRC_H_

#include "stdio.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C"{
#endif

uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);

#ifdef __cplusplus
}
#endif
#endif /* CRC_H_ */
