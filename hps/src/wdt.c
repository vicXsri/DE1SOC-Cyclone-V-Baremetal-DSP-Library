/*
 * wdt.c
 *
 *  Created on: 31-Mar-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "wdt.h"

/* Reset WDT Timer */
void WDT_Reset(WATCHDOG_TypeDef* wdt){
	wdt->crr = 0x76;
}

/* Disable WDT Timer */
void WDT_Disable(WATCHDOG_TypeDef* wdt){
	wdt->cr &= ~(1U << 0);
}
