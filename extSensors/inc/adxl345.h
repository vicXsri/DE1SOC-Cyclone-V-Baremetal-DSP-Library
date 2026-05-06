/*
 * adxl345.h
 *
 *  Created on: 08-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef SENSOR_INC_ADXL345_H_
#define SENSOR_INC_ADXL345_H_

#include "main.h"
#include "math.h"

/* Defines */
#define DEVICE_ID				0xE5

#define	DEVID_REG				0x00U
#define	THRESHTAP_REG			0x1DU
#define	OFSX_REG				0x1EU
#define	OFSY_REG				0x1FU
#define	OFSZ_REG				0x20U
#define	DUR_REG					0x21U
#define	LATENT_REG				0x22U
#define	WINDOW_REG				0x23U
#define	THRESHACT_REG			0x24U
#define	THRESHINACT_REG			0x25U
#define	TIMEINACT_REG			0x26U
#define	ACTINACTCTL_REG			0x27U
#define	THRESHFF_REG			0x28U
#define	TIMEFF_REG				0x29U
#define	TAPAXES_REG				0x2AU
#define	ACTTAPSTAT_REG			0x2BU
#define	BWRATE_REG				0x2CU
#define	POWERCTL_REG			0x2DU
#define	INTEN_REG				0x2EU
#define	INTMAP_REG				0x2FU
#define	INTSRC_REG				0x30U
#define	DATAFORMAT_REG			0x31U
#define	DATAX0_REG				0x32U
#define	DATAX1_REG				0x33U
#define	DATAY0_REG				0x34U
#define	DATAY1_REG				0x35U
#define	DATAZ0_REG				0x36U
#define	DATAZ1_REG				0x37U
#define	DATAFIFOCTL_REG			0x38U
#define	DATAFIFOSTAT_REG		0x39U


#define  ADXL345_FULLRES		(0x01UL << 3U)

#define  ADXL345_2GRANGE		(0x00UL << 0U)
#define  ADXL345_4GRANGE		(0x01UL << 0U)
#define  ADXL345_8GRANGE		(0x02UL << 0U)
#define  ADXL345_16GRANGE		(0x03UL << 0U)

#define	 ADXL345_400HZ			(0x0CU)
#define	 ADXL345_200HZ			(0x0BU)
#define	 ADXL345_100HZ			(0x0AU)
#define	 ADXL345_50HZ			(0x09U)
#define	 ADXL345_25HZ			(0x08U)
#define	 ADXL345_12HZ			(0x07U)

#define	 ADXL345_STANDBY		(0x00U << 3U)
#define	 ADXL345_MEASURMENT		(0x01U << 3U)

#define	 ADXL345_INTACT			(0x01U << 4U)
#define	 ADXL345_INTINACT		(0x01U << 3U)

#define	 ADXL345_INTSRCACT		(0x01U << 4U)

/* Funtions */
float ADXL345CalcRoll(float* rawData);
float ADXL345CalcPitch(float* rawData);

#endif /* SENSOR_INC_ADXL345_H_ */
