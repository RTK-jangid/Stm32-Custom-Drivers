/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

int divide (int x ,int y){
	return x/y;
}
int main(void)
{	uint32_t volatile *pSBCRS =(uint32_t*)0xE000ED24;
	uint32_t volatile *pCCR=(uint32_t*)0xE000ED14;
	*pSBCRS |= (1<<18);
	*pSBCRS |= (1<<17);
	*pSBCRS |= (1<<16);
	*pCCR |= (1<<4);

	// For undefined instruction
//	uint32_t volatile *pSRAM =(uint32_t*)0x20010000;
//	*pSRAM = 0xFFFFFFFF; //un-defined opcode
//	void (*some_address)(void);
//	some_address =(void*)0x20010001;
//	some_address();
    /* Loop forever */
	// For divide by Zero
	divide(10,0);


	for(;;);
}

void HardFault_Handler (void){
	printf("HardFault Exception\n");
	while(1);
}

void MemManage_Handler (void){
	printf("Mem_manage Exception\n");
	while(1);
}

void BusFault_Handler (void){
	printf("BusFault Exception\n");
	while(1);
}

__attribute__((naked)) UsageFault_Handler (void){
	__asm volatile("MRS r0,MSP");
	__asm ("B UsageFault_Handler_c");
}

void UsageFault_Handler_c (uint32_t *baseStackAddress){
//	__asm volatile("MRS r0,MSP");
//	register uint32_t msp_value __asm("r0");
//	uint32_t *pMSP =(uint32_t*)msp_value;
	printf("UsageFault Exception\n");
	uint32_t *pUFSR =(uint32_t*)0xE000ED2A;
	printf("%lx\n",(*pUFSR & 0xFFFF));
//	printf("MSP= %p\n",pMSP);
	printf("R0= %lx\n",baseStackAddress[0]);
	printf("R1= %lx\n",baseStackAddress[1]);
	printf("R2= %lx\n",baseStackAddress[2]);
	printf("R3= %lx\n",baseStackAddress[3]);
	printf("R12= %lx\n",baseStackAddress[4]);
	printf("LR= %lx\n",baseStackAddress[5]);
	printf("PC= %lx\n",baseStackAddress[6]);
	printf("XPRS= %lx\n",baseStackAddress[7]);

	while(1);
}

