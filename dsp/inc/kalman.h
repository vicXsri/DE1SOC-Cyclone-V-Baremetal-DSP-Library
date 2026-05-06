/*
 * kalman_sensor.h
 *
 *  Created on: 22-Apr-2026
 *      Author: Rachitha Madhavan
 */

#ifndef DSP_INC_KALMAN_SENSOR_H_
#define DSP_INC_KALMAN_SENSOR_H_

#include "main.h"

/* Typedef */
typedef struct{
	float32x4_t q;
	float32x4_t r;
	float32x4_t x;
	float32x4_t p;
	float32x4_t k;
}KalmanFilterNeon;

/* Funtions */
void KalmanFilter_Init(void);

void Kalman_Init(KalmanFilterNeon *kf,
				 float qx, float qy, float qz,
                 float rx, float ry, float rz,
				 float x0, float y0, float z0
                 );

int Kalman_ReadXYZ(float* data);

float32x4_t Kalman_Update(KalmanFilterNeon *kf, float32x4_t measurement);


#endif /* DSP_INC_KALMAN_SENSOR_H_ */
