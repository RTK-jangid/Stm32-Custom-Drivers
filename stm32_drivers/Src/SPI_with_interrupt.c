/*
 * SPI_COMMU_Arduino.c
 *
 *  Created on: Mar 5, 2024
 *      Author: ritik
 */


/*
 * SPI_dataSend_testing.c
 *
 *  Created on: Mar 4, 2024
 *      Author: ritik
 */

#include "stm32f407xx.h"
#include<stdint.h>
#include<stdio.h>
#include <string.h>

volatile uint8_t dataAvailable=0;

volatile uint8_t rcsv=0;
uint8_t readbyte;

char Buff[500];

SPI_Handle_t spi2;
void SPI2_inits(SPI_Handle_t *spi2){


	SPI2_ENABLE();
	spi2->pSPIx=SPI2_REG;

	spi2->SPIConfig.Mode=SPI_DEVICE_MASTER;
	spi2->SPIConfig.CPHA=CPHA_DI;
	spi2->SPIConfig.CPOL=CPOL_DI;
	spi2->SPIConfig.DFF = SHIFT_REGISTER_SIZE_8BITS;
	spi2->SPIConfig.SSM=SPI_SSM_DI;
	spi2->SPIConfig.SclkSpeed=SPI_DRIVER_SPEED_DIV32;
	spi2->SPIConfig.BusConfig = SPI_BUS_FD;
	SPI_init(spi2);

}


void GPIO_INITs(void){

	GPIO_Handle_t gpioB;

	gpioB.pGPIOx=GPIOB_REG;

	gpioB.GPIO_PinConfig.PinMode=GPIO_MODE_ALTFUN;
	gpioB.GPIO_PinConfig.PinAltFunMode=5;
	gpioB.GPIO_PinConfig.PinOPType=GPIO_OP_TYPE_PP;
	gpioB.GPIO_PinConfig.PinPuPdControl=GPIO_NO_PUPD;
	gpioB.GPIO_PinConfig.PinSpeed=GPIO_OSPEED_MED;

	//SCLK
	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_13;
	GPIO_init(&gpioB);

	//MISO
	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_14;
	GPIO_init(&gpioB);

	//MOSI
	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_15;
	GPIO_init(&gpioB);

	//NSS

	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_12;
	GPIO_init(&gpioB);

	GPIO_Handle_t gpioD_pin;
    /* Loop forever */


	gpioD_pin.pGPIOx = GPIOD_REG;
	gpioD_pin.GPIO_PinConfig.PinMode = GPIO_MODE_IT_FT;

	gpioD_pin.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_6;
	gpioD_pin.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	gpioD_pin.GPIO_PinConfig.PinPuPdControl = GPIO_PU;
	gpioD_pin.GPIO_PinConfig.PinSpeed = GPIO_OSPEED_MED;
	gpioD_pin.GPIO_PinConfig.PinAltFunMode = 0;

	GPIO_init(&gpioD_pin);

	GPIO_IRQConfig(EXTI5_9_IRQ_NO, ENABLE);
	//GPIO_IRQ_Set_Priority(EXTI5_9_IRQ_NO, 15);

}


int main(){

	uint8_t dummy_byte =0xff;



	GPIO_INITs();

	SPI2_inits(&spi2);

//	SSI_Config(SPI2_REG,ENABLE); //ONly when SSM is 1 or in software select mode

	SSOE_Config(SPI2_REG,ENABLE); //for automatic management of low and high of nss pin

	SPI_IRQConfig(SPI2_IRQ_NO, ENABLE);
	printf("Starte\n");
	while(1){
		rcsv=0;
		while(!dataAvailable);
		dataAvailable=0;
		GPIO_IRQConfig(EXTI5_9_IRQ_NO, DISABLE);

		SPI_Enable(SPI2_REG, ENABLE);

		while(!rcsv){
			while(SPI_SendIT(&spi2, &dummy_byte, 1) == SPI_BUSY_IN_TX);
			while(SPI_RecieveIT(&spi2, &readbyte, 1) == SPI_BUSY_IN_RX);
		}

		while(getFlag_Status(SPI2_REG, SPI_FLAG_BUSY) == FLAG_SET);

		SPI_Enable(SPI2_REG, DISABLE);
		printf("Received Data: %s \n",Buff);
		GPIO_IRQConfig(EXTI5_9_IRQ_NO, ENABLE);

	}

	return 0;
}
void SPI2_IRQHandler(void){
	SPI_IRQHandling(&spi2);
}
void SPI_ApplicationEventCallback(SPI_Handle_t *pHandle,uint8_t AppEv){
	static uint32_t i=0;
	if(AppEv==SPI_EVENT_RX_CMPLT) {

		Buff[i++]=readbyte;
		if(readbyte == "\0" || i==500){
			rcsv=1;
			Buff[i-1] ="\0" ;
			i=0;
//			dataAvailable=0;
		}
	}
}
void EXTI9_5_IRQHandler(void){
	dataAvailable=1;
	printf("come\n");
	GPIO_IRQHandling(GPIO_PIN_NO_6);
}


