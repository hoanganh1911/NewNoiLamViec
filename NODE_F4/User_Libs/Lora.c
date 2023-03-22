/*
 * Lora.c
 *
 *  Created on: Mar 12, 2023
 *      Author: hoanganh
 */
#include "Lora.h"

static void CS_Set(lora_t* _lora)
{
	HAL_GPIO_WritePin(_lora->CS_port, _lora->CS_pin, GPIO_PIN_RESET);
}
static void CS_Reset(lora_t* _lora)
{
	HAL_GPIO_WritePin(_lora->CS_port, _lora->CS_pin, GPIO_PIN_SET);
}
void Lora_Reset(lora_t* _lora)
{
	CS_Set(_lora);
	HAL_Delay(1);
	CS_Reset(_lora);
	HAL_Delay(100);
}
uint8_t Lora_Read(lora_t* _lora,uint8_t address)
{
	uint8_t dt;
	uint8_t dt_addr;
	dt_addr = address & 0x7F; // 7 bit địa chỉ, bit đầu tiên là bit đọc(0)
	CS_Set(_lora);
	HAL_SPI_Transmit(_lora->hSPIx,&dt_addr,1,HAL_MAX_DELAY);
	HAL_SPI_Receive(_lora->hSPIx, &dt, 1, HAL_MAX_DELAY);
	CS_Reset(_lora);
	return dt;
}
void Lora_Write(lora_t* _lora,uint8_t address,uint8_t value)
{
	uint8_t dt_addr;
	dt_addr = address | 0x80; //7 bit địa chỉ, bit đầu tiên là bit viết(1)
	CS_Set(_lora);
	HAL_SPI_Transmit(_lora->hSPIx,&dt_addr,1,HAL_MAX_DELAY);
	HAL_SPI_Transmit(_lora->hSPIx,&value,1,HAL_MAX_DELAY);
	CS_Reset(_lora);
}
void LoRa_BurstWrite(lora_t* _lora, uint8_t address, uint8_t *value, uint8_t length)
{
	uint8_t dt_addr;
	dt_addr = address | 0x80;

	CS_Set(_lora);
	HAL_SPI_Transmit(_lora->hSPIx, &dt_addr, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(_lora->hSPIx, value, length, HAL_MAX_DELAY);
	CS_Reset(_lora);
}
void Lora_Setmode(lora_t* _lora,int mode)
{
	uint8_t recvDt;
	uint8_t dt;

	recvDt = Lora_Read(_lora, RegOpMode);
	recvDt = recvDt & 0xF8;

	if(mode == SLEEP_MODE)
	{
		dt = recvDt | 0x00;
		_lora->currentMode = SLEEP_MODE;
	}
	else if(mode == STDBY_MODE)
	{
		dt = recvDt | 0x01;
		_lora->currentMode = STDBY_MODE;
	}
	else if(mode ==  FSTx)
	{
		dt = recvDt | 0x02;
		_lora->currentMode = FSTx;
	}
	else if(mode == Tx)
	{
		dt = recvDt | 0x03;
		_lora->currentMode = Tx;
	}
	else if(mode == FSRx)
	{
		dt = recvDt | 0x04;
		_lora->currentMode = FSRx;
	}
	else if(mode == Rx)
	{
		dt = recvDt | 0x05;
		_lora->currentMode = Rx;
	}
	Lora_Write(_lora, RegOpMode, dt);
}
void Lora_Set_Frequency(lora_t* _lora,int freqency)
{
	uint8_t dt;
	uint8_t freq;
	freq = (freqency * 524288)/32; //523288 = 2^19 32=2^5;
	dt = freq >> 16; //MSB
	Lora_Write(_lora,RegFrMsb, dt); //MSB
	// HAL_Delay(5);
	dt = freq >> 8;
	Lora_Write(_lora, RegFrMid, dt); //Mid
	// HAL_Delay(5):
	dt = freq;
	Lora_Write(_lora,RegFrLsb,dt); //LSB
	// HAL_Delay(5);
}
void Lora_Set_SpreadingFactor(lora_t* _lora, int SF)
{
	uint8_t dt;
	uint8_t recvdt;

	if(SF > 12)
		SF = 12;
	if(SF < 7)
		SF = 7;

	recvdt = Lora_Read(_lora, RegModemConfig2);
	// HAL_Delay(10);
	dt = (SF << 4) | (recvdt & 0x0F);
	Lora_Write(_lora, RegModemConfig2, dt);
	// HAL_Delay(10);
}
void Lora_SetPower(lora_t* _lora, uint8_t power)
{
	Lora_Write(_lora, RegPaConfig, power);
	//HAL_Delay(10);
}
void Lora_SetOCP(lora_t*_lora,uint8_t current)
{
	uint8_t OcpTrim = 0;
	if(current < 45)
		current = 45;
	if(current > 240)
		current = 240;

	if(current <= 120)
		OcpTrim = (current - 45)/5;
	else if(current <= 240)
		OcpTrim = (current + 30)/10;

	OcpTrim = OcpTrim | (1 << 5);
	Lora_Write(_lora, RegOcp, OcpTrim);
	//HAL_Delay(10);
}
void Lora_SetTOMsb(lora_t* _lora)
{
	uint8_t recvdt, dt;
	recvdt = Lora_Read(_lora, RegModemConfig2);

	dt = recvdt | 0x07;
	Lora_Write(_lora, RegModemConfig2, dt);
	//HAL_Delay(10);
}

bool Lora_Transmit(lora_t* _lora, uint8_t* data, uint8_t length, uint16_t timeout)
{
	uint8_t recvdt;
	int mode = _lora->currentMode;
	Lora_Setmode(_lora, STDBY_MODE);
	recvdt = Lora_Read(_lora, RegFiFoTxBaseAddr);
	Lora_Write(_lora, RegFiFoAddPtr, recvdt);
	Lora_Write(_lora, RegPayloadLength, length);
	LoRa_BurstWrite(_lora, RegFiFo, data, length);
	Lora_Setmode(_lora, Tx);
	while(1){
		recvdt = Lora_Read(_lora, RegIrqFlags);
		if((recvdt & 0x08)!=0){
			Lora_Write(_lora, RegIrqFlags, 0xFF);
			Lora_Setmode(_lora, mode);
			return true;
		}
		else{
			if(--timeout==0){
				Lora_Setmode(_lora, mode);
				return false;
			}
		}
		HAL_Delay(1);
	}
}

void Lora_StartReceving(lora_t* _lora)
{
	Lora_Setmode(_lora, Rx);
}
uint8_t Lora_Receive(lora_t* _lora,uint8_t* data,uint8_t length)
{
	uint8_t recvdt;
	uint8_t number_of_bytes;
	uint8_t min = 0;
	for(int i=0;i<length;i++)
		data[i] = 0;
	Lora_Setmode(_lora, STDBY_MODE);
	recvdt = Lora_Read(_lora, RegIrqFlags);
	if((recvdt & 0x40) != 0)
	{
		Lora_Write(_lora, RegIrqFlags, 0xFF);
		number_of_bytes = Lora_Read(_lora,RegRxNbBytes);
		recvdt = Lora_Read(_lora, RegFiFoRxCurrentAddr);
		Lora_Write(_lora,RegFiFoAddPtr,recvdt);
		min = length >= number_of_bytes ? number_of_bytes : length;
		for(int i=0;i<min;i++)
			data[i] = Lora_Read(_lora, RegFiFo);
	}
	Lora_Setmode(_lora, Rx);
	return min;
}
int Lora_GetRSSI(lora_t* _lora)
{
	uint8_t recvdt;
	recvdt = Lora_Read(_lora, RegPktRssiValue);
	return -164 + recvdt;
}
uint32_t Lora_init(lora_t* _lora)
{
	uint8_t dt;
	uint8_t recvdt;
	Lora_Setmode(_lora, SLEEP_MODE);
	HAL_Delay(10);

	recvdt = Lora_Read(_lora, RegOpMode);
	HAL_Delay(10);
	dt = recvdt | 0x80;
	Lora_Write(_lora, RegOpMode, dt);
	HAL_Delay(100);

	Lora_Set_Frequency(_lora, _lora->frequency);
	Lora_SetPower(_lora, _lora->power);
	Lora_SetOCP(_lora, _lora->overCurrentProtection);
	Lora_Write(_lora, RegLna, 0x23);
	Lora_SetTOMsb(_lora);
	Lora_Set_SpreadingFactor(_lora, _lora->spredingFactor);
	Lora_Write(_lora,RegSymbTimeoutL,0xFF);
	dt = 0;
	dt = _lora->bandWidth << 4 | _lora->crcRate << 1;
	Lora_Write(_lora, RegModemConfig1, dt);
	Lora_Write(_lora,RegPreambleMsb,_lora->preamble >> 8);
	Lora_Write(_lora,RegPreambleLsb,_lora->preamble >> 0);
	recvdt = Lora_Read(_lora, RegDioMapping1);
	dt = recvdt | 0x3F;
	Lora_Write(_lora, RegDioMapping1, dt);
	Lora_Setmode(_lora, STDBY_MODE);
	_lora->currentMode = STDBY_MODE;
	HAL_Delay(10);
	recvdt = Lora_Read(_lora, RegVersion);
	if(recvdt == 0x12)
		return LORA_OK;
	else
		return LORA_NOT_FOUND;
}





