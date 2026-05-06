/*
 * irq.h
 *
 *  Created on: 03-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

/*Interrupts :
 * https://docs.altera.com/r/docs/683126/21.2/cyclone-v-hard-processor-system-technical-reference-manual/gic-interrupt-map-for-the-cyclone-v-soc-hps
 */

#ifndef INC_IRQ_H_
#define INC_IRQ_H_

#include "main.h"
#include "../Drivers/Util/irq_attr.h"

/* Defines */
///* ARM A9 MPCORE devices (0-15 are software, 16-31 are unused except for these three) */
//#define IRQ_MPCORE_GLOBAL_TIMER          27
//#define IRQ_MPCORE_PRIVATE_TIMER         29
//#define IRQ_MPCORE_WATCHDOG              30
//
///* ARM A9 CPU Interrupts (40 total) */
//#define IRQ_CPU0_PARITYFAIL              32
//#define IRQ_CPU0_PARITYFAIL_BTAC         33
//#define IRQ_CPU0_PARITYFAIL_GHB          34
//#define IRQ_CPU0_PARITYFAIL_I_TAG        35
//#define IRQ_CPU0_PARITYFAIL_I_DATA       36
//#define IRQ_CPU0_PARITYFAIL_TLB          37
//#define IRQ_CPU0_PARITYFAIL_D_OUTER      38
//#define IRQ_CPU0_PARITYFAIL_D_TAG        39
//#define IRQ_CPU0_PARITYFAIL_D_DATA       40
//#define IRQ_CPU0_DEFLAGS0                41
//#define IRQ_CPU0_DEFLAGS1                42
//#define IRQ_CPU0_DEFLAGS2                43
//#define IRQ_CPU0_DEFLAGS3                44
//#define IRQ_CPU0_DEFLAGS4                45
//#define IRQ_CPU0_DEFLAGS5                46
//#define IRQ_CPU0_DEFLAGS6                47
//
//#define IRQ_CPU1_PARITYFAIL              48
//#define IRQ_CPU1_PARITYFAIL_BTAC         49
//#define IRQ_CPU1_PARITYFAIL_GHB          50
//#define IRQ_CPU1_PARITYFAIL_I_TAG        51
//#define IRQ_CPU1_PARITYFAIL_I_DATA       52
//#define IRQ_CPU1_PARITYFAIL_TLB          53
//#define IRQ_CPU1_PARITYFAIL_D_OUTER      54
//#define IRQ_CPU1_PARITYFAIL_D_TAG        55
//#define IRQ_CPU1_PARITYFAIL_D_DATA       56
//#define IRQ_CPU1_DEFLAGS0                57
//#define IRQ_CPU1_DEFLAGS1                58
//#define IRQ_CPU1_DEFLAGS2                59
//#define IRQ_CPU1_DEFLAGS3                60
//#define IRQ_CPU1_DEFLAGS4                61
//#define IRQ_CPU1_DEFLAGS5                62
//#define IRQ_CPU1_DEFLAGS6                63
//
//#define IRQ_SCU_PARITYFAIL0              64
//#define IRQ_SCU_PARITYFAIL1              65
//#define IRQ_SCU_EV_ABORT                 66
//#define IRQ_L2_ECC_BYTE_WR               67
//#define IRQ_L2_ECC_CORRECTED             68
//#define IRQ_L2_ECC_UNCORRECTED           69
//#define IRQ_L2_COMBINED                  70
//#define IRQ_DDR_ECC_ERROR                71
//
///* FPGA interrupts (64 total) */
//#define IRQ_FPGA0                        72
//#define IRQ_FPGA1                        73
//#define IRQ_FPGA2                        74
//#define IRQ_FPGA3                        75
//#define IRQ_FPGA4                        76
//#define IRQ_FPGA5                        77
//#define IRQ_FPGA6                        78
//#define IRQ_FPGA7                        79
//#define IRQ_FPGA8                        80
//#define IRQ_FPGA9                        81
//#define IRQ_FPGA10                       82
//#define IRQ_FPGA11                       83
//#define IRQ_FPGA12                       84
//#define IRQ_FPGA13                       85
//#define IRQ_FPGA14                       86
//#define IRQ_FPGA15                       87
//#define IRQ_FPGA16                       88
//#define IRQ_FPGA17                       89
//#define IRQ_FPGA18                       90
//#define IRQ_FPGA19                       91
//#define IRQ_FPGA20                       92
//#define IRQ_FPGA21                       93
//#define IRQ_FPGA22                       94
//#define IRQ_FPGA23                       95
//#define IRQ_FPGA24                       96
//#define IRQ_FPGA25                       97
//#define IRQ_FPGA26                       98
//#define IRQ_FPGA27                       99
//#define IRQ_FPGA28                       100
//#define IRQ_FPGA29                       101
//#define IRQ_FPGA30                       102
//#define IRQ_FPGA31                       103
//#define IRQ_FPGA32                       104
//#define IRQ_FPGA33                       105
//#define IRQ_FPGA34                       106
//#define IRQ_FPGA35                       107
//#define IRQ_FPGA36                       108
//#define IRQ_FPGA37                       109
//#define IRQ_FPGA38                       110
//#define IRQ_FPGA39                       111
//#define IRQ_FPGA40                       112
//#define IRQ_FPGA41                       113
//#define IRQ_FPGA42                       114
//#define IRQ_FPGA43                       115
//#define IRQ_FPGA44                       116
//#define IRQ_FPGA45                       117
//#define IRQ_FPGA46                       118
//#define IRQ_FPGA47                       119
//#define IRQ_FPGA48                       120
//#define IRQ_FPGA49                       121
//#define IRQ_FPGA50                       122
//#define IRQ_FPGA51                       123
//#define IRQ_FPGA52                       124
//#define IRQ_FPGA53                       125
//#define IRQ_FPGA54                       126
//#define IRQ_FPGA55                       127
//#define IRQ_FPGA56                       128
//#define IRQ_FPGA57                       129
//#define IRQ_FPGA58                       130
//#define IRQ_FPGA59                       131
//#define IRQ_FPGA60                       132
//#define IRQ_FPGA61                       133
//#define IRQ_FPGA62                       134
//#define IRQ_FPGA63                       135
//
///* HPS device interrupts (76 total) */
//#define IRQ_DMA0                         136
//#define IRQ_DMA1                         137
//#define IRQ_DMA2                         138
//#define IRQ_DMA3                         139
//#define IRQ_DMA4                         140
//#define IRQ_DMA5                         141
//#define IRQ_DMA6                         142
//#define IRQ_DMA7                         143
//#define IRQ_DMA_ABORT                    144
//#define IRQ_DMA_ECC_CORRECTED            145
//#define IRQ_DMA_ECC_UNCORRECTED          146
//
//#define IRQ_EMAC0                        147
//#define IRQ_EMAC0_TX_ECC_CORRECTED       148
//#define IRQ_EMAC0_TX_ECC_UNCORRECTED     149
//#define IRQ_EMAC0_RX_ECC_CORRECTED       150
//#define IRQ_EMAC0_RX_ECC_UNCORRECTED     151
//
//#define IRQ_EMAC1                        152
//#define IRQ_EMAC1_TX_ECC_CORRECTED       153
//#define IRQ_EMAC1_TX_ECC_UNCORRECTED     154
//#define IRQ_EMAC1_RX_ECC_CORRECTED       155
//#define IRQ_EMAC1_RX_ECC_UNCORRECTED     156
//
//#define IRQ_USB0                         157
//#define IRQ_USB0_ECC_CORRECTED           158
//#define IRQ_USB0_ECC_UNCORRECTED         159
//#define IRQ_USB1                         160
//#define IRQ_USB1_ECC_CORRECTED           161
//#define IRQ_USB1_ECC_UNCORRECTED         162
//
//#define IRQ_CAN0_STS                     163
//#define IRQ_CAN0_MO                      164
//#define IRQ_CAN0_ECC_CORRECTED           165
//#define IRQ_CAN0_ECC_UNCORRECTED         166
//
//#define IRQ_CAN1_STS                     167
//#define IRQ_CAN1_MO                      168
//#define IRQ_CAN1_ECC_CORRECTED           169
//#define IRQ_CAN1_ECC_UNCORRECTED         170
//
//#define IRQ_SDMMC                        171
//#define IRQ_SDMMC_PORTA_ECC_CORRECTED    172
//#define IRQ_SDMMC_PORTA_ECC_UNCORRECTED  173
//#define IRQ_SDMMC_PORTB_ECC_CORRECTED    174
//#define IRQ_SDMMC_PORTB_ECC_UNCORRECTED  175
//
//#define IRQ_NAND                         176
//#define IRQ_NANDR_ECC_CORRECTED          177
//#define IRQ_NANDR_ECC_UNCORRECTED        178
//#define IRQ_NANDW_ECC_CORRECTED          179
//#define IRQ_NANDW_ECC_UNCORRECTED        180
//#define IRQ_NANDE_ECC_CORRECTED          181
//#define IRQ_NANDE_ECC_UNCORRECTED        182
//
//#define IRQ_QSPI                         183
//#define IRQ_QSPI_ECC_CORRECTED           184
//#define IRQ_QSPI_ECC_UNCORRECTED         185
//
//#define IRQ_SPI0                         186
//#define IRQ_SPI1                         187
//#define IRQ_SPI2                         188
//#define IRQ_SPI3                         189
//
//#define IRQ_I2C0                         190
//#define IRQ_I2C1                         191
//#define IRQ_I2C2                         192
//#define IRQ_I2C3                         193
//
//#define IRQ_UART0                        194
//#define IRQ_UART1                        195
//
//#define IRQ_GPIO0                        196
//#define IRQ_GPIO1                        197
//#define IRQ_GPIO2                        198
//
//#define IRQ_TIMER_L4SP_0                 199
//#define IRQ_TIMER_L4SP_1                 200
//#define IRQ_TIMER_OSC1_0                 201
//#define IRQ_TIMER_OSC1_1                 202
//
//#define IRQ_WDOG0                        203
//#define IRQ_WDOG1                        204
//
//#define IRQ_CLKMGR                       205
//#define IRQ_MPUWAKEUP                    206
//#define IRQ_FPGA_MAN                     207
//#define IRQ_NCTIIRQ_0                    208
//#define IRQ_NCTIIRQ_1                    209
//#define IRQ_RAM_ECC_CORRECTED            210
//#define IRQ_RAM_ECC_UNCORRECTED          211

/* Typedefs */
typedef void (*IRQ_Handler)();

#define __current_aspsr()												\
	__extension__({														\
	  register uint32_t current_spsr;									\
	  __asm__ __volatile__("mrs %0, spsr" : "=r"(current_spsr) : :); 	\
	  current_spsr;														\
	})

#define __current_acpsr()												\
	__extension__({														\
	  register uint32_t current_cpsr;									\
	  __asm__ __volatile__("mrs %0, cpsr" : "=r"(current_cpsr) : :); 	\
	  current_cpsr;														\
	})

static __inline__ void  __attribute__((__always_inline__, __nodebug__))
__aset_spsr(uint32_t spsr){
	__asm__ __volatile__("msr spsr_cxsf, %[spsr]\n" :: [spsr] "r" (spsr));
}


#define __AGET_PROC_SPSR()  __current_aspsr()
#define __AGET_PROC_CPSR()  __current_acpsr()
#define __ASET_PROC_SPSR(val)  __aset_spsr(val)


/* Funtions */
void IRQ_Init(uint16_t id, IRQ_Handler irq);

#endif /* INC_IRQ_H_ */
