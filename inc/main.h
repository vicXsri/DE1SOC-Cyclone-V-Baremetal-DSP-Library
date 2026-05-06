/*
 * main.h
 *
 *  Created on: 31-Mar-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

/*Standard Libraries*/
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#define __I 	volatile const 	/*Read Only Permission*/
#define __O 	volatile 		/*Write Only Permission*/
#define __IO 	volatile 		/*Read-Write Permission*/

#define ENABLE	true
#define DISABLE	false

#define HIGH	ENABLE
#define LOW		!ENABLE

#define PI						3.14159265359f

typedef enum{
	ARM_OK			= 0x00U,
	ARM_ERROR		= 0x01U,
	ARM_BUSY		= 0x02U,
	ARM_TIMEOUT		= 0x03U
}Status_TypeDef;

/*External Libraries*/
#include <arm_neon.h>
#include "../Drivers/FatFS/ff.h"
#include "../Drivers/FatFS/diskio.h"

/*Custom Libraries by Team 19*/

#include "address.h"

#include "gic.h"
#include "irq.h"
#include "cache.h"
#include "privatetimer.h"

#include "sysmgr.h"
#include "gpio.h"
#include "wdt.h"
#include "clock.h"
#include "uart.h"
#include "i2c.h"
#include "sdmmc.h"
#include "rstmgr.h"

#include "adxl345.h"
#include "wm8731.h"
#include "lt24.h"

#include "fpgaip.h"

#include "msp.h"
#include "it.h"

#include "parser.h"
#include "fir_sdr.h"
#include "fir_audio.h"
#include "fft_sdr.h"
#include "tester.h"
#include "dsp_uart_protocol.h"
#include "kalman.h"
#include "median_image.h"
#include "data.h"

#include "audio.h"
#include "image.h"

#endif /* INC_MAIN_H_ */
