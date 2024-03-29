#include<stdint.h>
#include "led.h"
uint32_t volatile* const address_RCC = (uint32_t*)(0x40023800+0x30);
uint32_t volatile* const address_GPIOMode = (uint32_t*)(0x40020000);
uint32_t volatile* const address_OUTPUT = (uint32_t*)(0x40020000+0x14);


void delay (uint32_t count){
	for(uint32_t i=0; i<count;i++);
}

void led_init(void){
	*address_RCC |=  1;
	*address_GPIOMode |= (1<<12);
	*address_GPIOMode |= (1<<10);
	*address_GPIOMode |= (1<<14);
	*address_GPIOMode |= (1<<2);

	led_off(A);
	led_off(B);
	led_off(C);
	led_off(D);
}

void led_on (uint32_t no){
	if(no==6 || no==7){
		*address_OUTPUT &= ~(1<<no);
	}
	else{
		*address_OUTPUT |= (1<<no);
	}
}

void led_off (uint32_t no){
	if(no==6 || no==7){
		*address_OUTPUT |= (1<<no);
	}
	else{
		*address_OUTPUT &= ~(1<<no);
	}
}
