/*
 * stm32f407_SPI_drivers.c
 *
 *  Created on: Mar 3, 2024
 *      Author: ritik
 */
#include "stm32f407_SPI_drivers.h"

static void spi_txe_interrupt_handle(SPI_Handle_t *pHandle){
	if(pHandle->pSPIx->CR1 & (1<<SPI_CR1_DFF)){
		pHandle->pSPIx->DR = *(uint16_t*)(pHandle->Txbuffer);
		pHandle->TxLen--;
		pHandle->TxLen--;
		(uint16_t*)(pHandle->Txbuffer)++;
		}
	else{
		pHandle->pSPIx->DR = *(pHandle->Txbuffer);
		pHandle->TxLen--;
		(pHandle->Txbuffer)--;
		}
	if(! pHandle->TxLen){
		SPI_CloseTransmission(pHandle);
		SPI_ApplicationEventCallback(pHandle,SPI_EVENT_TX_CMPLT);

	}
}
static void spi_rxne_interrupt_handle(SPI_Handle_t *pHandle){
	if(pHandle->pSPIx->CR1 & (1<<SPI_CR1_DFF)){
		*(uint16_t*)(pHandle->Rxbuffer) =pHandle->pSPIx->DR ;
		pHandle->RxLen--;
		pHandle->RxLen--;
		(uint16_t*)(pHandle->Rxbuffer)++;
		}
	else{
		*(pHandle->Rxbuffer) = pHandle->pSPIx->DR ;
		(pHandle->RxLen)--;
		(pHandle->Rxbuffer)--;
		}
	if(! pHandle->RxLen){
		SPI_CloseReception(pHandle);
		SPI_ApplicationEventCallback(pHandle,SPI_EVENT_RX_CMPLT);

	}
}
static void spi_ovrere_interrupt_handle(SPI_Handle_t *pHandle){

	if(pHandle->TxState != SPI_BUSY_IN_TX){
		SPI_ClearOVRFlag(pHandle->pSPIx);
	}
	SPI_ApplicationEventCallback(pHandle,SPI_EVENT_OVR_ERR);
}
uint8_t getFlag_Status(SPIx_Reg_t *pSPIx, uint8_t flag){
	if(pSPIx->SR & flag){
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SSOE_Config(SPIx_Reg_t *pSPIx, uint8_t EnorDi){
	if(EnorDi==ENABLE){
		pSPIx->CR2 |= (1<<SPI_CR2_SSOE);
	}else{
		pSPIx->CR2 &= ~(1<<SPI_CR2_SSOE);
	}

}

void SSI_Config(SPIx_Reg_t *pSPIx, uint8_t EnorDi){
	if(EnorDi==ENABLE){
		pSPIx->CR1 |= (1<<SPI_CR1_SSI);
	}else{
		pSPIx->CR1 &= ~(1<<SPI_CR1_SSI);
	}

}

void SPI_Enable(SPIx_Reg_t *pSPIx, uint8_t EnorDi){
	if(EnorDi==ENABLE){
		pSPIx->CR1 |= (1<<SPI_CR1_SPE);
	}else{
		pSPIx->CR1 &= ~(1<<SPI_CR1_SPE);
	}

}

void SPI_PeriClockControl(SPIx_Reg_t *pSPIx, uint8_t EnorDi){
	if(EnorDi==ENABLE){
		if(pSPIx==SPI1_REG){
			SPI1_ENABLE();
		}
		else if(pSPIx==SPI2_REG){
			SPI2_ENABLE();
		}else if(pSPIx==SPI3_REG){
			SPI3_ENABLE();
		}
	}
}

void SPI_init(SPI_Handle_t *pSPIHandle){

	pSPIHandle->pSPIx->CR1 = 0;

	pSPIHandle->pSPIx->CR1 |=(pSPIHandle->SPIConfig.Mode<<SPI_CR1_MSTR);


	pSPIHandle->pSPIx->CR1 &= ~(7<<SPI_CR1_BRD);
	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.SclkSpeed<<SPI_CR1_BRD);
	if(pSPIHandle->SPIConfig.BusConfig==SPI_BUS_FD){

		pSPIHandle->pSPIx->CR1 &= ~(1<<SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->SPIConfig.BusConfig==SPI_BUS_HD){
		pSPIHandle->pSPIx->CR1 |= (1<<SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->SPIConfig.BusConfig==SPI_BUS_SIMPLEX_RXONLY){
		pSPIHandle->pSPIx->CR1 &= ~(1<<SPI_CR1_BIDIMODE);
		pSPIHandle->pSPIx->CR1 |= (1<<SPI_CR1_RXONLY);
	}

	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.CPHA<<SPI_CR1_CPHA);

	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.CPOL<<SPI_CR1_CPOL);

	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.DFF<<SPI_CR1_DFF);

	pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.SSM<<SPI_CR1_SSM);
}

void SPI_deinit(SPIx_Reg_t *pSPIx){
	if(pSPIx==SPI1_REG){
		RCC_REG->APB2RSTR |= (1<<12);
		RCC_REG->APB2RSTR &= ~(1<<12);
	}else if(pSPIx==SPI2_REG){
		RCC_REG->APB1RSTR |= (1<<14);
		RCC_REG->APB1RSTR &= ~(1<<14);
	}else if(pSPIx==SPI3_REG){
		RCC_REG->APB1RSTR |= (1<<15);
		RCC_REG->APB1RSTR &= ~(1<<15);
	}
}

void SPI_Send(SPIx_Reg_t *pSPIx,uint8_t *TxBuffer,uint32_t Len){
	while(Len>0){

		while (getFlag_Status(pSPIx,SPI_FLAG_TXE)==FLAG_RESET);

		if(pSPIx->CR1 & (1<<SPI_CR1_DFF)){
			pSPIx->DR = *(uint16_t*)TxBuffer;
			Len--;
			Len--;
			(uint16_t*)TxBuffer++;
		}else{
			pSPIx->DR = *TxBuffer;
			Len--;
			TxBuffer++;
		}
	}
}

void SPI_Recieve(SPIx_Reg_t *pSPIx,uint8_t *RxBuffer,uint32_t Len){
	while(Len>0){

		while (getFlag_Status(pSPIx,SPI_FLAG_RXNE)==FLAG_RESET);

		if(pSPIx->CR1 & (1<<SPI_CR1_DFF)){
			*(uint16_t*)RxBuffer= pSPIx->DR ;
			Len--;
			Len--;
			(uint16_t*)RxBuffer++;
		}else{
			*RxBuffer = pSPIx->DR ;
			Len--;
			RxBuffer++;
		}
	}
}
uint8_t SPI_SendIT(SPI_Handle_t *pHandle,uint8_t *TxBuffer,uint32_t Len){
	uint8_t state = pHandle->TxState;
	if (state != SPI_BUSY_IN_TX){
		pHandle->Txbuffer= TxBuffer;
		pHandle->TxLen=Len;
		pHandle->TxState=SPI_BUSY_IN_TX;
		pHandle->pSPIx->CR2 |= (1<<SPI_CR2_TXNEIE);
	}
	return state;
}
uint8_t SPI_RecieveIT(SPI_Handle_t *pHandle,uint8_t *RxBuffer,uint32_t Len){
	uint8_t state = pHandle->RxState;
	if (state != SPI_BUSY_IN_RX){
		pHandle->Rxbuffer= RxBuffer;
		pHandle->RxLen=Len;
		pHandle->RxState=SPI_BUSY_IN_RX;
		pHandle->pSPIx->CR2 |= (1<<SPI_CR2_RXNEIE);
	}
	return state;
}


void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi){

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

void SPI_IRQHandling(SPI_Handle_t *pHandle){
	uint8_t temp1 = pHandle->pSPIx->CR2 & (1<<SPI_CR2_TXNEIE);
	uint8_t temp2 =pHandle->pSPIx->SR & (1<<SPI_SR_TXE);

	if(temp1 && temp2 ){
		spi_txe_interrupt_handle(pHandle);
	}
	temp1 = pHandle->pSPIx->CR2 & (1<<SPI_CR2_RXNEIE);
	temp2 =pHandle->pSPIx->SR & (1<<SPI_SR_RXNE);
	if(temp1 && temp2 ){
		spi_rxne_interrupt_handle(pHandle);
	}
	temp1 = pHandle->pSPIx->CR2 & (1<<SPI_CR2_ERRIE);
	temp2 =pHandle->pSPIx->SR & (1<<SPI_SR_OVR);
	if(temp1 && temp2 ){
		spi_ovrere_interrupt_handle(pHandle);
	}
}

void SPI_IRQ_Set_Priority(uint8_t IRQNumber,uint32_t IRQPriority){
	uint8_t temp1 = (IRQNumber/4);
	uint8_t temp2 = (IRQNumber%4);
	temp2 = (8*temp2)+ (8-NO_BITS_PR_IMPLEMENTED);
	*(NVIC_IPR + temp1) |= (IRQPriority <<(temp2));
}

void SPI_CloseTransmission(SPI_Handle_t *pHandle){
	pHandle->pSPIx->CR2 &= ~(1<<SPI_CR2_TXNEIE);
	pHandle->Txbuffer=NULL;
	pHandle->TxLen=0;
	pHandle->TxState=SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pHandle){
	pHandle->pSPIx->CR2 &= ~(1<<SPI_CR2_RXNEIE);
	pHandle->Rxbuffer=NULL;
	pHandle->RxLen=0;
	pHandle->RxState=SPI_READY;
}

void SPI_ClearOVRFlag(SPIx_Reg_t *pSPIx){
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;

}

__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pHandle,uint8_t AppEv){

}
