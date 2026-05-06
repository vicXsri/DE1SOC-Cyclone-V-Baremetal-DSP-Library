/*
 * uart.c
 *
 *  Created on: 01-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "uart.h"
#include "../Drivers/HPS_IRQ/HPS_IRQ_IDs.h"

/* Intialize UART Pheripheral */
Status_TypeDef UART_Init(UART_HandleTypeDef* uart){

	if(uart == NULL) return ARM_ERROR;

	/* Clock and GPIO Pins Intialization */
	UART_MspInit(uart);

	uart->Instance->srr |= (0x01U << 2) | (0x01U << 1) | (0x01U << 0);

	/* Read the Divisor Value */
	uint16_t divisor = UART_Compute_Divisor(UART_FREQ, uart->Init.BaudRate); // 100 MHz

	/* Enable reading and writing of the Divisor Latch register */
	uart->Instance->lcr |= (0x01UL << 7);

	/* Divisors LSB */
	uart->Instance->dll = (uint8_t)(divisor & 0xFF);

	/* Divisors MSB */
	uart->Instance->dlh = (uint8_t)((divisor >> 8) & 0xFF);

	/* Disable reading and writing of the Divisor Latch register */
	uart->Instance->lcr &= ~(0x01UL << 7U);

	/*Configure Data | Length Stop Bits | Return Status*/
	uart->Instance->lcr |= uart->Init.DataLength | uart->Init.StopBits  | uart->Init.parity;

	/*Return Status*/
	return ARM_OK;
}

/* Activate Interrupt UART Pheripheral */
void UART_ActivateNotification(UART_HandleTypeDef* uart){

	/*Enable Interrupt*/
	uart->Instance->dlh = (0x01U << 0U);

	/*Intizalize in the GIC*/
	if(uart->Instance == UART0)
		IRQ_Init(IRQ_UART0, UART0_IRQHandler);
	else
		IRQ_Init(IRQ_UART1, UART1_IRQHandler);
}

/* Compute the Divisor for setting up the baudrate of UART */
uint16_t UART_Compute_Divisor(uint32_t pclk, uint32_t baudrate){
	return (pclk / (16 * baudrate));
}

/* function to print the data through uart */
void hprintf(UART_HandleTypeDef *uart, const char *s)
{
    while (*s)	debug(uart, *s++);
}

/* function to print a single charater in the uart */
void debug(UART_HandleTypeDef* uart, uint8_t ch){
	while(!(uart->Instance->lsr & (0x01U << 5))){}
		uart->Instance->sthr = ch;
}

/* function to recieve a single byte data from uart*/
void UART_Recieve(UART_HandleTypeDef* uart, uint8_t *ch){
	while(!(uart->Instance->lsr & (0x01U << 0))){}
		*ch = uart->Instance->srbr;
}

//void UART_IRQHandler(UART_HandleTypeDef* uart){
//
//	if((uart->Instance->IIR_FCR.IIR & 0x03U) == UART_IRQ_RxData){
//		byte = UART0->srbr;
//	}
//
//}
