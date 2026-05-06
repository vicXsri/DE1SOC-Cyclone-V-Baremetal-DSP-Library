/*
 * wdt.h
 *
 *  Created on: 31-Mar-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_WDT_H_
#define INC_WDT_H_

#include "main.h"

/* Typedef */
typedef struct{
	__IO uint32_t cr; // Control Register
	__IO uint32_t torr; // Timeout Range Register
	__I uint32_t ccvr; // Current Counter Value Register
	__O uint32_t crr; // Counter Restart Register
	__I uint32_t intstr; // Interrupt Status Register.
	__I uint32_t intclr; // Interrupt Clear Register
	__I uint32_t topmax; // Component Parameters Register 5
	__I uint32_t topinitmax; // Component Parameters Register 4
	__I uint32_t toprst; // Component Parameters Register 3
	__I uint32_t cntrst; // Component Parameters Register 2
	__I uint32_t wdtparam; // Component Parameters Register 1
	__I uint32_t cmpver; // Component Version Register
	__I uint32_t cmptyp; // Component Type Register

}WATCHDOG_TypeDef;

/* Defines */
#define WDGT0  	((WATCHDOG_TypeDef*) WATCHDOG0_BASE)
#define WDGT1   ((WATCHDOG_TypeDef*) WATCHDOG1_BASE)

/* Funtions */
void WDT_Reset(WATCHDOG_TypeDef* wdt);
void WDT_Disable(WATCHDOG_TypeDef* wdt);

#endif /* INC_WDT_H_ */
