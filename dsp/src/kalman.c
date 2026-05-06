/*
 * kalman_sensor.c
 *
 *  Created on: 22-Apr-2026
 *      Author: Rachitha Madhavan
 */

#include "kalman.h"

extern I2C_HandleTypeDef 	hi2c0;

KalmanFilterNeon kfxyz;

/* Kalman Filter Sensor Init */
void KalmanFilter_Init(void)
{
    Kalman_Init(&kfxyz, 0.01f, 0.01f, 0.01f,
            2.0f,  2.0f,  2.0f,
            0.0f,  0.0f,  0.0f);
	LEDIP_WritePin(LEDIP, GPIO_PIN_3, HIGH);
}

/* Kalman Filter Init */
void Kalman_Init(KalmanFilterNeon *kf,
				 float qx, float qy, float qz,
                 float rx, float ry, float rz,
				 float x0, float y0, float z0
                 ){
    kf->q = (float32x4_t){ qx, qy, qz, 0.0f };
    kf->r = (float32x4_t){ rx, ry, rz, 0.0f };
    kf->x = (float32x4_t){ x0, y0, z0, 0.0f };
    kf->p = vdupq_n_f32(1.0f);
    kf->k = vdupq_n_f32(0.0f);
}

/* Kalman Read XYZ */
int Kalman_ReadXYZ(float* data){

	int16_t raw[3];

    float out[4];

    float32x4_t meas;
    float32x4_t result;


    if(!ADXL345IsRxAvailable(&hi2c0)) return 0;

    ADXL345XYZRead(&hi2c0, raw);

    meas = (float32x4_t){(float)raw[0], (float)raw[1], (float)raw[2], 0.0f};

    result = Kalman_Update(&kfxyz, meas);

    vst1q_f32(out, result);

    data[0] = out[0];
    data[1] = out[1];
    data[2] = out[2];

    return 1;

}

/* Kalman Filter main */
float32x4_t Kalman_Update(KalmanFilterNeon *kf, float32x4_t measurement){

	float32x4_t one = vdupq_n_f32(1.0f);
	float32x4_t den;
	float32x4_t reci;

	kf->p = vaddq_f32(kf->p, kf->q);


	den = vaddq_f32(kf->p, kf->r);

	reci = vrecpeq_f32(den);

	reci = vmulq_f32(vrecpsq_f32(den, reci), reci);
	reci = vmulq_f32(vrecpsq_f32(den, reci), reci);

	kf->k = vmulq_f32(kf->p, reci);

	kf->x = vaddq_f32(kf->x, vmulq_f32(kf->k, vsubq_f32(measurement, kf->x)));

	kf->p = vmulq_f32(vsubq_f32(one, kf->k), kf->p);

    return kf->x;

}
