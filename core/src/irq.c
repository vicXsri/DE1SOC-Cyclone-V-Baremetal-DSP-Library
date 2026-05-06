/*
 * irq.c
 *
 *  Created on: 03-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "arm_compat.h"

#include "irq.h"
#include "gic.h"


IRQ_Handler IRQh[256] = {0};

//extern UART_HandleTypeDef huart0;

/* IRQ Init */
void IRQ_Init(uint16_t id, IRQ_Handler irq){

	__disable_irq();

	/*Setup the Handler*/
	IRQh[id] = irq;

	/*register the Interrupt id in the Register*/
	IRQDIS->ICDISER[id/32] = (0x01U << (id & 31));

	/* This Register chooses which CPU the Interrupt should fire to !
	 * Each Register, contains 4 interrupts of 8 bits, 0-7, to choose the CPU Number !
	 */

	uint8_t* icdiptr = (uint8_t*)&IRQDIS->ICDIPTR[id/4];
	icdiptr[id & 0x03] = 0x01;

	__enable_irq();

}

/* The __irq_gic is the funtion which is called by the startup file when an interrupt triggers */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wextra"
__irq void __irq_gic(void){

//	hprintf(&huart0,"reached\r\n");

	uint32_t spsr = __AGET_PROC_SPSR();

	uint16_t irqn = GIC->ICCIAR;

	__ASET_PROC_SPSR(__AGET_PROC_CPSR());

	IRQh[irqn]();

	GIC->ICCEOIR = irqn;

	__ASET_PROC_SPSR(spsr);

}
#pragma clang diagnostic pop


