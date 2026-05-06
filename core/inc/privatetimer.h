/*
 * timebase.h
 *
 *  Created on: 02-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_PRIVATETIMER_H_
#define INC_PRIVATETIMER_H_

#include "main.h"
#include "../Drivers/HPS_IRQ/HPS_IRQ_IDs.h"
#include "arm_compat.h"

/* Defines */
#define CLOCK_FREQ			900000000UL
#define PRIVATETIM_FREQ		(CLOCK_FREQ/4)
#define	ONE_MSEC_LOAD		(PRIVATETIM_FREQ/1000U)
#define	MAX_DELAY			(0xFFFFFFFFUL)
#define TICK_FREQ			(1U)


/* Typedef */
typedef struct{
	__IO uint32_t LR; // Load Register
	__IO uint32_t CR; // Counter Register
	__IO uint32_t CNTRL; // Control Register
	__IO uint32_t ISR; // Interrupt Status Register
}PTim_TypeDef;

typedef struct{
	uint32_t ReloadValue;
	uint8_t  Prescaler;
}PTim_InitTypeDef;

/* Defines */
#define PTIM				((PTim_TypeDef*) PRIVATETIMER_BASE)

#define PRIVATETIM_EN			(0x01UL << 0U)
#define PRIVATETIM_AutoReload	(0x01UL << 1U)
#define PRIVATETIM_IrqEn		(0x01UL << 2U)

/* Funtions */
Status_TypeDef PTIM_Init(PTim_TypeDef* ptim , PTim_InitTypeDef* Init);
void PTIM_ActivateNotification(PTim_TypeDef* ptim);
void PTIM_IRQHandler();
uint32_t getTick(void);
void delay(uint32_t ms);
#endif /* INC_PRIVATETIMER_H_ */
