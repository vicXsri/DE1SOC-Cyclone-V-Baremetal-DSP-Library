/*
 * rstmgr.c
 *
 *  Created on: 12-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "rstmgr.h"

/* RSTMGR Enable Pheripheral */
void RSTMGR_PheripheralsSet(RSTMGR_TypeDef* rstmgr, uint32_t pheri){

	rstmgr->PERMODRST |= pheri;

}

/* RSTMGR Disable Pheripheral */
void RSTMGR_PheripheralsReset(RSTMGR_TypeDef* rstmgr, uint32_t pheri){

	rstmgr->PERMODRST &= ~pheri;

}

/* funtion for resseting a Pheripheral */
void RSTMGR_Pheripherals(RSTMGR_TypeDef* rstmgr, uint32_t pheri, uint32_t timeout){

	/* Set the bit for resetting */
	RSTMGR_PheripheralsSet(rstmgr, pheri);

	/* Wait until the peripheral is resseting*/
	while(timeout--);

	/* Reset the bit for resetting */
	RSTMGR_PheripheralsReset(rstmgr, pheri);

}
