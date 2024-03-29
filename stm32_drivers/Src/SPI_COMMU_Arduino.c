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
#include<string.h>

#define COMMAND_LED_CTRL			0x50
#define COMMAND_SENSOR_READ			0x51
#define COMMAND_LED_READ			0x52
#define COMMAND_PRINT				0x53
#define COMMAND_ID_READ				0x54

uint8_t g_tick=0;

void delay(void){
	for(int i=0;i<500000;i++);
}
void delay1(void){
	for(int i=0;i<200;i++);
}

uint8_t SPI_VerifyResponse(uint8_t ack){
	if(ack==0xf5){
		return 1;
	}
	return 0;
}

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

	GPIO_Handle_t gpioE_btn;
    /* Loop forever */


	gpioE_btn.pGPIOx = GPIOE_REG;
	gpioE_btn.GPIO_PinConfig.PinMode = GPIO_MODE_IT_FT;

	gpioE_btn.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_3;
	gpioE_btn.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	gpioE_btn.GPIO_PinConfig.PinPuPdControl = GPIO_PU;
	gpioE_btn.GPIO_PinConfig.PinSpeed = GPIO_OSPEED_MED;
	gpioE_btn.GPIO_PinConfig.PinAltFunMode = 0;

	GPIO_init(&gpioE_btn);


	GPIO_IRQConfig(EXTI3_IRQ_NO, ENABLE);
	GPIO_IRQ_Set_Priority(EXTI3_IRQ_NO, 15);

}


int main(){

	SPI_Handle_t spi2;

	GPIO_INITs();

	SPI2_inits(&spi2);

//	SSI_Config(SPI2_REG,ENABLE); //ONly when SSM is 1 or in software select mode

	SSOE_Config(SPI2_REG,ENABLE); //for automatic management of low and high of nss pin

	while(1);
	return 0;
}
void EXTI3_IRQHandler(void){
	uint8_t dummy_byte=0xff;
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_3);
	if(g_tick%5==0){

		uint8_t user[]="An Arduino Uno board is best suited for beginners who have just started using microcontrollers, on the other hand, Arduino Mega board is for enthusiasts who require a lot of I/O pins for their projects";
		//uint8_t dummywrite;

		uint8_t dummyread;

		uint8_t ackbyte;

		uint8_t args[2];



		SPI_Enable(SPI2_REG,ENABLE);

		uint8_t command=COMMAND_PRINT;


		SPI_Send(SPI2_REG, &command, 1);
		SPI_Recieve(SPI2_REG, &dummyread, 1);

		SPI_Send(SPI2_REG, &dummy_byte, 1);
		SPI_Recieve(SPI2_REG,&ackbyte, 1);
		if (SPI_VerifyResponse(ackbyte)){
			args[0] = strlen((char*)user);
			SPI_Send(SPI2_REG,args,1);
			SPI_Recieve(SPI2_REG,&dummyread,1);

			delay();

			for(int i = 0 ; i < args[0] ; i++){
				SPI_Send(SPI2_REG,&user[i],1);
				SPI_Recieve(SPI2_REG,&dummyread,1);
			}
		}
		printf("COMMAND_PRINT Executed \n");


		while(getFlag_Status(SPI2_REG, SPI_FLAG_BUSY));

		SPI_Enable(SPI2_REG, DISABLE);
		g_tick++;
	}else if (g_tick%5==1){
		SPI_Enable(SPI2_REG,ENABLE);

		uint8_t command=COMMAND_LED_CTRL;

		uint8_t dummyread;

		uint8_t ackbyte;

		uint8_t args[2];
		SPI_Send(SPI2_REG, &command, 1);
		SPI_Recieve(SPI2_REG, &dummyread, 1);

		SPI_Send(SPI2_REG, &dummy_byte, 1);
		SPI_Recieve(SPI2_REG,&ackbyte, 1);

		if (SPI_VerifyResponse(ackbyte)){
			args[0]=9;
			args[1]=0;

			SPI_Send(SPI2_REG, args,2);
			SPI_Recieve(SPI2_REG, args,2);
			printf("COMMAND_LED_CTRL Executed\n");
		}

		while(getFlag_Status(SPI2_REG, SPI_FLAG_BUSY));

		SPI_Enable(SPI2_REG, DISABLE);
		g_tick++;

	}else if(g_tick%5==2){
		SPI_Enable(SPI2_REG,ENABLE);

		uint8_t command=COMMAND_SENSOR_READ;
		uint8_t args[2];
		uint8_t dummyread;
		uint8_t ans;
		uint8_t ackbyte;
		SPI_Send(SPI2_REG, &command, 1);

		SPI_Recieve(SPI2_REG, &dummyread, 1);

		SPI_Send(SPI2_REG, &dummy_byte, 1);
		SPI_Recieve(SPI2_REG,&ackbyte, 1);

		if (SPI_VerifyResponse(ackbyte)){
			args[0]=0;


			SPI_Send(SPI2_REG, args,1);
			SPI_Recieve(SPI2_REG,&dummyread,1);
			delay();
			SPI_Send(SPI2_REG,&dummy_byte, 1);
			SPI_Recieve(SPI2_REG,&ans,1);
		}



		while(getFlag_Status(SPI2_REG, SPI_FLAG_BUSY));

		SPI_Enable(SPI2_REG, DISABLE);
		g_tick++;
		printf("%d\n",ans);
	}else if(g_tick%5==3) {
		SPI_Enable(SPI2_REG,ENABLE);

		uint8_t command=COMMAND_LED_READ;

		uint8_t dummyread;
		uint8_t args[2];
		uint8_t ans;
		uint8_t ackbyte;
		SPI_Send(SPI2_REG, &command, 1);

		SPI_Recieve(SPI2_REG, &dummyread, 1);

		SPI_Send(SPI2_REG,&dummy_byte , 1);
		SPI_Recieve(SPI2_REG,&ackbyte, 1);

		if (SPI_VerifyResponse(ackbyte)){
			args[0]=9;


			SPI_Send(SPI2_REG, args,1);
			SPI_Recieve(SPI2_REG,&dummyread,1);

			delay();
			SPI_Send(SPI2_REG,&dummy_byte, 1);
			SPI_Recieve(SPI2_REG,&ans,1);
		}

		while(getFlag_Status(SPI2_REG, SPI_FLAG_BUSY));

		SPI_Enable(SPI2_REG, DISABLE);
		g_tick++;
		printf("%d\n",ans);
	}else{
		SPI_Enable(SPI2_REG,ENABLE);

		uint8_t command=COMMAND_ID_READ;

		uint8_t dummyread;
		uint8_t ackbyte;

		SPI_Send(SPI2_REG, &command, 1);

		SPI_Recieve(SPI2_REG, &dummyread, 1);

		SPI_Send(SPI2_REG,&dummy_byte , 1);

		SPI_Recieve(SPI2_REG, &ackbyte, 1);
		char ans[11];
		uint32_t i=0;
		if (SPI_VerifyResponse(ackbyte)){

		}

		for(i=0;i<10;i++){
			SPI_Send(SPI2_REG, &dummy_byte, 1);

			SPI_Recieve(SPI2_REG, (uint8_t*)&ans[i], 1);

		}
		//ans[10]="\0";
		printf("COMMAND_ID : %s \n",ans);


		while(getFlag_Status(SPI2_REG, SPI_FLAG_BUSY));

		SPI_Enable(SPI2_REG, DISABLE);
		g_tick++;

	}
}
