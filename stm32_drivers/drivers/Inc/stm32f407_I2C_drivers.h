/*
 * stm32f407_I2C_drivers.h
 *
 *  Created on: Mar 14, 2024
 *      Author: ritik
 */
#include<stdint.h>
#include "stm32f407xx.h"
#ifndef INC_STM32F407_I2C_DRIVERS_H_
#define INC_STM32F407_I2C_DRIVERS_H_

typedef struct{
	volatile uint32_t I2C_SCLSpeed;
	volatile uint8_t I2C_DeviceAddress;
	volatile uint8_t I2C_ACKControl;
	volatile uint8_t I2C_FMDutyCycle;
}I2C_Config_t;

typedef struct{
	  I2Cx_Reg_t *pI2Cx;
	  I2C_Config_t I2CConfig;
	  volatile uint8_t *pTxBuffer;
	  volatile uint8_t *pRxBuffer;
	  volatile uint32_t TxLen;
	  volatile uint32_t RxLen;
	  volatile uint8_t TxRxState;
	  volatile uint8_t DevAddr;
	  volatile uint8_t RxSize;
	  volatile uint8_t Sr;
}I2C_Handle_t;

void I2C_PeriClockControl(I2Cx_Reg_t *pGPIOx, uint8_t EnorDi);

void I2C_init(I2C_Handle_t *pI2CHandle);
void I2C_deinit(I2Cx_Reg_t *pI2Cx);

void I2C_SendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t len, uint8_t SlaveAddress);
void I2C_RecieveData(I2C_Handle_t *pI2CHandle,uint8_t *pRxbuffer, uint32_t len, uint8_t SlaveAddress);

uint8_t I2C_SendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t len, uint8_t SlaveAddress,uint8_t SR);
uint8_t I2C_RecieveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t len, uint8_t SlaveAddress,uint8_t SR);

void Manage_Acking(I2C_Handle_t *pI2CHandle , uint8_t EnorDi);

void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQ_Set_Priority(uint8_t IRQNumber,uint32_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ERR_IRQHandling(I2C_Handle_t *pI2CHandle);

uint8_t I2C_getFlag_Status(I2Cx_Reg_t *pI2Cx, uint8_t flag);

void I2C_Enable(I2Cx_Reg_t *pI2Cx, uint8_t EnorDi);

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);
void I2C_CloseRecieveData(I2C_Handle_t *pI2CHandle);

void I2CGenerateStopCondition(I2Cx_Reg_t *pI2Cx);

void I2C_ApplicationCallBack(I2C_Handle_t *pI2CHandle,uint8_t AppEv);

void I2C_SlaveSendData(I2Cx_Reg_t *pI2C,uint8_t data);
uint8_t I2C_SlaveReceiveData(I2Cx_Reg_t *pI2C);

void I2C_SlaveEnableDisable_IT_Bits(I2Cx_Reg_t *pI2Cx,uint8_t EnorDi);



#define I2C_SCL_SPEED_STD		100000U
#define I2C_SCL_SPEED_FAST		400000U
#define I2C_SCL_SPEED_FAST_2	200000U

#define I2C_ACK_ENABLE			1
#define I2C_ACK_DISABlE			0

#define I2C_FM_DUTY_2			0
#define I2C_FM_DUTY_16_9		1

#define I2C_FLAG_TXE	 		(1<<I2C_SR1_TXE)
#define I2C_FLAG_SB		 		(1<<I2C_SR1_SB)
#define I2C_FLAG_RXNE		    (1<<I2C_SR1_RXNE)
#define I2C_FLAG_OVR	 		(1<<I2C_SR1_OVR)
#define I2C_FLAG_AF				(1<<I2C_SR1_AF)
#define I2C_FLAG_ARLO			(1<<I2C_SR1_ARLO)
#define I2C_FLAG_BERR			(1<<I2C_SR1_BERR)
#define I2C_FLAG_STOPF			(1<<(I2C_SR1_STOPF))
#define I2C_FLAG_ADD10			(1<<I2C_SR1_ADD10)
#define I2C_FLAG_BTF			(1<<I2C_SR1_BTF)
#define I2C_FLAG_ADDR			(1<<I2C_SR1_ADDR)
#define I2C_FLAG_TIMEOUT		(1<<I2C_SR1_TIMEOUT)

#define I2C_READY		 		0
#define I2C_BUSY_IN_TX	 		1
#define I2C_BUSY_IN_RX	 		2

#define I2C_EV_TX_COMPLT 		0
#define I2C_EV_RX_COMPLT 		1
#define I2C_EV_STOP 	 		2
#define I2C_ERROR_BERR  		3
#define I2C_ERROR_ARLO  		4
#define I2C_ERROR_AF    		5
#define I2C_ERROR_OVR  			6
#define I2C_ERROR_TIMEOUT 		7
#define I2C_EV_REQ				8
#define I2C_EV_REC				9

#endif /* INC_STM32F407_I2C_DRIVERS_H_ */
