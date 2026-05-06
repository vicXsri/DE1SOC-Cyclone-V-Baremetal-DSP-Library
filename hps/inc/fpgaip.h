/*
 * fpgaip.h
 *
 *  Created on: 31-Mar-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_FPGAIP_H_
#define INC_FPGAIP_H_

#include "main.h"

/* Typedef */
typedef struct{
	__IO uint32_t output;
}LEDIP_TypeDef;

typedef struct{
	__I uint32_t input;
}SWITCHIP_TypeDef;

typedef struct{
	__I uint32_t input;
}BUTTONIP_TypeDef;

typedef struct{
	__O uint8_t output;
}HEXIP_TypeDef;

typedef struct{
	__IO uint32_t control;
	__IO uint32_t fifospace;
	__IO uint32_t leftdata;
	__IO uint32_t rightdata;
}CODECIP_TypeDef;

typedef struct{

	union{
	__IO uint32_t input;
	__IO uint32_t output;
	}opip;

	union{
	__IO uint32_t direction;
	__IO uint32_t splitinput;
	}dirinp;

	__IO uint32_t int_mask;
	__IO uint32_t int_flags;
	__IO uint32_t outset;
	__IO uint32_t outclear;
}GPIOIP_TypeDef;

typedef enum{
	WRITE_READY,
	WRITE_HOLD,
	WRITE_TDONE,
	WRITE_RDONE,
	WRITE_TST,
	WRITE_FULL,
	WRITE_LO,
	WRITE_PLAY
}HexContent_Typedef;

typedef enum{

	BLANK	= 0x00,

	ZR		= 0x7F,
	ONE		= 0x06,
	TWO		= 0x5B,
	THREE	= 0x4F,
	FOUR	= 0x66,
	FIVE	= 0x6D,
	SIX		= 0x7D,
	SEVEN	= 0x07,
	EIGHT	= 0x7F,
	NINE	= 0x6F,

	HY		= 0x40, // ' - '

	A		= 0x77,
	B		= 0x7C,
	C		= 0x39,
	D		= 0x5E,
	E		= 0x7B,
	F		= 0x71,
	H		= 0x76,
	L		= 0x38,
	O		= 0x5C,
	N		= 0x37,
	P		= 0x73,
	R		= 0x31,
	S		= 0x6D,
	T		= 0x78,
	U		= 0x3E,
	Y		= 0x6E,

}HexChar_Typedef;

/* Defines */
#define LEDIP		((LEDIP_TypeDef*) 	 	LEDIP_BASE)
#define SWITCHIP	((SWITCHIP_TypeDef*) 	SWITCHIP_BASE)
#define BUTTONIP	((BUTTONIP_TypeDef*) 	BUTTONIP_BASE)
#define CODECIP		((CODECIP_TypeDef*) 	CODECIP_BASE)
#define GPIO1IP		((GPIOIP_TypeDef*) 		JP1IP_BASE)
#define GPIO2IP		((GPIOIP_TypeDef*) 		JP2IP_BASE)
#define HEX0IP		((HEXIP_TypeDef*) 		HEX3HEX0IP_BASE + 0x00)
#define HEX1IP		((HEXIP_TypeDef*) 		HEX3HEX0IP_BASE + 0x01)
#define HEX2IP		((HEXIP_TypeDef*) 		HEX3HEX0IP_BASE + 0x02)
#define HEX3IP		((HEXIP_TypeDef*) 		HEX3HEX0IP_BASE + 0x03)
#define HEX4IP		((HEXIP_TypeDef*) 		HEX5HEX4IP_BASE + 0x00)
#define HEX5IP		((HEXIP_TypeDef*) 		HEX5HEX4IP_BASE + 0x01)

#define LED_PIN_0					(0x0000)
#define LED_PIN_1					(0x0001)
#define	LED_PIN_2					(0x0002)
#define LED_PIN_3					(0x0003)
#define LED_PIN_4					(0x0004)
#define LED_PIN_5					(0x0005)
#define LED_PIN_6					(0x0006)
#define LED_PIN_7					(0x0007)
#define LED_PIN_8					(0x0008)
#define LED_PIN_9					(0x0009)

#define SW_PIN_0					(0x0000)
#define SW_PIN_1					(0x0001)
#define SW_PIN_2					(0x0002)
#define SW_PIN_3					(0x0003)
#define SW_PIN_4					(0x0004)
#define SW_PIN_5					(0x0005)
#define SW_PIN_6					(0x0006)
#define SW_PIN_7					(0x0007)
#define SW_PIN_8					(0x0008)
#define SW_PIN_9					(0x0009)

#define SDR_FIR_SW					SW_PIN_0
#define SDR_FFT_SW					SW_PIN_1
#define AUDIO_FIR_SW				SW_PIN_2
#define KALMAN_SW					SW_PIN_3
#define I2C_MUX_SW					SW_PIN_4
#define MEDIAN_IMAGE				SW_PIN_5
#define IMAGE_FIL					SW_PIN_6
#define IMAGE_UNFIL					SW_PIN_7
#define PLAY_FIL					SW_PIN_8
#define PLAY_UNFIL					SW_PIN_9

#define KEY_PIN_0					(0x0000)
#define KEY_PIN_1					(0x0001)
#define KEY_PIN_2					(0x0002)
#define KEY_PIN_3					(0x0003)

#define VOL_DEC						KEY_PIN_0
#define VOL_INC						KEY_PIN_1

#define GPIOINDIR					(0x01U << 0x00U)
#define GPIOOPDIR					(0x01U << 0x01U)
#define GPIOBOTHDIR					(GPIOINDIR | GPIOOPDIR)
#define GPIOBIDIR					((0x01U << 0x02U) | GPIOBOTHDIR)

/* Funtions */
void LEDIP_WritePin(LEDIP_TypeDef* ledip, uint8_t led, uint8_t state);
void LEDIP_TogglePin(LEDIP_TypeDef* ledip, uint8_t led);

bool SWITCHIP_ReadPin(SWITCHIP_TypeDef* switchip, uint8_t Sswitch);

bool BUTTONIP_ReadPin(BUTTONIP_TypeDef* buttonip, uint8_t Sbutton);
void HEXIP_WriteData(HEXIP_TypeDef* hexip, uint8_t numberinHex);
void HEXIP_Text(HexContent_Typedef content);
void HEXIP_Reset(HEXIP_TypeDef* hexip);
void HEXIP_ResetALL();
void HEXIP_Blink(HexContent_Typedef content, uint32_t speed);
void HEXIP_ROT(HEXIP_TypeDef* hexip);

void CODECIP_CLEARFIFO(CODECIP_TypeDef* codec, uint8_t adc, uint8_t dac);
void CODECIP_WRITE(CODECIP_TypeDef* codec, uint32_t left, uint32_t right);
void CODECIP_GETFIFOSPACE(CODECIP_TypeDef* codec, uint32_t* fifo);

void GPIOIP_INIT(GPIOIP_TypeDef* gpio, uint32_t piotype,uint32_t dir, uint32_t port);
void GPIOIP_setDirection(GPIOIP_TypeDef* gpio, uint32_t val);
void GPIOIP_setOutput(GPIOIP_TypeDef* gpio, uint32_t val);

void LT24_POWERCONFIG(GPIOIP_TypeDef* gpio, bool state);
void LT24_WRITE(GPIOIP_TypeDef* gpio, bool isData, uint16_t val);
void LT24_DISPLAY(GPIOIP_TypeDef* gpio, uint16_t colr);
void LT24_SETWINDOW(GPIOIP_TypeDef* gpio, uint16_t width, uint16_t height, uint16_t x_left, uint16_t y_top);

#endif /* INC_FPGAIP_H_ */
