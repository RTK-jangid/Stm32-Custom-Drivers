/*
 * SPI_dataSend_testing.c
 *
 *  Created on: Mar 4, 2024
 *      Author: ritik
 */

#include "stm32f407xx.h"
#include<stdint.h>
#include<stdio.h>
#include<string.h>
void SPI2_inits(SPI_Handle_t *spi2){


	SPI2_ENABLE();
	spi2->pSPIx=SPI2_REG;

	spi2->SPIConfig.Mode=SPI_DEVICE_MASTER;
	spi2->SPIConfig.CPHA=CPHA_DI;
	spi2->SPIConfig.CPOL=CPOL_EN;
	spi2->SPIConfig.DFF = SHIFT_REGISTER_SIZE_8BITS;
	spi2->SPIConfig.SSM=SPI_SSM_EN;
	spi2->SPIConfig.SclkSpeed=SPI_DRIVER_SPEED_DIV2;
	spi2->SPIConfig.BusConfig = SPI_BUS_FD;
	SPI_init(spi2);



}


void GPIO_INITs(void){

	GPIO_Handle_t gpioB;

	gpioB.pGPIOx=GPIOB_REG;
	GPIO_PeriClockControl(gpioB.pGPIOx, ENABLE);
	gpioB.GPIO_PinConfig.PinMode=GPIO_MODE_ALTFUN;
	gpioB.GPIO_PinConfig.PinAltFunMode=5;
	gpioB.GPIO_PinConfig.PinOPType=GPIO_OP_TYPE_PP;
	gpioB.GPIO_PinConfig.PinPuPdControl=GPIO_NO_PUPD;
	gpioB.GPIO_PinConfig.PinSpeed=GPIO_OSPEED_HIGH;

	//SCLK
	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_13;
	GPIO_init(&gpioB);

	//MISO
//	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_14;
//	GPIO_init(&gpioB);

	//MOSI
	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_15;
	GPIO_init(&gpioB);

	//NSS
//	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_12;
//	GPIO_init(&gpioB);

}


int main(){
	char user[]="Hello World!";
	SPI_Handle_t spi2;

	GPIO_INITs();

	SPI2_inits(&spi2);

	SSI_Config(SPI2_REG,ENABLE);
	SPI_Enable(SPI2_REG,ENABLE);



	while(1){
		SPI_Send(SPI2_REG,(uint8_t*)user, strlen(user));
	}
	return 0;
}
