/*
 * timebase.h
 *
 *  Created on: 02-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "privatetimer.h"

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;

volatile uint32_t itr = 0;

/* Private Timer Initalize */
Status_TypeDef PTIM_Init(PTim_TypeDef* ptim, PTim_InitTypeDef* Init){

	if(ptim == NULL || Init == NULL) return ARM_ERROR;


	/* Setup the Reload Value */
	ptim->LR 	 = Init->ReloadValue;

	/* Setup the Prescaler, IRQ, AR Mode, Enable of Privater Timer */
	ptim->CNTRL |= (Init->Prescaler << 8U) 		|
				   (PRIVATETIM_AutoReload)	    |
				   (PRIVATETIM_EN);

	return ARM_OK;

}

/* Private Initalize Interrupt */
void PTIM_ActivateNotification(PTim_TypeDef* ptim){

		ptim->CNTRL |= PRIVATETIM_IrqEn;
		IRQ_Init(IRQ_MPCORE_PRIVATE_TIMER, PTIM_IRQHandler);
}

/* This is a function used to create a delay in blocking mode, need to pass the time in ms as parameter */
void delay (uint32_t ms){
	uint32_t tickstart = getTick();
	uint32_t wait = ms;

	if(wait < MAX_DELAY){
		wait+= (uint32_t)TICK_FREQ;
	}

	while((getTick() - tickstart) < wait){}
}

/* This is a function to get the current tick  */
uint32_t getTick(void){
	__disable_irq();
	g_curr_tick_p = g_curr_tick;
	__enable_irq();
	return g_curr_tick_p;
}

/* This is a funtion to increament tick value */
void Tick_Increament(){
	g_curr_tick += TICK_FREQ;
}

/* This is a funtion to Handle the Private timer interrupt, currently the tick is set to 1ms */
void PTIM_IRQHandler(){
	Tick_Increament();

	if(itr++ >= 5000){
		itr = 0;
		WDT_Reset(WDGT0);
	}

	PTIM->ISR = (0x01U << 0U);
}
