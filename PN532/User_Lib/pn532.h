/*
 * pn532.h
 *
 *  Created on: Feb 16, 2023
 *      Author: hoanganh
 */

#ifndef PN532_H_
#define PN532_H_
#include "stdio.h"
#include "main.h"
#define PN532_COMMAND_GETFIRMWAREVERSION    (0x02)

typedef struct {
    uint8_t _uid[7];       // ISO14443A uid
    uint8_t _uidLen;       // uid len
    uint8_t _key[6];       // Mifare Classic key
    uint8_t _inListedTag;  // Tg number of inlisted tag.
} pn532_t;

void pn532_begin(SPI_HandleTypeDef *hspi);
uint32_t pn532_getFirmwareVersion(pn532_t *obj);

#endif /* PN532_H_ */
