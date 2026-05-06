/*
 * it.c
 *
 *  Created on: 03-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "it.h"


uint8_t byte = 0;

/* IRQ Handler for UART0 */
void UART0_IRQHandler(){
    byte = UART0->srbr;
    dsp_uart_protocol_rx_byte(byte);
//    printf("byte -> %d", byte);
}

/* IRQ Handler for UART1 */
void UART1_IRQHandler(){

}

/* IRQ Handler for SDMMC */
void SDMMC_IRQHandler(){

}
