/*
 * adxl345.c
 *
 *  Created on: 08-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "adxl345.h"

//extern I2C_HandleTypeDef 	hi2c0;

/* ADXL345 Init */
void ADXL345Init(I2C_HandleTypeDef* i2c){

	/* Set up the Resolution to max and the range to 16g */
	I2C_WriteByte(i2c, DATAFORMAT_REG, ADXL345_FULLRES | ADXL345_16GRANGE);

	/* Set up the Output Data Rate to 200Hz */
	I2C_WriteByte(i2c, BWRATE_REG, ADXL345_200HZ);

	/* Activity Threshold */
	I2C_WriteByte(i2c, THRESHACT_REG, 0x04);

	/* Inactivity Threshold */
	I2C_WriteByte(i2c, THRESHINACT_REG, 0x02);

	/* Time for Inactivity */
	I2C_WriteByte(i2c, TIMEINACT_REG, 0x02);

	/* Enable AC coupling for Thresholds */
	I2C_WriteByte(i2c, ACTINACTCTL_REG, 0xFF);

	/* Enable Interrupts */
	I2C_WriteByte(i2c, INTEN_REG, ADXL345_INTACT | ADXL345_INTINACT);

	/* Stop Measure */
	I2C_WriteByte(i2c, POWERCTL_REG, ADXL345_STANDBY);

	/* Start Measure */
	I2C_WriteByte(i2c, POWERCTL_REG, ADXL345_MEASURMENT);

	/* Fixing the offeset of Z axing*/
	I2C_WriteByte(i2c, OFSZ_REG, -1); // -(round(error/4));, error -> 9

	uint8_t pwr = 0;
	I2C_ReadByte(i2c, POWERCTL_REG, &pwr);

	printf("power data 0x%d\r\n",pwr);

}

/* ADXL345 Get the ID */
void ADXL345GETId(I2C_HandleTypeDef* i2c, uint8_t *id){

	/* Start Measure */
	I2C_ReadByte(i2c, DEVID_REG, id);
}

/* ADXL345 Read data of X, Y, Z axis */
void ADXL345XYZRead(I2C_HandleTypeDef* i2c, int16_t* data16){

	uint8_t data8[6];

	I2C_ReadMultiBytes(i2c, DATAX0_REG, (uint8_t*)&data8, sizeof(data8));

	data16[0] = ((data8[1] << 8) | data8[0]);
	data16[1] = ((data8[3] << 8) | data8[2]);
	data16[2] = ((data8[5] << 8) | data8[4]);

}

/* ADXL345 is data available, works since interrupt is enabled in the adxl345 */
bool ADXL345IsRxAvailable(I2C_HandleTypeDef* i2c){

	uint8_t data=0;

	I2C_ReadByte(i2c, INTSRC_REG, &data);
	if(data & ADXL345_INTSRCACT) return true;

	return false;
}

/* ADXL345 calculate Roll */
float ADXL345CalcRoll(float* rawData){

	float X = rawData[0];
	float Y = rawData[1];
	float Z = rawData[2];

	return atan2(Y, sqrt(X*X + Z*Z)) * 180.0f /PI;
}

/* ADXL345 calculate Pitch */
float ADXL345CalcPitch(float* rawData){

	float X = rawData[0];
	float Y = rawData[1];
	float Z = rawData[2];

	return atan2(-X, sqrt(Y*Y + Z*Z)) * 180.0f / PI;
}
