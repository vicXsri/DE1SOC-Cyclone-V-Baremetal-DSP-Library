/*
 * wm8731.h
 *
 *  Created on: 24-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef EXTSENSORS_INC_WM8731_H_
#define EXTSENSORS_INC_WM8731_H_

#include "main.h"

/* Defines */
#define CODEC_ID			0x1A

#define	CODEC_LFINCTRL		0x00
#define	CODEC_RTINCTRL		0x01
#define	CODEC_LTOUTCTRL		0x02
#define	CODEC_RTOUTCTRL		0x03
#define	CODEC_ANPACTRL		0x04
#define	CODEC_DGPACTRL		0x05
#define	CODEC_PWRCTRL		0x06
#define	CODEC_DATAFMCTRL	0x07
#define	CODEC_SMPLMCTRL		0x08
#define	CODEC_ACTICTRL		0x09

/* Funtions */
uint16_t reverse16Data(uint16_t data);

#endif /* EXTSENSORS_INC_WM8731_H_ */
