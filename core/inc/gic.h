/*
 * gic.h
 *
 *  Created on: 02-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_GIC_H_
#define INC_GIC_H_

#include "main.h"

/* Typedef */
typedef struct{

	__IO uint32_t ICCICR; // CPU Interface Control Register
	__IO uint32_t ICCPMR; // Interrupt Priority Mask Register
	__IO uint32_t ICCBPR; // Binary Point Register
	__I  uint32_t ICCIAR; // Interrupt Acknowledge Register
	__O  uint32_t ICCEOIR; // End Of Interrupt Register
	__I  uint32_t ICCRPR; //  Running Priority Register
	__I  uint32_t ICCHPIR; //  Highest Pending Interrupt Register
	__IO uint32_t ICCABPR; //  Aliased Non-secure Binary Point Register
	uint32_t RESERVED0[238];
	__I  uint32_t ICCIDR; //  CPU Interface Implementer Identification Register

}GIC_TypeDef;


/* Interrupt Distributor register */

typedef struct{

    __IO uint32_t ICDDCR;            // Distributor Control Register
    __I  uint32_t ICDICTR;           // Interrupt Controller Type Register
    __I  uint32_t ICDIIDR;           // Distributor Implementer ID Register

         uint32_t RESERVED0[29];

    __IO uint32_t ICDISR[8];         // Interrupt Security Registers

         uint32_t RESERVED1[24];

    __IO uint32_t ICDISER[32];       // Interrupt Set-Enable Registers
    __IO uint32_t ICDICER[32];       // Interrupt Clear-Enable Registers

    __IO uint32_t ICDISPR[32];       // Interrupt Set-Pending Registers
    __IO uint32_t ICDICPR[32];       // Interrupt Clear-Pending Registers

    __IO uint32_t ICDABR[32];        // Active Bit Registers

         uint32_t RESERVED2[32];

    __IO uint32_t ICDIPR[255];       // Interrupt Priority Registers

         uint32_t RESERVED3;

    __IO uint32_t ICDIPTR[255];      // Interrupt Processor Targets Registers

         uint32_t RESERVED4;

    __IO uint32_t ICDICFR[64];       // Interrupt Configuration Registers

    __I  uint32_t ICPPHISR;          // PPI Status Register
    __I  uint32_t ICPPSR[7];        // SPI Status Registers

         uint32_t RESERVED5[120];

    __O  uint32_t ICDSGIR;           // Software Generated Interrupt Register

         uint32_t RESERVED6[51];

    __I  uint32_t ICPIDR0;           // Peripheral ID0
    __I  uint32_t ICPIDR1;           // Peripheral ID1
    __I  uint32_t ICPIDR2;           // Peripheral ID2
    __I  uint32_t ICPIDR3;           // Peripheral ID3
    __I  uint32_t ICPIDR4;           // Peripheral ID4
    __I  uint32_t ICPIDR5;           // Peripheral ID5
    __I  uint32_t ICPIDR6;           // Peripheral ID6
    __I  uint32_t ICPIDR7;           // Peripheral ID7
    __I  uint32_t ICCIDR0;           // Component ID0
    __I  uint32_t ICCIDR1;           // Component ID1
    __I  uint32_t ICCIDR2;           // Component ID2
    __I  uint32_t ICCIDR3;           // Component ID3

}IRQDIS_TypeDef; /* Access this register in Secure State */

/* Defines */
#define GIC		((GIC_TypeDef*) GIC_BASE)
#define IRQDIS	((IRQDIS_TypeDef*) IRQDIS_BASE)

/* Funtions */
Status_TypeDef GIC_Init(GIC_TypeDef* gic, IRQDIS_TypeDef* irqdis);

#endif /* INC_GIC_H_ */
