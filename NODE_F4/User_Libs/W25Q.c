/*
 * W25Q.c
 *
 *  Created on: Mar 8, 2023
 *      Author: hoanganh
 */


#include "W25Q.h"
uint8_t buf[256];
/*----------------------------------------*/
// Pin CS SPI W25Qxx
void CS_Set(w25q_t* _w25q)
{
	HAL_GPIO_WritePin(_w25q->CS_port,_w25q->CS_Pin, GPIO_PIN_RESET);
}
void CS_Reset(w25q_t* _w25q)
{
	HAL_GPIO_WritePin(_w25q->CS_port, _w25q->CS_Pin, GPIO_PIN_SET);
}
/*----------------------------------------*/
// Send data to W25Qxx
void W25_Send(w25q_t* _w25q,uint8_t *dt,uint16_t cnt)
{
	HAL_SPI_Transmit(_w25q->hSPIx, dt, cnt, 1000);
}
// Receive data from W25Q
void W25_Recv(w25q_t* _w25q,uint8_t *dt,uint16_t cnt)
{
	HAL_SPI_Receive(_w25q->hSPIx, dt, cnt, 1000);
}
// Send reset command W25Qxx
void W25_Reset(w25q_t* _w25q)
{
	CS_Set(_w25q);
	buf[0] = W25_ENABLE_RESET;
	buf[1] = W25_RESET;
	W25_Send(_w25q,buf,2);
	CS_Reset(_w25q);
}
// Init W25Qxx
void W25_Init(w25q_t* _w25q)
{
	HAL_Delay(100);
	W25_Reset(_w25q);
	HAL_Delay(100);
}

// Read data from W25Qxx
void W25_Read_Data(w25q_t* _w25q, uint32_t addr, uint8_t* dt, uint32_t sz)
{
	CS_Set(_w25q);
	buf[0] = W25_READ;
	buf[1] = (addr >> 16) & 0xFF;
	buf[2] = (addr >> 8) & 0xFF;
	buf[3] = addr & 0xFF;
	W25_Send(_w25q, dt, 4);
	W25_Recv(_w25q, dt, sz);
	CS_Reset(_w25q);
}
// Read Unique ID from W25Qxx
uint64_t W25_Read_ID(w25q_t* _w25q)
{
	uint8_t dt[12];
	buf[0] = W25_UNIQUE_ID;
	CS_Set(_w25q);
	uint64_t received_id = 0;
	for (int i = 4; i < 12; i++) // Handle dummy byte
	{
		received_id |= dt[i];
		if (i != 11)
			received_id <<= 8;
	}
	return received_id;
}
// Read data by Page
void W25_Read_Page(w25q_t* _w25q, uint8_t* dt, uint32_t page_addr, uint32_t offset, uint32_t sz)
{
	// Just read one page and this is a condition
	if(sz > _w25q->PageSize)
		sz = _w25q->PageSize;
	if((offset+sz) > _w25q->PageSize)
		sz = _w25q->PageSize - offset;

	page_addr = page_addr * _w25q->PageSize + offset;
	buf[0] =  W25_FAST_READ;

	//Handle high_cap which depends on capacity of W25Q device to find address
	if(_w25q->high_cap)
	{
		buf[1] = (page_addr >> 24) & 0xFF;
		buf[2] = (page_addr >> 16) & 0xFF;
		buf[3] = (page_addr >> 8)  & 0xFF;
		buf[4] = page_addr & 0xFF;
		buf[5] = 0;
		CS_Set(_w25q);
		W25_Send(_w25q, buf, 6);
	}
	else
	{
		buf[1] = (page_addr >> 16) & 0xFF;
		buf[2] = (page_addr >> 8) & 0xFF;
		buf[3] = page_addr & 0xFF;
		buf[4] = 0;
		CS_Set(_w25q);
		W25_Send(_w25q, buf, 5);
	}
	W25_Recv(_w25q, dt, sz);
	CS_Reset(_w25q);
}
// Read JEDEC ID W25Qxx
uint32_t W25_Read_Jedec_ID(w25q_t* _w25q)
{
	uint8_t dt[4];
	buf[0] = W25_GET_JEDEC_ID;
	CS_Set(_w25q);
	W25_Send(_w25q, buf, 1);
	W25_Recv(_w25q, dt, 3);
	CS_Reset(_w25q);
	return (dt[0] << 16) | (dt[1] << 8) | dt[2];
}
// Check info W25Qxx that depends on JEDEC ID
void W25_Info(w25q_t* _w25q)
{
	uint32_t Jedec_ID = W25_Read_Jedec_ID(_w25q);
	Jedec_ID &= 0x0000ffff;
	_w25q->high_cap = 0;
	switch(Jedec_ID)
	{
		case 0x401A: //w25q512
			_w25q->high_cap = 1;
			_w25q->BlockCount=1024;
			break;
		case 0x4019: //w25q256
			_w25q->high_cap = 1;
			_w25q->BlockCount=512;
			break;
		case 0x4018: //w25q128
			_w25q->BlockCount=256;
			break;
		case 0x4017: //w25q64
			_w25q->BlockCount=128;
			break;
		case 0x4016: //w25q32
			_w25q->BlockCount=64;
			break;
		case 0x4015: //w25q16
			_w25q->BlockCount=32;
			break;
		case 0x4014: //w25q80
			_w25q->BlockCount=16;
			break;
		case 0x4013: //w25q40
			_w25q->BlockCount=8;
			break;
		case 0x4012: //w25q20
			_w25q->BlockCount=4;
			break;
		case 0x4011: //w25q10
			_w25q->BlockCount=2;
			break;
		default:
			break;
	}
	_w25q->PageSize=256;
	_w25q->SectorSize=0x1000;
	_w25q->SectorCount=_w25q->BlockCount*16;
	_w25q->PageCount=(_w25q->SectorCount*_w25q->SectorSize)/_w25q->PageSize;
	_w25q->BlockSize=_w25q->SectorSize*16;
	_w25q->NumKB=(_w25q->SectorCount*_w25q->SectorSize)/1024;
}
// Enable Write data to W25Qxx
void W25_Write_Enable(w25q_t* _w25q)
{
	CS_Set(_w25q);
	buf[0] = W25_WRITE_ENABLE;
	W25_Send(_w25q,buf,1);
	CS_Reset(_w25q);
}
// Disable Write data to W25Qxx
void W25_Write_Disable(w25q_t* _w25q)
{
	CS_Set(_w25q);
	buf[0] = W25_WRITE_DISABLE;
	W25_Send(_w25q,buf,1);
	CS_Reset(_w25q);
}
// Check busy bit in register 1
void W25_Wait_Write_End(w25q_t* _w25q)
{
	HAL_Delay(1);
	CS_Set(_w25q);
	buf[0] = W25_READ_STATUS_1;
	W25_Send(_w25q,buf, 1);
	do{
		W25_Recv(_w25q,buf,1);
		_w25q->StatusRegister1 = buf[0];
		HAL_Delay(1);
	}
	while((_w25q->StatusRegister1 & 0x01) == 0x01);
	CS_Reset(_w25q);
}
// Protect Writting to W25Qxx
void W25_Set_Block_Protect(w25q_t* _w25q,uint8_t val)
{
	buf[0] = 0x50;

	CS_Set(_w25q);
	W25_Send(_w25q,buf, 1);
	CS_Reset(_w25q);

	buf[0] = W25_WRITE_STATUS_1;
	buf[1] = ((val & 0x0F) << 2);

	CS_Set(_w25q);
	W25_Send(_w25q,buf, 2);
	CS_Reset(_w25q);
}
// Write data to W25Qxx
void W25_Write_Data(w25q_t* _w25q,uint32_t addr, uint8_t* dt, uint32_t sz)
{
	W25_Wait_Write_End(_w25q);
	W25_Set_Block_Protect(_w25q,0x00);
	W25_Write_Enable(_w25q);
	CS_Set(_w25q);
	buf[0] = W25_PAGE_PROGRAMM;
	if(_w25q->high_cap)
	{
		buf[1] = (addr >> 24) & 0xFF;
		buf[2] = (addr >> 16) & 0xFF;
		buf[3] = (addr >> 8) & 0xFF;
		buf[4] = addr & 0xFF;
		W25_Send(_w25q,buf, 5);
	}
	else
	{
		buf[1] = (addr >> 16) & 0xFF;
		buf[2] = (addr >> 8) & 0xFF;
		buf[3] = addr & 0xFF;
		W25_Send(_w25q,buf, 4);
	}
	W25_Send(_w25q,dt, sz);
	CS_Reset(_w25q);
	W25_Wait_Write_End(_w25q);
	W25_Write_Disable(_w25q);
	W25_Set_Block_Protect(_w25q,0x0F);
}
// Write data page by page to W25Qxx
void W25_Write_Page(w25q_t* _w25q,uint8_t* dt, uint32_t page_addr, uint32_t offset, uint32_t sz)
{
	if(sz > _w25q->PageSize)
		sz = _w25q->PageSize;
	if((offset+sz) > _w25q->PageSize)
		sz = _w25q->PageSize - offset;
	page_addr = page_addr * _w25q->PageSize + offset;

	W25_Wait_Write_End(_w25q);
	W25_Set_Block_Protect(_w25q,0x00);
	W25_Write_Enable(_w25q);
	CS_Set(_w25q);
	buf[0] = W25_PAGE_PROGRAMM;
	if(_w25q->high_cap)
	{
		buf[1] = (page_addr >> 24) & 0xFF;
		buf[2] = (page_addr >> 16) & 0xFF;
		buf[3] = (page_addr >> 8) & 0xFF;
		buf[4] = page_addr & 0xFF;
		W25_Send(_w25q,buf, 5);
	}
	else
	{
		buf[1] = (page_addr >> 16) & 0xFF;
		buf[2] = (page_addr >> 8) & 0xFF;
		buf[3] = page_addr & 0xFF;
		W25_Send(_w25q,buf, 4);
	}
	W25_Send(_w25q,dt, sz);
	CS_Reset(_w25q);
	W25_Wait_Write_End(_w25q);
	W25_Write_Disable(_w25q);
	W25_Set_Block_Protect(_w25q,0x0F);
}
// The Sector Erase instruction sets all memory within a specified sector (4K-bytes) to the erased state of all 1s (FFh)
void W25_Erase_Sector(w25q_t* _w25q,uint32_t addr)
{
	W25_Wait_Write_End(_w25q);
	W25_Set_Block_Protect(_w25q,0x00);
	addr = addr * _w25q->SectorSize;
	W25_Write_Enable(_w25q);
	CS_Set(_w25q);
	buf[0] = W25_SECTOR_ERASE;
	if(_w25q->high_cap)
	{
		buf[1] = (addr >> 24) & 0xFF;
		buf[2] = (addr >> 16) & 0xFF;
		buf[3] = (addr >> 8) & 0xFF;
		buf[4] = addr & 0xFF;
		W25_Send(_w25q,buf, 5);
	}
	else
	{
		buf[1] = (addr >> 16) & 0xFF;
		buf[2] = (addr >> 8) & 0xFF;
		buf[3] = addr & 0xFF;
		W25_Send(_w25q,buf, 4);
	}
	CS_Reset(_w25q);
	W25_Wait_Write_End(_w25q);
	W25_Write_Disable(_w25q);
	W25_Set_Block_Protect(_w25q,0x0F);
}
// The Block Erase instruction sets all memory within a specified block (64K-bytes) to the erased state of all 1s (FFh).
void W25_Erase_Block(w25q_t* _w25q,uint32_t addr)
{
  W25_Wait_Write_End(_w25q);
  W25_Set_Block_Protect(_w25q,0x00);
  addr = addr * _w25q->BlockSize;
  W25_Write_Enable(_w25q);
  CS_Set(_w25q);
  buf[0] = W25_BLOCK_ERASE;
  if(_w25q->high_cap)
  {
    buf[1] = (addr >> 24) & 0xFF;
    buf[2] = (addr >> 16) & 0xFF;
    buf[3] = (addr >> 8) & 0xFF;
    buf[4] = addr & 0xFF;
    W25_Send(_w25q,buf, 5);
  }
  else
  {
    buf[1] = (addr >> 16) & 0xFF;
    buf[2] = (addr >> 8) & 0xFF;
    buf[3] = addr & 0xFF;
    W25_Send(_w25q,buf, 4);
  }
  CS_Reset(_w25q);
  W25_Wait_Write_End(_w25q);
  W25_Write_Disable(_w25q);
  W25_Set_Block_Protect(_w25q,0x0F);
}
// The Chip Erase instruction sets all memory within the device to the erased state of all 1s (FFh)
void W25_Erase_Chip(w25q_t* _w25q)
{
	W25_Wait_Write_End(_w25q);
	W25_Set_Block_Protect(_w25q,0x00);
	W25_Write_Enable(_w25q);
	CS_Set(_w25q);
	buf[0] = W25_CHIP_ERASE;
	W25_Send(_w25q,buf, 1);
	CS_Reset(_w25q);
	W25_Wait_Write_End(_w25q);
	W25_Write_Disable(_w25q);
	W25_Set_Block_Protect(_w25q,0x0F);
}










