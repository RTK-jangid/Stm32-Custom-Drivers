/*
 * main1.c
 *
 *  Created on: Jan 26, 2024
 *      Author: ritik
 */


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
#include "main.h"
#include "led.h"

__attribute__((naked)) void init_sheduler_stack (uint32_t top_of_the_stack);
__attribute__((naked)) void switch_sp_to_psp(void);
void systick_init (uint32_t tick_hz);
void task1_handler (void);
void task2_handler (void);
void task3_handler (void);
void task4_handler (void);
void init_tasks_stack(void);
void exception_handling (void);
uint32_t get_psp_value (void);
void update_current_value (void);
void save_psp_value(uint32_t current_address_value);
void idle_task (void);
void task_delay(uint32_t tick_count);
void update_g_tick_count(void);
void schedule(void);

uint8_t current_task =1;//task1 is running
uint32_t g_tick_count=0;

typedef struct{
	uint32_t psp_value;
	uint32_t block_count;
	uint8_t current_state;
	void (*task_handler)(void);
}TCB_t;

TCB_t user_tasks[MAX_TASKS];


int main(void)
{
    exception_handling();

	init_sheduler_stack(SCHED_SRAM_START);

	init_tasks_stack();
	led_init();
	systick_init(TICK_HZ);
	switch_sp_to_psp();
	task1_handler();
	/* Loop forever */
	for(;;);
}


void exception_handling (void){
	uint32_t volatile *pSBCRS =(uint32_t*)0xE000ED24;
	*pSBCRS |= (1<<18);
	*pSBCRS |= (1<<17);
	*pSBCRS |= (1<<16);
}

__attribute__((naked)) void init_sheduler_stack (uint32_t top_of_the_stack){
	__asm volatile ("MSR MSP,%0" : :"r"(top_of_the_stack):);
	__asm volatile("BX LR");
}

uint32_t get_psp_value (void){
	return user_tasks[current_task].psp_value;
}

void save_psp_value(uint32_t current_address_value){
	user_tasks[current_task].psp_value=current_address_value;
}

void update_current_value (void){
	int state=TASK_BLOCKED_STATE;
	for(int i=0;i<(MAX_TASKS);i++){
		current_task++;
		current_task %= MAX_TASKS;
		state=user_tasks[current_task].current_state;
		if((state ==TASK_RUNNING_STATE) && (current_task !=0)){
			break;
		}
	}
	if(state !=TASK_RUNNING_STATE){
		current_task=0;
	}
}

void schedule(void){
	uint32_t *pICSR= (uint32_t*)0xE000ED04;
	*pICSR |= (1<<28);
}

void task_delay(uint32_t tick_count){

	//disable interrupt
	INTERRUPT_DISABLE();
	if(current_task){
		user_tasks[current_task].block_count=g_tick_count+tick_count;
		user_tasks[current_task].current_state=TASK_BLOCKED_STATE;
		schedule();
	}
	//enable interrupt
	INTERRUPT_ENABLE();
}

__attribute__((naked)) void switch_sp_to_psp(void){
	//initialize the psp of current task
	__asm volatile("PUSH {LR}");//store LR
	__asm volatile("BL get_psp_value");
	__asm volatile("MSR PSP,R0");//initialize psp
	__asm volatile("POP {LR}");//pop LR

	//change sp to psp
	__asm volatile("MOV R0,#0x02");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile("BX LR");


}


void systick_init (uint32_t tick_hz){
	uint32_t *pSRVR=(uint32_t*)0xE000E014;
	uint32_t count_value =((SYSTICK_TIM_CLK)/tick_hz)-1;
	uint32_t *pSCSR=(uint32_t*)0xE000E010;

	*pSRVR &= ~(0x00FFFFFF);
	*pSRVR |= count_value;

	// do some settings
	*pSCSR |= (1<<1);
	*pSCSR |= (1<<2);

	//enable systick timer
	*pSCSR |= (1<<0);
}

void init_tasks_stack(void){

	user_tasks[0].current_state =TASK_RUNNING_STATE;
	user_tasks[1].current_state =TASK_RUNNING_STATE;
	user_tasks[2].current_state =TASK_RUNNING_STATE;
	user_tasks[3].current_state =TASK_RUNNING_STATE;
	user_tasks[4].current_state =TASK_RUNNING_STATE;

	user_tasks[0].task_handler=idle_task;
	user_tasks[1].task_handler=task1_handler;
	user_tasks[2].task_handler=task2_handler;
	user_tasks[3].task_handler=task3_handler;
	user_tasks[4].task_handler=task4_handler;

	user_tasks[0].psp_value=IDLE_SRAM_START;
	user_tasks[1].psp_value=T1_SRAM_START;
	user_tasks[2].psp_value=T2_SRAM_START;
	user_tasks[3].psp_value=T3_SRAM_START;
	user_tasks[4].psp_value=T4_SRAM_START;
	uint32_t *pPSP;
	for(int i=0; i< MAX_TASKS;i++){
		pPSP= (uint32_t*)user_tasks[i].psp_value;

		pPSP--;//XPSR
		*pPSP = DUMMY_XPSR;

		pPSP--;//PC
		*pPSP = (uint32_t)user_tasks[i].task_handler;

		pPSP--;//LR
		*pPSP = 0xFFFFFFFD;
		for(int j=0 ; j<13;j++){
				pPSP--;
				*pPSP = 0;
			}
		user_tasks[i].psp_value=(uint32_t)pPSP;
	}


}

void idle_task (void){
	while(1);
}


void task1_handler (void){
	while(1){
		led_on(A);
		task_delay(DELAY_COUNT_1S);
		led_off(A);
		task_delay(DELAY_COUNT_1S);
	}
}

void task2_handler (void){
	while(1){
		led_on(B);
		task_delay(DELAY_COUNT_500mS);
		led_off(B);
		task_delay(DELAY_COUNT_500mS);
	}
}

void task3_handler (void){
	while(1){
		led_on(C);
		task_delay(DELAY_COUNT_250mS);
		led_off(C);
		task_delay(DELAY_COUNT_250mS);
	}
}

void task4_handler (void){
	while(1){
		led_on(D);
		task_delay(DELAY_COUNT_125mS);
		led_off(D);
		task_delay(DELAY_COUNT_125mS);
	}
}

__attribute__((naked)) void PendSV_Handler (void){
	__asm volatile("MRS R0,PSP");
	__asm volatile("STMDB R0!,{R4-R11}");
	__asm volatile("PUSH {LR}");
	__asm volatile("BL save_psp_value");
	__asm volatile("BL update_current_value");
	__asm volatile("BL get_psp_value");
	__asm volatile("LDMIA R0!,{R4-R11}");
	__asm volatile ("MSR PSP,R0");
	__asm volatile ("POP {LR}");
	__asm volatile ("BX LR");
}

void update_g_tick_count(void){
	g_tick_count++;
}

void unblock_tasks(void){
	for(int i=1; i<MAX_TASKS;i++){
		if(user_tasks[i].current_state!=TASK_RUNNING_STATE){
			if(user_tasks[i].block_count==g_tick_count){
				user_tasks[i].current_state=TASK_RUNNING_STATE;
			}
		}
	}
}

void SysTick_Handler (void){
	uint32_t *pICSR= (uint32_t*)0xE000ED04;

	update_g_tick_count();
	unblock_tasks();

	*pICSR |= (1<<28);

}