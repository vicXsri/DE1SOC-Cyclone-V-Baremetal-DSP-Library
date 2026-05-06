/*
 * gpio.c
 *
 *  Created on: 31-Mar-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "gpio.h"

/* Intialize GPIO */
Status_TypeDef GPIO_Init(GPIO_TypeDef* gpio, GPIO_InitTypeDef* GPIO_Init){

	if(gpio == NULL || GPIO_Init == NULL)	return ARM_ERROR;

	if(GPIO_Init->Mode == GPIO_OUTPUT){
		gpio->ddr |= (1UL << GPIO_Init->Pin);
	}else{
		gpio->ddr &= ~(1UL << GPIO_Init->Pin);
	}
	return ARM_OK;
}

/* GPIO Write Pin Funtion*/
void GPIO_WritePin(GPIO_TypeDef* gpio, uint32_t GPIO_PIN, uint8_t state){
	if(state){
		gpio->dr |= (1UL << GPIO_PIN);

	}
	else{
		gpio->dr &= ~(1UL << GPIO_PIN);
	}
}

/* GPIO Toggle Pin Funtion*/
void GPIO_TogglePin(GPIO_TypeDef* gpio, uint32_t GPIO_PIN){
	gpio->dr ^= (1UL << GPIO_PIN);
}

/* GPIO Read Pin Funtion*/
bool GPIO_ReadPin(GPIO_TypeDef* gpio, uint32_t GPIO_PIN){
	return (gpio->extport & (1UL << GPIO_PIN));
}
