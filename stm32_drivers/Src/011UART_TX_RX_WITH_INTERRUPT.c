/*
 * 011UART_TX_RX_WITH_INTERRUPT.c
 *
 *  Created on: Mar 28, 2024
 *      Author: ritik
 */




#include "stm32f407xx.h"
#include<stdint.h>
#include<stdio.h>
#include<string.h>


uint8_t some[] = "Something is Tested..";
uint8_t rx_buf[1024];
uint8_t rcv_set =RESET;
void delay(void){
	for(int i=0;i<500000;i++);
}


USART_Handle_t usart1;

void USART1_inits(USART_Handle_t *pUSARTHandle){

	pUSARTHandle->pUARTx = USART1_REG;

	pUSARTHandle->USARTConfig.BAUD=USART_STD_BAUD_115200;
	pUSARTHandle->USARTConfig.HW_Flow_Ctrl=USART_HW_FLOW_CTRL_NONE;
	pUSARTHandle->USARTConfig.Mode=USART_MODE_TXRX;
	pUSARTHandle->USARTConfig.Parity_Ctrl=USART_PARITY_DISABLE;
	pUSARTHandle->USARTConfig.No_of_Stop_Bits=USART_STOPBITS_1;
	pUSARTHandle->USARTConfig.WordLength=USART_WORDLEN_8BITS;

	USART_IRQInterruptConfig(USART1_IRQ_NO, ENABLE);

	USART_Init(&usart1);
}


void GPIO_INITs(void){

	GPIO_Handle_t gpioB;

	gpioB.pGPIOx=GPIOB_REG;

	gpioB.GPIO_PinConfig.PinMode=GPIO_MODE_ALTFUN;
	gpioB.GPIO_PinConfig.PinAltFunMode=7;
	gpioB.GPIO_PinConfig.PinOPType=GPIO_OP_TYPE_PP;
	gpioB.GPIO_PinConfig.PinPuPdControl=GPIO_PU;
	gpioB.GPIO_PinConfig.PinSpeed=GPIO_OSPEED_MED;

	//Tx
	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_6;
	GPIO_init(&gpioB);

	//Rx
	gpioB.GPIO_PinConfig.PinNumber=GPIO_PIN_NO_7;
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

	GPIO_INITs();

	USART1_inits(&usart1);

	USART_PeripheralControl(&usart1, ENABLE);

	while(1);
	return 0;
}
void EXTI3_IRQHandler(void){
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_3);


	while(USART_SendDataIT(&usart1, some,strlen((char*)some)) != USART_READY);
	while(USART_ReceiveDataIT(&usart1, rx_buf,strlen((char*)some)) != USART_READY);

//	USART_SendData(&usart1, some, strlen((char*)some));

	printf("Transmitted : %s\n",some);
	while(rcv_set!=SET);

	printf("Received    : %s\n",rx_buf);
	rcv_set =RESET;
}

void USART1_IRQHandler(void){

	USART_IRQHandling(&usart1);

}

void USART_EventCallBack(USART_Handle_t *pUSARTHandle , uint8_t AppEv){
	if (AppEv == USART_TX_COMPLT){
		;
	}else if(AppEv == USART_RX_COMPLT){
		rcv_set=SET;
	}

}

