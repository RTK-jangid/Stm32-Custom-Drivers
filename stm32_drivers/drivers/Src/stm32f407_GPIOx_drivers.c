/*
 * stm32_GPIOx_drivers.c
 *
 *  Created on: Feb 24, 2024
 *      Author: ritik
 */


#include "stm32f407_GPIOx_drivers.h"


void GPIO_PeriClockControl(GPIOx_Reg_t *pGPIOx, uint8_t EnorDi){

	if(EnorDi){
		if(pGPIOx==GPIOA_REG){
//			printf("%p \n",pGPIOx->MODER);
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx==GPIOB_REG){
			RCC_REG->AHB1ENR |= (1<<1);
		}
		else if(pGPIOx==GPIOC_REG){
			RCC_REG->AHB1ENR |= (1<<2);
		}
		else if(pGPIOx==GPIOD_REG){
			RCC_REG->AHB1ENR |= (1<<3);
		}
		else if(pGPIOx==GPIOE_REG){
			RCC_REG->AHB1ENR |= (1<<4);
		}
		else if(pGPIOx==GPIOF_REG){
			RCC_REG->AHB1ENR |= (1<<5);
		}
		else if(pGPIOx==GPIOG_REG){
			RCC_REG->AHB1ENR |= (1<<6);
		}
		else if(pGPIOx==GPIOH_REG){
			RCC_REG->AHB1ENR |= (1<<7);
		}
		else if(pGPIOx==GPIOI_REG){
			RCC_REG->AHB1ENR |= (1<<8);
		}

	}
	else{
		if(pGPIOx==GPIOA_REG){
			RCC_REG->AHB1ENR &= ~(1<<0);
		}
		else if(pGPIOx==GPIOB_REG){
			RCC_REG->AHB1ENR &= ~(1<<1);
		}
		else if(pGPIOx==GPIOC_REG){
			RCC_REG->AHB1ENR &= ~(1<<2);
		}
		else if(pGPIOx==GPIOD_REG){
			RCC_REG->AHB1ENR &= ~(1<<3);
		}
		else if(pGPIOx==GPIOE_REG){
			RCC_REG->AHB1ENR &= ~(1<<4);
		}
		else if(pGPIOx==GPIOF_REG){
			RCC_REG->AHB1ENR &= ~(1<<5);
		}
		else if(pGPIOx==GPIOG_REG){
			RCC_REG->AHB1ENR &= ~(1<<6);
		}
		else if(pGPIOx==GPIOH_REG){
			RCC_REG->AHB1ENR &= ~(1<<7);
		}
		else if(pGPIOx==GPIOI_REG){
			RCC_REG->AHB1ENR &= ~(1<<8);
		}
	}


}

void GPIO_init(GPIO_Handle_t *pGPIOHandle){
//	printf("%p \n",pGPIOHandle->pGPIOx->MODER);
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);
	if (pGPIOHandle->GPIO_PinConfig.PinMode	<= GPIO_MODE_ANALOG	){
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2*pGPIOHandle->GPIO_PinConfig.PinNumber));
		pGPIOHandle->pGPIOx->MODER |= (pGPIOHandle->GPIO_PinConfig.PinMode << (2*pGPIOHandle->GPIO_PinConfig.PinNumber));
	}
	else{
		SYSCFG_ENABLE();
		if(pGPIOHandle->GPIO_PinConfig.PinMode==GPIO_MODE_IT_FT){
			EXTI_REG->RTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.PinNumber);
			EXTI_REG->FTRS |= (1<<pGPIOHandle->GPIO_PinConfig.PinNumber);
		}else if (pGPIOHandle->GPIO_PinConfig.PinMode==GPIO_MODE_IT_RT){
			EXTI_REG->RTSR |= (1<<pGPIOHandle->GPIO_PinConfig.PinNumber);
			EXTI_REG->FTRS &= ~(1<<pGPIOHandle->GPIO_PinConfig.PinNumber);
		}else if (pGPIOHandle->GPIO_PinConfig.PinMode==GPIO_MODE_IT_RFT){
			EXTI_REG->RTSR |= (1<<pGPIOHandle->GPIO_PinConfig.PinNumber);
			EXTI_REG->FTRS |= (1<<pGPIOHandle->GPIO_PinConfig.PinNumber);
		}


		uint8_t temp1 =	(pGPIOHandle->GPIO_PinConfig.PinNumber)/4;
		uint8_t temp2 =	(pGPIOHandle->GPIO_PinConfig.PinNumber)%4;

		uint8_t portCode = PORT_SELECT_EXTI(pGPIOHandle->pGPIOx);

		SYSCFG_REG->EXTI_CR[temp1] |= (portCode<<(4*temp2));

		EXTI_REG->IMR |= 1<< pGPIOHandle->GPIO_PinConfig.PinNumber;

	}


	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << (pGPIOHandle->GPIO_PinConfig.PinNumber));
	pGPIOHandle->pGPIOx->OTYPER |= (pGPIOHandle->GPIO_PinConfig.PinOPType << (pGPIOHandle->GPIO_PinConfig.PinNumber));

	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2*pGPIOHandle->GPIO_PinConfig.PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR |= (pGPIOHandle->GPIO_PinConfig.PinSpeed << (2*pGPIOHandle->GPIO_PinConfig.PinNumber));

	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2*pGPIOHandle->GPIO_PinConfig.PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= (pGPIOHandle->GPIO_PinConfig.PinPuPdControl << (2*pGPIOHandle->GPIO_PinConfig.PinNumber));

	if (pGPIOHandle->GPIO_PinConfig.PinMode == GPIO_MODE_ALTFUN){

		uint32_t temp1 = (pGPIOHandle->GPIO_PinConfig.PinNumber % 8);
		uint32_t temp2 = (pGPIOHandle->GPIO_PinConfig.PinNumber / 8);

		pGPIOHandle->pGPIOx->AFR[temp2]	&= ~(0xf << (4*temp1));
		pGPIOHandle->pGPIOx->AFR[temp2]	|= (pGPIOHandle->GPIO_PinConfig.PinAltFunMode << (4*temp1));

	}

}
void GPIO_deinit(GPIOx_Reg_t *pGPIOx){
	if(pGPIOx==GPIOA_REG){
		RCC_REG->AHB1RSTR |= (1<<0);
		RCC_REG->AHB1RSTR &= ~(1<<0);
	}
	else if(pGPIOx==GPIOB_REG){
		RCC_REG->AHB1RSTR |= (1<<1);
		RCC_REG->AHB1RSTR &= ~(1<<1);
	}
	else if(pGPIOx==GPIOC_REG){
		RCC_REG->AHB1RSTR |= (1<<2);
		RCC_REG->AHB1RSTR &= ~(1<<2);
	}
	else if(pGPIOx==GPIOD_REG){
		RCC_REG->AHB1RSTR |= (1<<3);
		RCC_REG->AHB1RSTR &= ~(1<<3);
	}
	else if(pGPIOx==GPIOE_REG){
		RCC_REG->AHB1RSTR |= (1<<4);
		RCC_REG->AHB1RSTR &= ~(1<<4);
	}
	else if(pGPIOx==GPIOF_REG){
		RCC_REG->AHB1RSTR |= (1<<5);
		RCC_REG->AHB1RSTR &= ~(1<<5);
	}
	else if(pGPIOx==GPIOG_REG){
		RCC_REG->AHB1RSTR |= (1<<6);
		RCC_REG->AHB1RSTR &= ~(1<<6);
	}
	else if(pGPIOx==GPIOH_REG){
		RCC_REG->AHB1RSTR |= (1<<7);
		RCC_REG->AHB1RSTR &= ~(1<<7);
	}
	else if(pGPIOx==GPIOI_REG){
		RCC_REG->AHB1RSTR |= (1<<8);
		RCC_REG->AHB1RSTR &= ~(1<<8);
	}
}


uint8_t GPIO_ReadFromInputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber){
	return (((pGPIOx->IDR) >> PinNumber) & 0x1);
}

uint16_t GPIO_ReadFromInputPort(GPIOx_Reg_t *pGPIOx){
	return	pGPIOx->IDR;
}
void GPIO_WriteToOutputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
	if (Value==GPIO_SET) {
		pGPIOx->ODR |= (1 <<PinNumber);
	}
	else{
		pGPIOx->ODR &= ~(1 <<PinNumber);
	}
}
void GPIO_WriteToOutputPort(GPIOx_Reg_t *pGPIOx, uint16_t Value){
	pGPIOx->ODR = Value;
}
void GPIO_ToggleOutputPin(GPIOx_Reg_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^= (1<<PinNumber);
}


void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi){

	if (EnorDi==ENABLE){

		if (-1<IRQNumber && IRQNumber<32){
			*NVIC_ISER0 |= (1<<(IRQNumber));
		}else if(31<IRQNumber && IRQNumber<64){
			*NVIC_ISER1 |= (1<<(IRQNumber%32));

		}else if(63<IRQNumber && IRQNumber<96){
			*NVIC_ISER2 |= (1<<(IRQNumber%32));
		}

	}else{
		if (-1<IRQNumber && IRQNumber<32){
			*NVIC_ICER0 |= (1<<(IRQNumber));
		}else if(31<IRQNumber && IRQNumber<64){
			*NVIC_ICER1 |= (1<<(IRQNumber%32));

		}else if(63<IRQNumber && IRQNumber<96){
			*NVIC_ICER2 |= (1<<(IRQNumber%64));
		}
	}

}

void GPIO_IRQ_Set_Priority(uint8_t IRQNumber,uint32_t IRQPriority){
	uint8_t temp1 = (IRQNumber/4);
	uint8_t temp2 = (IRQNumber%4);
	temp2 = (8*temp2)+ (8-NO_BITS_PR_IMPLEMENTED);
	*(NVIC_IPR + temp1) |= (IRQPriority <<(temp2));
}
void GPIO_IRQHandling(uint8_t PinNumber){
	if (EXTI_REG->PR & (1<<PinNumber)){
		EXTI_REG->PR |= (1<<PinNumber);
	}
}

