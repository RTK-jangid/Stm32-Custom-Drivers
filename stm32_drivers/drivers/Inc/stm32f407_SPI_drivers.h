/*
 * stm32f407_SPI_drivers.h
 *
 *  Created on: Mar 3, 2024
 *      Author: ritik
 */
#include<stdint.h>
#include "stm32f407xx.h"
#ifndef INC_STM32F407_SPI_DRIVERS_H_
#define INC_STM32F407_SPI_DRIVERS_H_

typedef struct{
	volatile uint8_t Mode;
	volatile uint8_t SclkSpeed;
	volatile uint8_t BusConfig;
	volatile uint8_t DFF;
	volatile uint8_t CPOL;
	volatile uint8_t CPHA;
	volatile uint8_t SSM;
}SPI_Config_t;

typedef struct{
	  SPIx_Reg_t *pSPIx;
	  SPI_Config_t SPIConfig;
	  uint8_t *Txbuffer;
	  uint8_t *Rxbuffer;
	  uint8_t TxLen;
	  uint8_t RxLen;
	  uint8_t TxState;
	  uint8_t RxState;
}SPI_Handle_t;


void SPI_PeriClockControl(SPIx_Reg_t *pSPIx, uint8_t EnorDi);

void SPI_init(SPI_Handle_t *pSPIHandle);
void SPI_deinit(SPIx_Reg_t *pSPIx);

void SPI_Send(SPIx_Reg_t *pSPIx,uint8_t *TxBuffer,uint32_t Len);
void SPI_Recieve(SPIx_Reg_t *pSPIx,uint8_t *RxBuffer,uint32_t Len);

uint8_t SPI_SendIT(SPI_Handle_t *pHandle,uint8_t *TxBuffer,uint32_t Len);
uint8_t SPI_RecieveIT(SPI_Handle_t *pHandle,uint8_t *RxBuffer,uint32_t Len);

void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQHandling(SPI_Handle_t *pHandle);
void SPI_IRQ_Set_Priority(uint8_t IRQNumber,uint32_t IRQPriority);
uint8_t getFlag_Status(SPIx_Reg_t *pSPIx, uint8_t flag);
void SPI_Enable(SPIx_Reg_t *pSPIx, uint8_t EnorDi);
void SSI_Config(SPIx_Reg_t *pSPIx, uint8_t EnorDi);
void SSOE_Config(SPIx_Reg_t *pSPIx, uint8_t EnorDi);
void SPI_ClearOVRFlag(SPIx_Reg_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pHandle);
void SPI_CloseReception(SPI_Handle_t *pHandle);

//Some interrupt related Macros
#define SPI_READY			0
#define SPI_BUSY_IN_RX		1
#define SPI_BUSY_IN_TX		2

#define SPI_EVENT_TX_CMPLT 			1
#define SPI_EVENT_RX_CMPLT			2
#define SPI_EVENT_OVR_ERR			3
#define SPI_EVENT_CRC_ERR			4
//Modes
#define SPI_DEVICE_MASTER	1
#define SPI_DEVICE_SLAVE	0

//SpeedControl using BRD
#define SPI_DRIVER_SPEED_DIV2	0
#define SPI_DRIVER_SPEED_DIV4	1
#define SPI_DRIVER_SPEED_DIV8	2
#define SPI_DRIVER_SPEED_DIV16	3
#define SPI_DRIVER_SPEED_DIV32	4
#define SPI_DRIVER_SPEED_DIV64	5
#define SPI_DRIVER_SPEED_DIV128	6
#define SPI_DRIVER_SPEED_DIV256	7

//BusConfig
#define SPI_BUS_FD				1
#define SPI_BUS_HD				2
#define SPI_BUS_SIMPLEX_RXONLY	3

//DFF
#define SHIFT_REGISTER_SIZE_8BITS			0
#define SHIFT_REGISTER_SIZE_16BITS			1

//SSM
#define SPI_SSM_EN				1
#define	SPI_SSM_DI				0

//CPOL
#define	CPOL_EN		1
#define CPOL_DI		0

//CPHA
#define	CPHA_EN		1
#define CPHA_DI		0

#define SPI_FLAG_TXE	(1<<SPI_SR_TXE)
#define SPI_FLAG_BUSY	(1<<SPI_SR_BSY)
#define SPI_FLAG_RXNE	(1<<SPI_SR_RXNE)

void SPI_ApplicationEventCallback(SPI_Handle_t *pHandle,uint8_t AppEv);

#endif /* INC_STM32F407_SPI_DRIVERS_H_ */
