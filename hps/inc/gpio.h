/*
 * gpio.h
 *
 *  Created on: 31-Mar-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

/* Typedef */
typedef struct{
	__IO uint32_t dr; /*Data Register*/
	__IO uint32_t ddr; /*Data Direction Register*/
	uint32_t reserved0[10];
	__IO uint32_t inten; /*	Interrupt Enable Register*/
	__IO uint32_t intmask; /*Interrupt Mask Register*/
	__IO uint32_t intlvl; /*Interrupt Level Register*/
	__IO uint32_t intpol; /*Interrupt Polarity Register*/
	__I uint32_t intstr; /*Interrupt Status Register*/
	__I uint32_t rawintstr; /*Raw Interrupt Status Register*/
	__IO uint32_t deen; /*Debounce Enable Register*/
	__O uint32_t clrint; /*Clear Interrupt Register*/
	__I uint32_t extport; /*External Port Register*/
	uint32_t reserved1[3];
	__IO uint32_t synclvl; /*Synchronization Level Register*/
	__I uint32_t idCode; /*ID Code Register*/
	uint32_t reserved2;
	__I uint32_t veridcode; /*GPIO Version Register*/
	__I uint32_t cnfreg2; /*Configuration Register 2*/
	__I uint32_t cnfreg1; /*Configuration Register 1*/
}GPIO_TypeDef;

typedef struct{
	uint32_t Pin; // Specify the gpio pin
	uint32_t Mode; // Specify the Mode
}GPIO_InitTypeDef;

/* Defines */
#define GPIO0   ((GPIO_TypeDef*) GPIO0_BASE)
#define GPIO1  ((GPIO_TypeDef*) GPIO1_BASE)
#define GPIO2   ((GPIO_TypeDef*) GPIO2_BASE)


#define	GPIO_INPUT					(0x00U)
#define	GPIO_OUTPUT					(0x01U)
#define	GPIO_ALTERNATE				(0x02U)
#define	GPIO_ANALOG					(0x03U)

#define GPIO_PIN_0					(0x0000)
#define GPIO_PIN_1					(0x0001)
#define GPIO_PIN_2					(0x0002)
#define GPIO_PIN_3					(0x0003)
#define GPIO_PIN_4					(0x0004)
#define GPIO_PIN_5					(0x0005)
#define GPIO_PIN_6					(0x0006)
#define GPIO_PIN_7					(0x0007)
#define GPIO_PIN_8					(0x0008)
#define GPIO_PIN_9					(0x0009)
#define GPIO_PIN_10					(0x000A)
#define GPIO_PIN_11					(0x000B)
#define GPIO_PIN_12					(0x000C)
#define GPIO_PIN_13					(0x000D)
#define GPIO_PIN_14					(0x000E)
#define GPIO_PIN_15					(0x000F)
#define GPIO_PIN_16					(0x0010)
#define GPIO_PIN_17					(0x0011)
#define GPIO_PIN_18					(0x0012)
#define GPIO_PIN_19					(0x0013)
#define GPIO_PIN_20					(0x0014)
#define GPIO_PIN_21					(0x0015)
#define GPIO_PIN_22					(0x0016)
#define GPIO_PIN_23					(0x0017)
#define GPIO_PIN_24					(0x0018)
#define GPIO_PIN_25					(0x0019)
#define GPIO_PIN_26					(0x001A)

#define USER_LED					GPIO_PIN_24
#define USER_BUTTON					GPIO_PIN_25
#define I2C_MUX_HIGH				GPIO_PIN_19

/* Funtions */
Status_TypeDef GPIO_Init(GPIO_TypeDef* gpio, GPIO_InitTypeDef* GPIO_Init);
void GPIO_WritePin(GPIO_TypeDef* gpio, uint32_t GPIO_PIN, uint8_t state);
void GPIO_TogglePin(GPIO_TypeDef* gpio, uint32_t GPIO_PIN);
bool GPIO_ReadPin(GPIO_TypeDef* gpio, uint32_t GPIO_PIN);

#endif /* INC_GPIO_H_ */
