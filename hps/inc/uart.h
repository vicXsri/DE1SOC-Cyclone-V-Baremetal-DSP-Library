/*
 * uart.h
 *
 *  Created on: 01-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

/* Typedef */
typedef struct{
	__IO uint32_t dll; // Rx Buffer, Tx Holding, and Divisor Latch Low
	__IO uint32_t dlh; // Interrupt Enable and Divisor Latch High

	union {
	    __I uint32_t IIR; // Interrupt Identity Register (when read)
	    __O uint32_t FCR; // FIFO Control (when written)
	} IIR_FCR;

//	__IO uint32_t iirfcr; // Interrupt Identity Register | 	FIFO Control
	__IO uint32_t lcr; // 	Line Control Register
	__IO uint32_t mcr; // 	Modem Control Register
	__I uint32_t lsr; // 	Line Status Register
	__I uint32_t msr; // 	Modem Status Register
	__IO uint32_t scr; // 	Scratchpad Register
	uint32_t reserved0[4];
	__IO uint32_t srbr; // 	Shadow Receive Buffer Register
	__IO uint32_t sthr; // Shadow Transmit Buffer Register
	uint32_t reserved1[14];
	__IO uint32_t far; // FIFO Access Register
	__I uint32_t tfr; // FIFO Access Register
	__O uint32_t rfw; // Receive FIFO Write
	__I uint32_t usr; // UART Status Register
	__I uint32_t tfl; // Transmit FIFO Level
	__I uint32_t rfl; // Transmit FIFO Level
	__O uint32_t srr; // Software Reset Register
	__IO uint32_t srts; // Shadow Request to Send
	__IO uint32_t sbcr; // Shadow Break Control Register
	__IO uint32_t sdmam; // Shadow DMA Mode
	__IO uint32_t sfe; // Shadow FIFO Enable
	__IO uint32_t srt; // Shadow Rx Trigger
	__IO uint32_t stet; // Shadow Tx Empty Trigger
	__IO uint32_t htx; // Halt Tx
	__O uint32_t dmasa; // DMA Software Acknowledge
	uint32_t reserved2[17];
	__I uint32_t cpr; // Component Parameter Register
	__I uint32_t ucv; // Component Version
	__I uint32_t ctr; // Component Type Register
}UART_TypeDef;

typedef struct{
	uint32_t BaudRate;
	uint32_t DataLength;
	uint32_t StopBits;
	uint32_t parity;
	uint32_t Mode;
	uint32_t HWFlowCtl;
	uint32_t OverSampling;
}UART_InitTypeDef;

typedef struct {
	UART_TypeDef		*Instance; /*Chooses Which UART*/
	UART_InitTypeDef	 Init;    /*Init Data*/
}UART_HandleTypeDef;


/* Defines */
#define UART0	((UART_TypeDef*) UART0_BASE)
#define UART1	((UART_TypeDef*) UART1_BASE)

#define UART_DL_5B	  (0x00UL);
#define UART_DL_6B	  (0x01UL << 0U);
#define UART_DL_7B	  (0x02UL << 0U);
#define UART_DL_8B	  (0x03UL << 0U);

#define UART_STOP_BIT1		(0x00UL)
#define UART_STOP_BIT1f5	(0x01UL << 2U)

#define UART_PARITY_NONE    (0x00UL)
#define UART_PARITY_ODD     (1UL << 3)
#define UART_PARITY_EVEN    ((1UL << 3) | (1UL << 4))

#define UART_IRQ_MODSTAT	0x00
#define UART_IRQ_NOIRQ		0x01
#define UART_IRQ_THREMP		0x02
#define UART_IRQ_RxData		0x04
#define UART_IRQ_RXLNSt		0x06
#define UART_IRQ_Chrtim		0x0C

#define UART_FREQ		100000000UL

/* Funtions */
Status_TypeDef UART_Init(UART_HandleTypeDef* uart);
uint16_t UART_Compute_Divisor(uint32_t pclk, uint32_t baudrate);
void debug(UART_HandleTypeDef* uart, uint8_t ch);
void hprintf(UART_HandleTypeDef *uart, const char *s);
void UART_Recieve(UART_HandleTypeDef* uart, uint8_t *ch);
void UART_ActivateNotification(UART_HandleTypeDef* uart);
void UART_MspInit(UART_HandleTypeDef* uart);
void UART_IRQHandler(UART_HandleTypeDef* uart);

#endif /* INC_UART_H_ */
