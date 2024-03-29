/*
 * led.h
 *
 *  Created on: Jan 25, 2024
 *      Author: ritik
 */

#ifndef LED_H_
#define LED_H_

#define A 		6
#define B		5
#define C		7
#define D		1

#define DELAY_COUNT_1mS 	1250U
#define DELAY_COUNT_1S		(1000)
#define DELAY_COUNT_500mS   (500)
#define DELAY_COUNT_250mS   (250)
#define DELAY_COUNT_125mS   (125)


void led_init(void);
void delay (uint32_t count);
void led_on (uint32_t no);
void led_off (uint32_t no);
#endif /* LED_H_ */
