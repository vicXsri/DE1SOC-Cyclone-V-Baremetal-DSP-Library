/*
 * clock.h
 *
 *  Created on: 01-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_

#include "main.h"

/* Typedef */
typedef struct{

	/* Clock Manager Module */
	__IO uint32_t ctrl; // Control Register
	__IO uint32_t bypass; // PLL Bypass Register
	__IO uint32_t instr; // Interrupt Status Register
	__IO uint32_t intrstr; // Interrupt Status Register
	__IO uint32_t intren; // Interrupt Enable Register
	__IO uint32_t dbctrl; // Debug clock Control Register
	__I  uint32_t clkstat; // Status Register
	uint32_t reserved0[10];

	/* Main PLL Group */
	__IO uint32_t mainvco; // Main PLL VCO Control Register
	__IO uint32_t mainmisc; // Main PLL VCO Advanced Control Register
	__IO uint32_t mpuclk; // Main PLL C0 Control Register for Clock mpu_clk
	__IO uint32_t mainclk; // Main PLL C1 Control Register for Clock main_clk
	__IO uint32_t dbgatclk; // Main PLL C2 Control Register for Clock dbg_base_clk
	__IO uint32_t mainqspiclk; // Main PLL C3 Control Register for Clock main_qspi_clk
	__IO uint32_t mainnandsdmmcclk; // Main PLL C4 Control Register for Clock main_nand_sdmmc_clk
	__IO uint32_t cfgs2fuser0clk; // Main PLL C5 Control Register for Clock cfg_s2f_user0_clk
	__IO uint32_t mainen; // Enable Register
	__IO uint32_t maindiv; // Main Divide Register
	__IO uint32_t dbgdiv; // Debug Divide Register
	__IO uint32_t tracediv; // Debug Trace Divide Register
	__IO uint32_t l4src; // L4 MP SP APB Clock Source
	__I  uint32_t mainstat; // L4 MP SP APB Clock Source
	uint32_t reserved1[2];

	/* Peripheral PLL Group */
	__IO uint32_t perivco; // Peripheral PLL VCO Control Register
	__IO uint32_t perimisc; // Peripheral PLL VCO Advanced Control Register
	__IO uint32_t emac0clk; // Peripheral PLL C0 Control Register for Clock emac0_clk
	__IO uint32_t emac1clk; // Peripheral PLL C1 Control Register for Clock emac1_clk
	__IO uint32_t perqspiclk; // Peripheral PLL C2 Control Register for Clock periph_qspi_clk
	__IO uint32_t pernandsdmmcclk; // Peripheral PLL C3 Control Register for Clock periph_nand_sdmmc_clk
	__IO uint32_t perbaseclk; // Peripheral PLL C4 Control Register for Clock periph_base_clk
	__IO uint32_t s2fuser1clk; // Peripheral PLL C5 Control Register for Clock s2f_user1_clk
	__IO uint32_t perien; // Enable Register
	__IO uint32_t peridiv; // Divide Register
	__IO uint32_t gpiodiv; // GPIO Divide Register
	__IO uint32_t perisrc; // Flash Clock Source Register
	__I  uint32_t peristat; // Peripheral PLL Output Counter Reset Ack Status Register
	uint32_t reserved2[3];

	/* SDRAM PLL Group */
	__IO uint32_t sdramvco; // SDRAM PLL VCO Control Register
	__IO uint32_t sdramctrl; // SDRAM PLL VCO Advanced Control Register
	__IO uint32_t ddrdqsclk; // SDRAM PLL C0 Control Register for Clock ddr_dqs_clk
	__IO uint32_t ddr2xdqsclk; // SDRAM PLL C1 Control Register for Clock ddr_2x_dqs_clk
	__IO uint32_t ddrdqclk; // SDRAM PLL C2 Control Register for Clock ddr_dq_clk
	__IO uint32_t s2fuser2clk; // SDRAM PLL C5 Control Register for Clock s2f_user2_clk
	__IO uint32_t sdramen; // Enable Register
	__I  uint32_t sdramstat; // SDRAM PLL Output Counter Reset Ack Status Register

}CLOCK_Typedef;



typedef struct{
	uint16_t safemode;
	uint16_t safemodedebug;
	uint16_t debugclk;
}CLOCK_ClockMgrTypedef;

typedef struct{
	bool enableMainVco;
	uint16_t numerator;
	uint16_t denominator;
	uint16_t bwadj;
	bool	saten;
	uint16_t mpuclkdiv;
	uint16_t mainclkdiv;
	uint16_t dbgatclkdiv;
	uint16_t mainqspiclkdiv;
	uint16_t mainnandsdmmcclkdiv;
	uint16_t cfgs2fuser0clkdiv;
	uint16_t l3mpclkdiv;
	uint16_t l3spclkdiv;
	uint16_t l4mpclkdiv;
	uint16_t l4spclkdiv;
	uint16_t dbgclk;
	uint16_t dbgatclk;
	uint16_t traceclk;
	uint16_t l4mpsrc;
	uint16_t l4spsrc;
}CLOCK_MainPLLTypedef;


typedef struct{
	bool enablePeripheralVco;
	uint16_t numerator;
	uint16_t denominator;
	bool saten;
	uint16_t bwadj;
	uint16_t emac0div;
	uint16_t emac1div;
	uint16_t perqspidiv;
	uint16_t pernandsdmmcdiv;
	uint16_t perbasediv;
	uint16_t s2fuser1div;
	uint16_t usbclkdiv;
	uint16_t spimclkdiv;
	uint16_t can0clkdiv;
	uint16_t can1clkdiv;
	uint16_t gpiodbclkdiv;
	uint16_t sdmmcclk;
	uint16_t nandclk;
	uint16_t qspiclk;

}CLOCK_PeripheralPLLTypedef;

typedef struct{
	bool enableSdramVco;
	uint16_t numerator;
	uint16_t denominator;
	uint16_t bwadj;
	bool	saten;
	uint16_t ddrdqsclkdiv;
	uint16_t ddr2xdqsclkdiv;
	uint16_t ddrdqclkdiv;
	uint16_t s2fuser2clkdiv;
	uint16_t sdramendiv;
}CLOCK_SDRAMPLLTypedef;

typedef struct{
	CLOCK_ClockMgrTypedef mainclkmgr;
	CLOCK_MainPLLTypedef mainpll;
	CLOCK_PeripheralPLLTypedef peripheralpll;
	CLOCK_SDRAMPLLTypedef sdrampll;
}CLOCK_InitTypedef;

/* Defines */
#define CLOCK 			((CLOCK_Typedef*) CLK_BASE)

/*Main PLL Enable*/
#define L4MAINEN		(0x01UL << 0)
#define L3MPEN			(0x01UL << 1)
#define L4MPEN			(0x01UL << 2)
#define L4SPEN			(0x01UL << 3)
#define DBGATEN			(0x01UL << 4)
#define DBGEN			(0x01UL << 5)
#define DBGTRACEEN		(0x01UL << 6)
#define DBGTIMEREN		(0x01UL << 7)
#define CFGEN			(0x01UL << 8)
#define S2FUSER0EN		(0x01UL << 9)

/*Peripheral PLL Enable*/
#define EMAC0EN			(0x01UL << 0);
#define EMAC1EN			(0x01UL << 1);
#define USBEN			(0x01UL << 2);
#define SPIEN			(0x01UL << 3);
#define CAN0EN			(0x01UL << 4);
#define CAN1EN			(0x01UL << 5);
#define GPIOEN			(0x01UL << 6);
#define S2FUSER1EN		(0x01UL << 7);
#define SDMMCEN			(0x01UL << 8);
#define NANDXEN			(0x01UL << 9);
#define NANDEN			(0x01UL << 10);
#define QSPIEN			(0x01UL << 11);

#define DDRDQSEN			(0x01UL << 0);
#define DDR2XDQSEN			(0x01UL << 1);
#define DDRDQEN				(0x01UL << 2);
#define S2FUSER2ENEN		(0x01UL << 3);
#define SDRAMEN				DDRDQSEN | DDR2XDQSEN | DDRDQEN |S2FUSER2ENEN;

#define CLKSAFEMODEEN			(0x01U << 2U)
#define CLKSAFEMODEDIS			false

#define CLKDEBUGAFEMODEEN		(0x01U << 1U)
#define CLKDEBUGAFEMODEDIS 		false

#define CLKDEBUGOSC1			(0x01U << 0U)


#define DIV1					 0x00U
#define DIV2					 0x01U
#define DIV4					 0x02U
#define DIV8					 0x03U
#define DIV16					 0x04U

#define MAINCLK					 0x00U
#define PERIBASECLK				 0x01U

#define PERIREFCLK				 0x00U
#define MAINNANDSDMMCCLK		 0x01U
#define PERIPHNANDSDMMCCLK		 0x02U

#define CLK_L4MAIN     0
#define CLK_L3MP       1
#define CLK_L4MP       2
#define CLK_L4SP       3
#define CLK_DBGATE     4
#define CLK_DBG        5
#define CLK_DBGTRACE   6
#define CLK_DBGTIMR    7
#define CLK_CFG        8
#define CLK_S2FUSER0   9
#define CLK_EMAC0      10
#define CLK_EMAC1      11
#define CLK_USB        12
#define CLK_SPI        13
#define CLK_CAN0       14
#define CLK_CAN1       15
#define CLK_GPIO       16
#define CLK_S2FUSER1   17
#define CLK_SDMMC      18
#define CLK_NANDX      19
#define CLK_NAND       20
#define CLK_QSPI       21
#define CLK_DDRDQS     22
#define CLK_DDR2XDQS   23
#define CLK_DDRDQ      24
#define CLK_S2FUSER2   25
#define CLK_OSC1	   26
#define CLK_F2SFREE	   27
#define CLK_SCHSDIV2   28
#define CLK_MAINPLL	   29
#define CLK_PERPPLL	   30
#define CLK_MPU		   31
#define CLK_MPUPHEP	   32
#define CLK_MPUL2RAM   33
#define CLK_L3MAIN	   34
#define CLK_L4SYS	   35
#define CLK_TIMER	   36
#define CLK_AT	   	   37
#define CLK_PLLREF 	   38

/* Funtions */
Status_TypeDef CLOCK_Init(CLOCK_Typedef* clk, CLOCK_InitTypedef* init);
void CLOCK_ClockMgrInit(CLOCK_Typedef* clk, CLOCK_InitTypedef* init);
void CLOCK_MainPllInit(CLOCK_Typedef* clk, CLOCK_InitTypedef* init);
void CLOCK_PeripheralPllInit(CLOCK_Typedef* clk, CLOCK_InitTypedef* init);
void CLOCK_SDRAMPllInit(CLOCK_Typedef* clk, CLOCK_InitTypedef* init);


void __CLOCK_L4MAIN_ENABLE(void);
void __CLOCK_L3MP_ENABLE(void);
void __CLOCK_L4MP_ENABLE(void);
void __CLOCK_L4SP_ENABLE(void);
void __CLOCK_DBGAT_ENABLE(void);
void __CLOCK_DBG_ENABLE(void);
void __CLOCK_DBGTRACE_ENABLE(void);
void __CLOCK_DBGTIMER_ENABLE(void);
void __CLOCK_CFG_ENABLE(void);
void __CLOCK_S2FUSER0_ENABLE(void);


void __CLOCK_EMAC0_ENABLE(void);
void __CLOCK_EMAC1_ENABLE(void);
void __CLOCK_USB_ENABLE(void);
void __CLOCK_SPI_ENABLE(void);
void __CLOCK_CAN0_ENABLE(void);
void __CLOCK_CAN1_ENABLE(void);
void __CLOCK_GPIO_ENABLE(void);
void __CLOCK_S2FUSER1_ENABLE(void);
void __CLOCK_SDMMC_ENABLE(void);
void __CLOCK_NANDX_ENABLE(void);
void __CLOCK_NAND_ENABLE(void);
void __CLOCK_QSPI_ENABLE(void);


void __CLOCK_SDRAM_ENABLE(void);


bool CLOCK_IsEnabled(uint32_t clk);
bool CLOCK_GetFrequency(uint32_t clk, uint32_t* freq);

void ClockGetFreqSDMMC(uint32_t* freq);

#endif /* INC_CLOCK_H_ */
