/*
 * sysmgr.h
 *
 *  Created on: 01-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_SYSMGR_H_
#define INC_SYSMGR_H_

#include "main.h"

/* Typedef */
typedef struct {

    /* System Manager Module */

    __I uint32_t siliconid1;
    __I uint32_t siliconid2;
    uint32_t reserved0[2];

    __IO uint32_t wddbg;
    __I uint32_t bootinfo;
    __I uint32_t hpsinfo;
    __IO uint32_t parityinj;

    /* FPGA Interface Group */

    __IO uint32_t gbl;
    __IO uint32_t indiv;
    __IO uint32_t module;
    uint32_t reserved1;

    /* Scan Manager */

    __IO uint32_t ctrl_scan;
    uint32_t reserved2[3];

    /* Freeze Control */

    __IO uint32_t vioctrl;
    uint32_t reserved3[3];
    __IO uint32_t hioctrl;
    __IO uint32_t src;
    __IO uint32_t hwctrl;
    uint32_t reserved4[1];

    /* EMAC */

    __IO uint32_t emac_ctrl;
    __IO uint32_t emac_l3master;
    uint32_t reserved5[2];

    /* DMA */

    __IO uint32_t dma_ctrl;
    __IO uint32_t persecurity;
    uint32_t reserved6[2];

    /* Preloader */

    __IO uint32_t handoff;
    uint32_t reserved7[15];

    /* Boot ROM */

    __IO uint32_t rom_ctrl;
    __IO uint32_t cpu1startaddr;
    __IO uint32_t initswstate;
    __IO uint32_t initswlastld;
    __IO uint32_t bootromswstate;
    uint32_t reserved8[3];

    /* Warm Boot */

    __IO uint32_t wb_enable;
    __IO uint32_t wb_datastart;
    __IO uint32_t wb_length;
    __IO uint32_t wb_execution;
    __IO uint32_t wb_crc;
    uint32_t reserved9[3];

    /* Boot ROM HW */

    __IO uint32_t romhw_ctrl;
    uint32_t reserved10;

    /* SDMMC */

    __IO uint32_t sdmmc_ctrl;
    __IO uint32_t sdmmc_l3master;

    /* NAND */

    __IO uint32_t nand_bootstrap;
    __IO uint32_t nand_l3master;

    /* USB */

    __IO uint32_t usb_l3master;
    uint32_t reserved11[9];

    /* ECC */

    __IO uint32_t ecc_l2;
    __IO uint32_t ecc_ocram;
    __IO uint32_t ecc_usb0;
    __IO uint32_t ecc_usb1;
    __IO uint32_t ecc_emac0;
    __IO uint32_t ecc_emac1;
    __IO uint32_t ecc_dma;
    __IO uint32_t ecc_can0;
    __IO uint32_t ecc_can1;
    __IO uint32_t ecc_nand;
    __IO uint32_t ecc_qspi;
    __IO uint32_t ecc_sdmmc;
    uint32_t reserved12[164];

    /* EMACIO */
    __IO uint32_t EMACIO0;
    __IO uint32_t EMACIO1;
    __IO uint32_t EMACIO2;
    __IO uint32_t EMACIO3;
    __IO uint32_t EMACIO4;
    __IO uint32_t EMACIO5;
    __IO uint32_t EMACIO6;
    __IO uint32_t EMACIO7;
    __IO uint32_t EMACIO8;
    __IO uint32_t EMACIO9;
    __IO uint32_t EMACIO10;
    __IO uint32_t EMACIO11;
    __IO uint32_t EMACIO12;
    __IO uint32_t EMACIO13;

    uint32_t reserved13[6];

    /* FLASHIO */
    __IO uint32_t FLASHIO0;
    __IO uint32_t FLASHIO1;
    __IO uint32_t FLASHIO2;
    __IO uint32_t FLASHIO3;
    __IO uint32_t FLASHIO4;
    __IO uint32_t FLASHIO5;
    __IO uint32_t FLASHIO6;
    __IO uint32_t FLASHIO7;
    __IO uint32_t FLASHIO8;
    __IO uint32_t FLASHIO9;
    __IO uint32_t FLASHIO10;
    __IO uint32_t FLASHIO11;

    /* GENERALIO */
    __IO uint32_t GENERALIO0;
    __IO uint32_t GENERALIO1;
    __IO uint32_t GENERALIO2;
    __IO uint32_t GENERALIO3;
    __IO uint32_t GENERALIO4;
    __IO uint32_t GENERALIO5;
    __IO uint32_t GENERALIO6;
    __IO uint32_t GENERALIO7;
    __IO uint32_t GENERALIO8;
    __IO uint32_t GENERALIO9;
    __IO uint32_t GENERALIO10;
    __IO uint32_t GENERALIO11;
    __IO uint32_t GENERALIO12;
    __IO uint32_t GENERALIO13;
    __IO uint32_t GENERALIO14;
    __IO uint32_t GENERALIO15;
    __IO uint32_t GENERALIO16;
    __IO uint32_t GENERALIO17;
    __IO uint32_t GENERALIO18;

    uint32_t reserved14[13];

    /* MIXED1IO  */
    __IO uint32_t MIXED1IO0;
    __IO uint32_t MIXED1IO1;
    __IO uint32_t MIXED1IO2;
    __IO uint32_t MIXED1IO3;
    __IO uint32_t MIXED1IO4;
    __IO uint32_t MIXED1IO5;
    __IO uint32_t MIXED1IO6;
    __IO uint32_t MIXED1IO7;
    __IO uint32_t MIXED1IO8;
    __IO uint32_t MIXED1IO9;
    __IO uint32_t MIXED1IO10;
    __IO uint32_t MIXED1IO11;
    __IO uint32_t MIXED1IO12;
    __IO uint32_t MIXED1IO13;
    __IO uint32_t MIXED1IO14;
    __IO uint32_t MIXED1IO15;
    __IO uint32_t MIXED1IO16;
    __IO uint32_t MIXED1IO17;
    __IO uint32_t MIXED1IO18;
    __IO uint32_t MIXED1IO19;
    __IO uint32_t MIXED1IO20;
    __IO uint32_t MIXED1IO21;

    uint32_t reserved15[8];

    /* GPLINMUXH */
    __IO uint32_t GPLINMUX48;
    __IO uint32_t GPLINMUX49;
    __IO uint32_t GPLINMUX50;
    __IO uint32_t GPLINMUX51;
    __IO uint32_t GPLINMUX52;
    __IO uint32_t GPLINMUX53;
    __IO uint32_t GPLINMUX54;
    __IO uint32_t GPLINMUX55;
    __IO uint32_t GPLINMUX56;
    __IO uint32_t GPLINMUX57;
    __IO uint32_t GPLINMUX58;
    __IO uint32_t GPLINMUX59;
    __IO uint32_t GPLINMUX60;
    __IO uint32_t GPLINMUX61;
    __IO uint32_t GPLINMUX62;
    __IO uint32_t GPLINMUX63;
    __IO uint32_t GPLINMUX64;
    __IO uint32_t GPLINMUX65;
    __IO uint32_t GPLINMUX66;
    __IO uint32_t GPLINMUX67;
    __IO uint32_t GPLINMUX68;
    __IO uint32_t GPLINMUX69;
    __IO uint32_t GPLINMUX70;

    /* GPLMUXL */
    __IO uint32_t GPLMUX0;
    __IO uint32_t GPLMUX1;
    __IO uint32_t GPLMUX2;
    __IO uint32_t GPLMUX3;
    __IO uint32_t GPLMUX4;
    __IO uint32_t GPLMUX5;
    __IO uint32_t GPLMUX6;
    __IO uint32_t GPLMUX7;
    __IO uint32_t GPLMUX8;
    __IO uint32_t GPLMUX9;
    __IO uint32_t GPLMUX10;
    __IO uint32_t GPLMUX11;
    __IO uint32_t GPLMUX12;
    __IO uint32_t GPLMUX13;
    __IO uint32_t GPLMUX14;
    __IO uint32_t GPLMUX15;
    __IO uint32_t GPLMUX16;
    __IO uint32_t GPLMUX17;
    __IO uint32_t GPLMUX18;
    __IO uint32_t GPLMUX19;
    __IO uint32_t GPLMUX20;
    __IO uint32_t GPLMUX21;
    __IO uint32_t GPLMUX22;
    __IO uint32_t GPLMUX23;
    __IO uint32_t GPLMUX24;
    __IO uint32_t GPLMUX25;
    __IO uint32_t GPLMUX26;
    __IO uint32_t GPLMUX27;
    __IO uint32_t GPLMUX28;
    __IO uint32_t GPLMUX29;
    __IO uint32_t GPLMUX30;
    __IO uint32_t GPLMUX31;
    __IO uint32_t GPLMUX32;
    __IO uint32_t GPLMUX33;
    __IO uint32_t GPLMUX34;
    __IO uint32_t GPLMUX35;
    __IO uint32_t GPLMUX36;
    __IO uint32_t GPLMUX37;
    __IO uint32_t GPLMUX38;
    __IO uint32_t GPLMUX39;
    __IO uint32_t GPLMUX40;
    __IO uint32_t GPLMUX41;
    __IO uint32_t GPLMUX42;
    __IO uint32_t GPLMUX43;
    __IO uint32_t GPLMUX44;
    __IO uint32_t GPLMUX45;
    __IO uint32_t GPLMUX46;
    __IO uint32_t GPLMUX47;
    __IO uint32_t GPLMUX48;
    __IO uint32_t GPLMUX49;
    __IO uint32_t GPLMUX50;
    __IO uint32_t GPLMUX51;
    __IO uint32_t GPLMUX52;
    __IO uint32_t GPLMUX53;
    __IO uint32_t GPLMUX54;
    __IO uint32_t GPLMUX55;
    __IO uint32_t GPLMUX56;
    __IO uint32_t GPLMUX57;
    __IO uint32_t GPLMUX58;
    __IO uint32_t GPLMUX59;
    __IO uint32_t GPLMUX60;
    __IO uint32_t GPLMUX61;
    __IO uint32_t GPLMUX62;
    __IO uint32_t GPLMUX63;
    __IO uint32_t GPLMUX64;
    __IO uint32_t GPLMUX65;
    __IO uint32_t GPLMUX66;
    __IO uint32_t GPLMUX67;
    __IO uint32_t GPLMUX68;
    __IO uint32_t GPLMUX69;
    __IO uint32_t GPLMUX70;

    uint32_t reserved16;

    /*HPS UART MUX*/
    __IO uint32_t UART0USEFPGA;
    __IO uint32_t RGMII1USEFPGA;
    __IO uint32_t SPIS0USEFPGA;

    __IO uint32_t CAN0USEFPGA;
    __IO uint32_t I2C0USEFPGA;
    __IO uint32_t SDMMC0USEFPGA;
    __IO uint32_t QSPI0USEFPGA;

    __IO uint32_t SPIS1USEFPGA;
    __IO uint32_t RGMII0USEFPGA;

    uint32_t reserved17;

    __IO uint32_t UART1USEFPGA;

    __IO uint32_t CAN1USEFPGA;

    __IO uint32_t I2C3USEFPGA;
    __IO uint32_t I2C2USEFPGA;
    __IO uint32_t I2C1USEFPGA;

    __IO uint32_t SPIM1USEFPGA;

    uint32_t reserved18;

    __IO uint32_t SPIM0USEFPGA;

} SYSMGR_TypeDef;

/* Defines */
#define SYSMGR ((SYSMGR_TypeDef*) SYSMGR_BASE)

#define UART0_TX_MUXEN 		(0x01U << 0);
#define UART0_RX_MUXEN 		(0x01U << 0);

#define I2C1_SCL_MUXEN 		(0x01U << 0);
#define I2C1_SDA_MUXEN 		(0x01U << 0);

#define I2C0_SCL_MUXEN 		(0x01U << 0);
#define I2C0_SDA_MUXEN 		(0x01U << 0);

#define SDMMC_CMD_MUXEN 	(0x03U << 0);
#define SDMMC_D0_MUXEN 		(0x03U << 0);
#define SDMMC_D1_MUXEN 		(0x03U << 0);
#define SDMMC_D2_MUXEN 		(0x03U << 0);
#define SDMMC_D3_MUXEN 		(0x03U << 0);
#define SDMMC_CLK_MUXEN 	(0x03U << 0);

/* Funtions */
void __SYSMGR_MUX_UART0EN(SYSMGR_TypeDef* sysmgr);
void __SYSMGR_MUX_I2C0EN(SYSMGR_TypeDef* sysmgr);
void __SYSMGR_MUX_I2C1EN(SYSMGR_TypeDef* sysmgr);
void __SYSMGR_MUX_I2C2EN(SYSMGR_TypeDef* sysmgr);
void __SYSMGR_MUX_SDMMCEN(SYSMGR_TypeDef* sysmgr);

#endif /* INC_SYSMGR_H_ */
