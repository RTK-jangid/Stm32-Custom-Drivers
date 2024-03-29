/*
 * 003led_toggle_with_btn_interrupt.c
 *
 *  Created on: Feb 29, 2024
 *      Author: ritik
 */


#include <stdint.h>
#include "stm32f407xx.h"
#include <stdio.h>
void delay(void){
	for(int i=0;i<250000;i++);
}
int main(void)
{	GPIO_Handle_t gpioA_led,gpioE_btn;
    /* Loop forever */
	GPIO_PeriClockControl(GPIOA_REG,ENABLE);
	GPIO_PeriClockControl(GPIOE_REG,ENABLE);
	gpioA_led.pGPIOx = GPIOA_REG;
	gpioA_led.GPIO_PinConfig.PinMode = GPIO_MODE_OUT;

	gpioA_led.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_6;
	gpioA_led.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	gpioA_led.GPIO_PinConfig.PinPuPdControl = GPIO_NO_PUPD;
	gpioA_led.GPIO_PinConfig.PinSpeed = GPIO_OSPEED_MED;

	gpioE_btn.pGPIOx = GPIOE_REG;
	gpioE_btn.GPIO_PinConfig.PinMode = GPIO_MODE_IT_FT;

	gpioE_btn.GPIO_PinConfig.PinNumber = GPIO_PIN_NO_3;
	gpioE_btn.GPIO_PinConfig.PinOPType = GPIO_OP_TYPE_PP;
	gpioE_btn.GPIO_PinConfig.PinPuPdControl = GPIO_PU;
	gpioE_btn.GPIO_PinConfig.PinSpeed = GPIO_OSPEED_MED;
	gpioE_btn.GPIO_PinConfig.PinAltFunMode = 0;
	GPIO_init(&gpioA_led);
	GPIO_init(&gpioE_btn);


	GPIO_IRQConfig(EXTI3_IRQ_NO, ENABLE);
	GPIO_IRQ_Set_Priority(EXTI3_IRQ_NO, 15);


	for(;;);
}


void EXTI3_IRQHandler(void){
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_3);
	GPIO_ToggleOutputPin(GPIOA_REG, 6);
}
