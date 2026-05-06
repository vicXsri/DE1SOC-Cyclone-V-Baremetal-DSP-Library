/*
 * gic.c
 *
 *  Created on: 02-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "gic.h"

#include "arm_compat.h"

/* GIC Init */
Status_TypeDef GIC_Init(GIC_TypeDef* gic, IRQDIS_TypeDef* irqdis){

	if(gic == NULL || irqdis == NULL) return ARM_ERROR;

	/*First disable all the global irq service before configuiring of the Generic Interrupt Controller*/
	__disable_irq();

	/*Clear All the bits in the ICDICER Register*/
	for(uint8_t id=0; id < 8; id++)	irqdis->ICDICER[id] = 0xFFFFFFFFUL;


	/*Setting it to the lowest priority, that means any interrupt can pass through !*/
	gic->ICCPMR = 0xFFFFUL;

	/*This ICCICR, is the interrupt control register, when this register is set, interrupt is enabled !*/
	gic->ICCICR = 0x01U;

	/*This helps enable the GIC Interrrupts, that is the SPIs and PPIs,enables the distributor to update register locations for Secure interrupts*/
	irqdis->ICDDCR = 0x01U;


	/*At end of all the necessay configuration, turn on the global irq service*/
	__enable_irq();
	return ARM_OK;
}




