/*
 * rstmgr.h
 *
 *  Created on: 12-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef HPS_INC_RSTMGR_H_
#define HPS_INC_RSTMGR_H_

#include "main.h"

/* Typedef */
typedef struct {
    __IO uint32_t STAT;         /*Status Register */
    __IO uint32_t CTRL;         /*Control Register */
    __IO uint32_t COUNTS;       /*Reset Cycles Count Register */
    __IO uint32_t MPUMODRST;    /*MPU Module Reset Register */
    __IO uint32_t PERMODRST;    /*Peripheral Module Reset Register */
    __IO uint32_t PER2MODRST;   /*Peripheral 2 Module Reset Register */
    __IO uint32_t BRGMODRST;    /*Bridge Module Reset Register */
    __IO uint32_t MISCMODRST;   /*Miscellaneous Module Reset Register */

    uint32_t RESERVED0[13];     /*Reserved */

    __IO uint32_t TSTSCRATCH;   /*Test Scratch Register */
} RSTMGR_TypeDef;

/* Defines */
#define RSTMGR	((RSTMGR_TypeDef*) RSTMGR_BASE)

#define RST_EMAC0        (1U << 0U)
#define RST_EMAC1        (1U << 1U)
#define RST_USB0         (1U << 2U)
#define RST_USB1         (1U << 3U)
#define RST_NAND         (1U << 4U)
#define RST_QSPI         (1U << 5U)
#define RST_14WD0        (1U << 6U)
#define RST_14WD1        (1U << 7U)
#define RST_OSC1TIMER0   (1U << 8U)
#define RST_OSC1TIMER1   (1U << 9U)
#define RST_SPTIMER0     (1U << 10U)
#define RST_SPTIMER1     (1U << 11U)
#define RST_I2C0         (1U << 12U)
#define RST_I2C1         (1U << 13U)
#define RST_I2C2         (1U << 14U)
#define RST_I2C3         (1U << 15U)

#define RST_UART0        (1U << 16U)
#define RST_UART1        (1U << 17U)
#define RST_SPIM0        (1U << 18U)
#define RST_SPIM1        (1U << 19U)
#define RST_SPIS0        (1U << 20U)
#define RST_SPIS1        (1U << 21U)
#define RST_SDMMC        (1U << 22U)
#define RST_CAN0         (1U << 23U)
#define RST_CAN1         (1U << 24U)
#define RST_GPIO0        (1U << 25U)
#define RST_GPIO1        (1U << 26U)
#define RST_GPIO2        (1U << 27U)
#define RST_DMA          (1U << 28U)
#define RST_SDR          (1U << 29U)

/* Funtions */
void RSTMGR_Pheripherals(RSTMGR_TypeDef* rstmgr, uint32_t pheri, uint32_t timeout);
void RSTMGR_PheripheralsReset(RSTMGR_TypeDef* rstmgr, uint32_t pheri);

#endif /* HPS_INC_RSTMGR_H_ */
