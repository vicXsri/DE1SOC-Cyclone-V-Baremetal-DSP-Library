/*
 * lt24.h
 *
 *  Created on: 27-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef EXTSENSORS_INC_LT24_H_
#define EXTSENSORS_INC_LT24_H_

#include "main.h"


/* Defines */
#define	LT_WRN			(0x01UL << 16U)
#define	LT_RS			(0x01UL << 17U)
#define	LT_RDN			(0x01UL << 18U)
#define	LT_CSN			(0x01UL << 19U)
#define	LT_RESET		(0x01UL << 20U)
#define	LT_LCD_ON		(0x01UL << 21U)
#define	LT_HP_OPT_ON	(0x01UL << 23U)
#define	LT_HP_OPT_OFF	(0x00UL << 23U)

#define LT_CMDDATA		(LT_CSN | LT_RDN | LT_RS | LT_WRN | 0xFFFFUL)
#define LT_PIO			(LT_CMDDATA | LT_LCD_ON | LT_RESET | LT_HP_OPT_ON)

#define MAX_HEIGHT		320
#define MAX_WIDTH		240


/* Funtions */
void LT24_INIT();

#endif /* EXTSENSORS_INC_LT24_H_ */
