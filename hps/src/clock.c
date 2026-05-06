/*
 * clock.c
 *
 *  Created on: 01-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "clock.h"


/* Initalize clock pass  CLOCK_Typedef & CLOCK_InitTypedef as input */
Status_TypeDef CLOCK_Init(CLOCK_Typedef* clk, CLOCK_InitTypedef* init){

	if(clk == NULL) return ARM_ERROR;

	/* Configure Main Clock Group */
	CLOCK_ClockMgrInit(clk, init);

	/* Configure Main PLL Group */
	CLOCK_MainPllInit(clk, init);

	/* Configure Pheripheral PLL Group */
	CLOCK_PeripheralPllInit(clk, init);

	/* Configure SDRAM PLL Group */
	CLOCK_SDRAMPllInit(clk, init);

	return ARM_OK;
}

/* Configure Main Clock Group */
void CLOCK_ClockMgrInit(CLOCK_Typedef* clk, CLOCK_InitTypedef* init){

	clk->ctrl |= 			init->mainclkmgr.safemode;

	clk->dbctrl |= 			init->mainclkmgr.debugclk | init->mainclkmgr.safemodedebug;

}

/* Configure Main PLL Group */
void CLOCK_MainPllInit(CLOCK_Typedef* clk, CLOCK_InitTypedef* init){

	clk->mainvco  |= 		(init->mainpll.enableMainVco << 1U) | (init->mainpll.numerator << 3U) | (init->mainpll.denominator << 16U) ;

	clk->mainmisc |= 		(init->mainpll.bwadj << 1U) | (init->mainpll.saten << 14UL);

	clk->mpuclk = 			init->mainpll.mpuclkdiv;
	clk->mainclk = 			init->mainpll.mainclkdiv;
	clk->dbgatclk = 		init->mainpll.dbgatclkdiv;
	clk->mainqspiclk = 		init->mainpll.mainqspiclkdiv;
	clk->mainnandsdmmcclk = init->mainpll.mainnandsdmmcclkdiv;
	clk->cfgs2fuser0clk = 	init->mainpll.cfgs2fuser0clkdiv;

	clk->maindiv = 			(init->mainpll.l3mpclkdiv << 0U) | (init->mainpll.l3spclkdiv << 2U) | (init->mainpll.l4mpclkdiv << 4UL) |(init->mainpll.l4spclkdiv << 7UL);

	clk->dbgatclk = 		(init->mainpll.dbgatclk << 0U  ) | (init->mainpll.dbgclk << 1U);

	clk->tracediv = 		(init->mainpll.traceclk << 0U  );

	clk->l4src = 			(init->mainpll.l4mpsrc << 0U   ) | (init->mainpll.l4spsrc << 1U );
}

/* Configure Pheripheral PLL Group */
void CLOCK_PeripheralPllInit(CLOCK_Typedef* clk, CLOCK_InitTypedef* init){

	clk->perivco  |= 		(init->peripheralpll.enablePeripheralVco << 1U  ) | (init->peripheralpll.numerator << 3U ) | (init->peripheralpll.denominator << 16UL );
	clk->perimisc |= 		(init->peripheralpll.bwadj << 1U) | (init->peripheralpll.saten << 14UL);

	clk->emac0clk =			init->peripheralpll.emac0div;
	clk->emac1clk =			init->peripheralpll.emac1div;
	clk->perqspiclk =		init->peripheralpll.perqspidiv;
	clk->pernandsdmmcclk =	init->peripheralpll.pernandsdmmcdiv;
	clk->perbaseclk =		init->peripheralpll.pernandsdmmcdiv;
	clk->s2fuser1clk =		init->peripheralpll.pernandsdmmcdiv;

	clk->peridiv =			(init->peripheralpll.usbclkdiv << 0U) | (init->peripheralpll.spimclkdiv << 3U) |(init->peripheralpll.can0clkdiv << 6UL) |(init->peripheralpll.can1clkdiv << 9UL);

	clk->gpiodiv =			init->peripheralpll.gpiodbclkdiv;
	clk->perisrc =			(init->peripheralpll.sdmmcclk << 0U) | (init->peripheralpll.nandclk << 2U) | (init->peripheralpll.qspiclk << 4UL);

}

/* Configure SDRAM PLL Group */
void CLOCK_SDRAMPllInit(CLOCK_Typedef* clk, CLOCK_InitTypedef* init){

	clk->sdramvco  |= 		(init->sdrampll.enableSdramVco << 1U  ) | (init->sdrampll.numerator << 3U ) | (init->sdrampll.denominator << 16UL );
	clk->sdramctrl |= 		(init->sdrampll.bwadj << 1U) | (init->sdrampll.saten << 14UL);

	clk->ddrdqsclk |= 		init->sdrampll.ddrdqsclkdiv;
	clk->ddr2xdqsclk |= 	init->sdrampll.ddr2xdqsclkdiv;
	clk->ddrdqclk |= 		init->sdrampll.ddrdqclkdiv;
	clk->s2fuser2clk |= 	init->sdrampll.s2fuser2clkdiv;
	clk->sdramen |= 		init->sdrampll.sdramendiv;

}

/* Clock Enable Functions For different Peripherals */

void __CLOCK_L4MAIN_ENABLE(void){
	CLOCK->mainen |= L4MAINEN;
}

void __CLOCK_L3MP_ENABLE(void){
	CLOCK->mainen |= L3MPEN;
}

void __CLOCK_L4MP_ENABLE(void){
	CLOCK->mainen |= L4MPEN;
}

void __CLOCK_L4SP_ENABLE(void){
	CLOCK->mainen |= L4SPEN;
}

void __CLOCK_DBGAT_ENABLE(void){
	CLOCK->mainen |= DBGATEN;
}

void __CLOCK_DBG_ENABLE(void){
	CLOCK->mainen |= DBGEN;
}

void __CLOCK_DBGTRACE_ENABLE(void){
	CLOCK->mainen |= DBGTRACEEN;
}

void __CLOCK_DBGTIMER_ENABLE(void){
	CLOCK->mainen |= DBGTIMEREN;
}

void __CLOCK_CFG_ENABLE(void){
	CLOCK->mainen |= CFGEN;
}

void __CLOCK_S2FUSER0_ENABLE(void){
	CLOCK->mainen |= S2FUSER0EN;
}

void __CLOCK_EMAC0_ENABLE(void){
	CLOCK->perien |= EMAC0EN;
}

void __CLOCK_EMAC1_ENABLE(void){
	CLOCK->perien |= EMAC1EN;
}

void __CLOCK_USB_ENABLE(void){
	CLOCK->perien |= USBEN;
}

void __CLOCK_SPI_ENABLE(void){
	CLOCK->perien |= SPIEN;
}

void __CLOCK_CAN0_ENABLE(void){
	CLOCK->perien |= CAN0EN;
}

void __CLOCK_CAN1_ENABLE(void){
	CLOCK->perien |= CAN1EN;
}

void __CLOCK_GPIO_ENABLE(void){
	CLOCK->perien |= GPIOEN;
}

void __CLOCK_S2FUSER1_ENABLE(void){
	CLOCK->perien |= S2FUSER1EN;
}

void __CLOCK_SDMMC_ENABLE(void){
	CLOCK->perien |= SDMMCEN;
}

void __CLOCK_NANDX_ENABLE(void){
	CLOCK->perien |= NANDXEN;
}

void __CLOCK_NAND_ENABLE(void){
	CLOCK->perien |= NANDEN;
}

void __CLOCK_QSPI_ENABLE(void){
	CLOCK->perien |= QSPIEN;
}

void __CLOCK_SDRAM_ENABLE(void){
	CLOCK->sdramen |= DDRDQSEN;
	CLOCK->sdramen |= DDR2XDQSEN;
	CLOCK->sdramen |= DDRDQEN;
	CLOCK->sdramen |= S2FUSER2ENEN;

}

/* Function returns if clock is enabled or not for a specific Pheripheral */
bool CLOCK_IsEnabled(uint32_t clk){
	switch (clk){
		case CLK_L4MAIN:
			return (CLOCK->mainen & (0x01UL << 0U));
		break;

		case CLK_L3MP:
			return (CLOCK->mainen & (0x01UL << 1U));
		break;

		case CLK_L4MP:
			return (CLOCK->mainen & (0x01UL << 2U));
		break;

		case CLK_L4SP:
			return (CLOCK->mainen & (0x01UL << 3U));
		break;

		case CLK_DBGATE:
			return (CLOCK->mainen & (0x01UL << 4U));
		break;

		case CLK_DBG:
			return (CLOCK->mainen & (0x01UL << 5U));
		break;

		case CLK_DBGTRACE:
			return (CLOCK->mainen & (0x01UL << 6U));
		break;

		case CLK_DBGTIMR:
			return (CLOCK->mainen & (0x01UL << 7U));
		break;

		case CLK_CFG:
			return (CLOCK->mainen & (0x01UL << 8U));
		break;

		case CLK_S2FUSER0:
			return (CLOCK->mainen & (0x01UL << 9U));
		break;

		case CLK_EMAC0:
			return (CLOCK->perien & (0x01UL << 0U));
		break;

		case CLK_EMAC1:
			return (CLOCK->perien & (0x01UL << 1U));
		break;

		case CLK_USB:
			return (CLOCK->perien & (0x01UL << 2U));
		break;

		case CLK_SPI:
			return (CLOCK->perien & (0x01UL << 3U));
		break;

		case CLK_CAN0:
			return (CLOCK->perien & (0x01UL << 4U));
		break;

		case CLK_CAN1:
			return (CLOCK->perien & (0x01UL << 5U));
		break;

		case CLK_GPIO:
			return (CLOCK->perien & (0x01UL << 6U));
		break;

		case CLK_S2FUSER1:
			return (CLOCK->perien & (0x01UL << 7U));
		break;

		case CLK_SDMMC:
			return (CLOCK->perien & (0x01UL << 8U));
		break;

		case CLK_NANDX:
			return (CLOCK->perien & (0x01UL << 9U));
		break;

		case CLK_NAND:
			return (CLOCK->perien & (0x01UL << 10U));
		break;

		case CLK_QSPI:
			return (CLOCK->perien & (0x01UL << 11U));
		break;

		case CLK_DDRDQS:
			return (CLOCK->sdramen & (0x01UL << 0U));
		break;

		case CLK_DDR2XDQS:
			return (CLOCK->sdramen & (0x01UL << 1U));
		break;

		case CLK_DDRDQ:
			return (CLOCK->sdramen & (0x01UL << 2U));
		break;

		case CLK_S2FUSER2:
			return (CLOCK->sdramen & (0x01UL << 3U));
		break;

		default:
			return false;
		break;

	}
	return false;
}

/* Provides Clock Frequency  of Various Peripherals */
bool CLOCK_GetFrequency(uint32_t clk, uint32_t* freq){

	switch(clk){

		case CLK_OSC1:
			return 0;
		break;

		case CLK_F2SFREE:
			return 0;
		break;

		case CLK_SCHSDIV2:
			return 0;
		break;

		case CLK_MAINPLL:
			return 0;
		break;

		case CLK_PERPPLL:
			return 0;
		break;

		case CLK_MPU:
			return 0;
		break;

		case CLK_MPUPHEP:
			return 0;
		break;

		case CLK_MPUL2RAM:
			return 0;
		break;

		case CLK_L3MAIN:
			return 0;
		break;

		case CLK_L4SYS:
			return 0;
		break;

		case CLK_L4MAIN:
			return 0;
		break;

		case CLK_L4MP:
			return 0;
		break;

		case CLK_L4SP:
			return 0;
		break;

		case CLK_TIMER:
			return 0;
		break;

		case CLK_AT:
			return 0;
		break;

		case CLK_DBG:
			return 0;
		break;

		case CLK_DBGTRACE:
			return 0;
		break;

		case CLK_S2FUSER0:
			return 0;
		break;

		case CLK_S2FUSER1:
			return 0;
		break;

		case CLK_EMAC0:
			return 0;
		break;

		case CLK_EMAC1:
			return 0;
		break;

		case CLK_GPIO:
			return 0;
		break;

		case CLK_SDMMC:
			ClockGetFreqSDMMC(freq);
			return true;
		break;

		case CLK_QSPI:
			return 0;
		break;
	}

	return 0;
}

void ClockGetFreqSDMMC(uint32_t* freq){
	*freq = 200000000UL; // will update later !
}

