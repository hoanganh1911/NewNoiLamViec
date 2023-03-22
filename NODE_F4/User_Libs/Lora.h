/*
 * Lora.h
 *
 *  Created on: Mar 12, 2023
 *      Author: hoanganh
 */

#ifndef LORA_H_
#define LORA_H_
#include "stm32f4xx_hal.h"
#include "stdbool.h"
/*-----------------MODES------------------*/
#define SLEEP_MODE 		0 // Sleep mode
#define STDBY_MODE		1 // Standby mode
#define FSTx			2 // FS mode Tx
#define Tx				3 // Transmitter mode
#define FSRx			4 // FS mode Rx
#define Rx				5 // Receiver mode
/*---------------REGISTERS----------------*/
#define RegFiFo					0x00
#define RegOpMode				0x01
#define RegFrMsb				0x06
#define RegFrMid				0x07
#define RegFrLsb				0x08
#define RegPaConfig				0x09
#define RegOcp					0x0B
#define RegLna					0x0C
#define RegFiFoAddPtr			0x0D
#define RegFiFoTxBaseAddr		0x0E
#define RegFiFoRxCurrentAddr	0x10
#define RegIrqFlags				0x12
#define RegRxNbBytes			0x13
#define RegPktRssiValue			0x1A
#define	RegModemConfig1			0x1D
#define RegModemConfig2			0x1E
#define RegSymbTimeoutL			0x1F
#define RegPreambleMsb			0x20
#define RegPreambleLsb			0x21
#define RegPayloadLength		0x22
#define RegDioMapping1			0x40
#define RegDioMapping2			0x41
#define RegVersion				0x42
/*----------------------------------------*/
#define LORA_OK					200
#define LORA_NOT_FOUND			404
#define LORA_UNAVAILABLE		503
/*----------------------------------------*/
typedef struct {
	GPIO_TypeDef* 		CS_port;
	uint16_t			CS_pin;
	GPIO_TypeDef*		RS_port;
	uint16_t			RS_pin;
	GPIO_TypeDef*		DIO0_port;
	uint16_t			DIO0_pin;
	SPI_HandleTypeDef* 	hSPIx;

	int 				currentMode;
	int 				frequency;
	uint8_t				spredingFactor; 		// Tỉ lệ giữa băng thông tín hiệu và băng thông tín hiệu phân tán
	uint8_t				bandWidth; 				// Băng thông cần thiết = băng thông tín hiệu + băng thông tín hiệu phân tán
	uint8_t				crcRate; 				// CRC là phương pháp kiểm tra lỗi. CRC Rate là tỉ lệ CRC
	uint16_t			preamble; 				// Bit đầu, đồng bộ hóa và phát hiện tín hiệu
	uint8_t				power;
	uint8_t				overCurrentProtection;
}lora_t;
/*----------------------------------------*/
void Lora_reset(lora_t* _lora);
uint8_t Lora_Read(lora_t* _lora,uint8_t address);
void Lora_Write(lora_t* _lora,uint8_t address,uint8_t value);
void LoRa_BurstWrite(lora_t* _lora, uint8_t address, uint8_t *value, uint8_t length);
void Lora_Setmode(lora_t* _lora,int mode);
void Lora_Set_Frequency(lora_t* _lora,int freqency);
/*----------------------------------------*/
#endif /* LORA_H_ */
