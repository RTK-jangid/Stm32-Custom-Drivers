/*
 * stm32407_GPIOx_drivers.h
 *
 *  Created on: Feb 24, 2024
 *      Author: ritik
 */

#ifndef INC_STM32F407_GPIOX_DRIVERS_H_
#define INC_STM32F407_GPIOX_DRIVERS_H_

#include "stm32f407xx.h"
#include <stdint.h>

typedef struct {
	uint8_t PinNumber;
	uint8_t PinMode;
	uint8_t PinSpeed;
	uint8_t PinPuPdControl;
	uint8_t PinOPType;
	uint8_t PinAltFunMode;
}GPIO_PinConfig_t;

typedef struct {
	GPIOx_Reg_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;

void GPIO_PeriClockControl(GPIOx_Reg_t *pGPIOx, uint8_t EnorDi);

void GPIO_init(GPIO_Handle_t *pGPIOHandle);
void GPIO_deinit(GPIOx_Reg_t *pGPIOx);


uint8_t GPIO_ReadFromInputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIOx_Reg_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIOx_Reg_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber);


void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);
void GPIO_IRQ_Set_Priority(uint8_t IRQNumber,uint32_t IRQPriority);

#define GPIO_MODE_IN		0
#define GPIO_MODE_OUT		1
#define GPIO_MODE_ALTFUN	2
#define GPIO_MODE_ANALOG	3
#define GPIO_MODE_IT_FT		4
#define GPIO_MODE_IT_RT		5
#define GPIO_MODE_IT_RFT	6

#define	GPIO_OP_TYPE_PP		0
#define GPIO_OP_TYPE_OD		1

#define GPIO_OSPEED_LOW		0
#define GPIO_OSPEED_MED		1
#define GPIO_OSPEED_HIGH	2
#define GPIO_OSPEED_VHIGH	3

#define GPIO_NO_PUPD		0
#define GPIO_PU				1
#define GPIO_PD				2

#define GPIO_OUT_HIGH		1
#define GPIO_OUT_LOW		0

#define GPIO_PIN_NO_0		0
#define GPIO_PIN_NO_1		1
#define GPIO_PIN_NO_2		2
#define GPIO_PIN_NO_3		3
#define GPIO_PIN_NO_4		4
#define GPIO_PIN_NO_5		5
#define GPIO_PIN_NO_6		6
#define GPIO_PIN_NO_7		7
#define GPIO_PIN_NO_8		8
#define GPIO_PIN_NO_9		9
#define GPIO_PIN_NO_10		10
#define GPIO_PIN_NO_11		11
#define GPIO_PIN_NO_12		12
#define GPIO_PIN_NO_13		13
#define GPIO_PIN_NO_14		14
#define GPIO_PIN_NO_15		15


#endif /* INC_STM32F407_GPIOX_DRIVERS_H_ */
