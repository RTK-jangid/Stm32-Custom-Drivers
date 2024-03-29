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

int fun_add( int a,int b, int c,int d){
	return a+b+c+d;
}

__attribute__((naked)) void change_sp_to_psp(void){
	__asm volatile(".equ SRAM_END , (0x20000000+(128*1024))");
	__asm volatile(".equ PSP_START , (SRAM_END-512)");
	__asm volatile("LDR R0 ,=PSP_START");
	__asm volatile("MSR PSP,R0");
	__asm volatile("MOV R0 ,#0x02");
	__asm volatile("MSR CONTROL ,R0");
	__asm volatile("BX LR");
}

void generate_exception(void){
	__asm volatile("SVC #0x2");
}
int main(void)
{
	change_sp_to_psp();
	int ret;
    ret=fun_add(1,2,3,4);
    /* Loop forever */

    printf("%d\n",ret);

    generate_exception();

    for(;;);
}

void SVC_Handler (void){
	printf("In handler mode and switched to MSP\n");
}