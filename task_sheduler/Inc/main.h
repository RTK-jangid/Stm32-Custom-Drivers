/*
 * main.h
 *
 *  Created on: Jan 25, 2024
 *      Author: ritik
 */

#ifndef MAIN_H_
#define MAIN_H_


#define MAX_TASKS 				5

#define SIZE_TASK_STACK  		1024U
#define SIZE_SCHED_STACK 		1024U

#define SRAM_START 				0x20000000U
#define SRAM_SIZE 				((128)*(1024))
#define SRAM_END 				((SRAM_SIZE)+(SRAM_START))

#define SYSTICK_TIM_CLK			16000000U



#define T1_SRAM_START			SRAM_END
#define T2_SRAM_START			((SRAM_END)-(1*(SIZE_TASK_STACK)))
#define T3_SRAM_START			((SRAM_END)-(2*(SIZE_TASK_STACK)))
#define T4_SRAM_START			((SRAM_END)-(3*(SIZE_TASK_STACK)))
#define IDLE_SRAM_START			((SRAM_END)-(4*(SIZE_TASK_STACK)))
#define SCHED_SRAM_START		((SRAM_END)-(5*(SIZE_TASK_STACK)))

#define TICK_HZ					1000U

#define DUMMY_XPSR				0x01000000U

#define TASK_RUNNING_STATE		0x00
#define TASK_BLOCKED_STATE		0xFF

#define INTERRUPT_DISABLE()	do{__asm volatile("MOV R0,#0x1"); __asm volatile("MSR PRIMASK,R0");}while(0)
#define INTERRUPT_ENABLE()	do{__asm volatile("MOV R0,#0x0"); __asm volatile("MSR PRIMASK,R0");}while(0)


#endif /* MAIN_H_ */
