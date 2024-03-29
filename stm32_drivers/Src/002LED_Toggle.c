/*
 * 002LED_Toggle.c
 *
 *  Created on: Feb 27, 2024
 *      Author: ritik
 */


#include <stdint.h>
#include "stm32f407xx.h"
#include <stdio.h>
void delay(void){
	for(int i=0;i<250000;i++);
}
int main(void)
{	GPIO_Handle_t gpioA_led,gpioA_btn;
    /* Loop forever */
	GPIO_PeriClockControl(GPIOA_REG,ENABLE);
//	printf("%p \n",gpioA.pGPIOx->MODER);
	gpioA_led.pGPIOx = GPIOA_REG;
	gpioA_led.GPIO_PinConfig.PinMode = GPIO_MODE_OUT;
//	printf("%p \n",gpioA.pGPIOx->MODER);
	gpioA_led.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_6;
	gpioA_led.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	gpioA_led.GPIO_PinConfig.PinPuPdControl = GPIO_NO_PUPD;
	gpioA_led.GPIO_PinConfig.PinSpeed = GPIO_OSPEED_MED;

	gpioA_btn.pGPIOx = GPIOA_REG;
	gpioA_btn.GPIO_PinConfig.PinMode = GPIO_MODE_IN;
//	printf("%p \n",gpioA.pGPIOx->MODER);
	gpioA_btn.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_0;
//	gpioA_btn.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	gpioA_btn.GPIO_PinConfig.PinPuPdControl = GPIO_PD;
	gpioA_btn.GPIO_PinConfig.PinSpeed = GPIO_OSPEED_MED;

	GPIO_init(&gpioA_led);
	GPIO_init(&gpioA_btn);


	while(1){
		if(GPIO_ReadFromInputPin(gpioA_btn.pGPIOx, GPIO_PIN_NO_0)==HIGH){
			GPIO_ToggleOutputPin(GPIOA_REG, 6);
			delay();
		}
	}


	for(;;);
}
