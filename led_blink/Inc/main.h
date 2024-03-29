/*
 * main.h
 *
 *  Created on: Jan 10, 2024
 *      Author: ritik
 */

#ifndef MAIN_H_
#define MAIN_H_
typedef struct{
	uint32_t pin0 :1;
	uint32_t pin1 :1;
	uint32_t pin2 :1;
	uint32_t pin3 :1;
	uint32_t pin4 :1;
	uint32_t pin5 :1;
	uint32_t pin6 :1;
	uint32_t pin7 :1;
	uint32_t pin8 :1;
	uint32_t pin9 :1;
	uint32_t pin10 :1;
	uint32_t pin11 :1;
	uint32_t pin12 :1;
	uint32_t pin13 :1;
	uint32_t pin14 :1;
	uint32_t pin15 :1;
	uint32_t reserved :16;
}GPIOx_ODR_t;

typedef struct{
	uint32_t pin0 :2;
	uint32_t pin1 :2;
	uint32_t pin2 :2;
	uint32_t pin3 :2;
	uint32_t pin4 :2;
	uint32_t pin5 :2;
	uint32_t pin6 :2;
	uint32_t pin7 :2;
	uint32_t pin8 :2;
	uint32_t pin9 :2;
	uint32_t pin10 :2;
	uint32_t pin11 :2;
	uint32_t pin12 :2;
	uint32_t pin13 :2;
	uint32_t pin14 :2;
	uint32_t pin15 :2;

}GPIOx_MODER_t;


typedef struct{
	uint32_t GPIOAEN :1;
	uint32_t GPIOBEN :1;
	uint32_t GPIOCEN :1;
	uint32_t GPIODEN :1;
	uint32_t GPIOEEN :1;
	uint32_t GPIOFEN :1;
	uint32_t GPIOGEN :1;
	uint32_t GPIOHEN :1;
	uint32_t GPIOIEN :1;
	uint32_t GPIOJEN :1;
	uint32_t GPIOKEN :1;
	uint32_t reseved1 :1;
	uint32_t CRCEN :1;
	uint32_t reserved2 :5;
	uint32_t BKPSRAMEN :1;
	uint32_t reserved3 :1;
	uint32_t CCMDATARAMEN :1;
	uint32_t DMA1EN :1;
	uint32_t DMA2EN :1;
	uint32_t DMA2DEN :1;
	uint32_t reserved4 :1;
	uint32_t ETHMACEN :1;
	uint32_t ETHMACTXEN :1;
	uint32_t ETHMACRXEN :1;
	uint32_t ETHMACPTPEN :1;
	uint32_t OTGHSEN :1;
	uint32_t OTGHSULPHIEN :1;
	uint32_t reserved5 :1;

}RCC_AHB1ENR_t;

#endif /* MAIN_H_ */
