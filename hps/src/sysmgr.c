/*
 * sysmgr.c
 *
 *  Created on: 01-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "sysmgr.h"

/* SYSMGR Funtion for Muxing UART0 */
void __SYSMGR_MUX_UART0EN(SYSMGR_TypeDef* sysmgr){
	sysmgr->GENERALIO1 |= UART0_RX_MUXEN;
	sysmgr->GENERALIO2 |= UART0_TX_MUXEN;
}

/* SYSMGR Funtion for Muxing I2C0 */
void __SYSMGR_MUX_I2C0EN(SYSMGR_TypeDef* sysmgr){
	sysmgr->GENERALIO7  |= I2C0_SDA_MUXEN;
	sysmgr->GENERALIO8  |= I2C0_SCL_MUXEN;
	sysmgr->I2C0USEFPGA = 0;
}

/* SYSMGR Funtion for Muxing I2C1 */
/* SYSMGR Funtion for Muxing I2C1 */
void __SYSMGR_MUX_I2C1EN(SYSMGR_TypeDef* sysmgr){
	sysmgr->GENERALIO3 |= I2C1_SDA_MUXEN;
	sysmgr->GENERALIO4 |= I2C1_SCL_MUXEN;
}

/* SYSMGR Funtion for Muxing I2C2 */
void __SYSMGR_MUX_I2C2EN(SYSMGR_TypeDef* sysmgr){
	sysmgr->GENERALIO5 |= (0x02U << 0);
	sysmgr->GENERALIO6 |= (0x02U << 0);
}

/* SYSMGR Funtion for Muxing SDMMC */
void __SYSMGR_MUX_SDMMCEN(SYSMGR_TypeDef* sysmgr){

	sysmgr->FLASHIO0  |= SDMMC_CMD_MUXEN;

	sysmgr->FLASHIO2  |= SDMMC_D0_MUXEN;
	sysmgr->FLASHIO3  |= SDMMC_D1_MUXEN;
	sysmgr->FLASHIO10 |= SDMMC_D2_MUXEN;
	sysmgr->FLASHIO11 |= SDMMC_D3_MUXEN

	sysmgr->FLASHIO9  |= SDMMC_CLK_MUXEN;
}
