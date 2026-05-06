/*
 * msp.c
 *
 *  Created on: 01-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "msp.h"


void UART_MspInit(UART_HandleTypeDef* uart){

//	__CLOCK_L4SP_ENABLE(); // when i try setting this bit the whole L4_SP line gets corrupted,
						   // So do not use this !

	if(uart->Instance == UART0){
		/* Enable the MUX Config for Uart Rx & Tx*/
		__SYSMGR_MUX_UART0EN(SYSMGR);
	}
}

void SDMMC_MspInit(SDMMC_TypeDef* sdmmc){

	//	__CLOCK_SDMMC_ENABLE(); // Don't turn on it's already turned on the Bootloader !

	if(sdmmc == SDMMC){

		/* Enable the MUX Config for SDMMC */
		__SYSMGR_MUX_SDMMCEN(SYSMGR);
	}
}

void I2C_MspInit(I2C_HandleTypeDef* i2c){
	//	__CLOCK_L4SP_ENABLE(); // do not use this ! // I2C uses l4_sp_clk
							   // L4_SP_CLK is 100 MHz
	if(i2c->Instance == I2C0){
		/* Enable the MUX Config for I2C0 */
		__SYSMGR_MUX_I2C0EN(SYSMGR);
	}
	if(i2c->Instance == I2C1){
		/* Enable the MUX Config for I2C1 */
		__SYSMGR_MUX_I2C1EN(SYSMGR);
	}
	if(i2c->Instance == I2C2){
		/* Enable the MUX Config for I2C2 */
		__SYSMGR_MUX_I2C2EN(SYSMGR);
	}

}
