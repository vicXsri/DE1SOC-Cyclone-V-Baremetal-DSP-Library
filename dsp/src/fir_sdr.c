/*
 * fir_SDC.c
 *
 *  Created on: 13-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 *      		Sanjukta Aparna Venkatachalam
 */

#include "fir_sdr.h"

float SDR_fir_raw_data[SDR_FIR_NUM_SAMPLES * 2];
float SDR_fir_filtered_data[SDR_FIR_NUM_SAMPLES * 2];

static float SDR_fir_NEON_I_filtered_data[SDR_FIR_NUM_SAMPLES];
static float SDR_fir_NEON_Q_filtered_data[SDR_FIR_NUM_SAMPLES];

static const float coeffs[N_taps] = {
    -0.000340f,  /* h[0]  = h[50] */
    -0.000721f,  /* h[1]  = h[49] */
    -0.000344f,  /* h[2]  = h[48] */
     0.001214f,  /* h[3]  = h[47] */
     0.003563f,  /* h[4]  = h[46] */
     0.004776f,  /* h[5]  = h[45] */
     0.002857f,  /* h[6]  = h[44] */
    -0.003048f,  /* h[7]  = h[43] */
    -0.010314f,  /* h[8]  = h[42] */
    -0.014753f,  /* h[9]  = h[41] */
    -0.010918f,  /* h[10] = h[40] */
     0.003825f,  /* h[11] = h[39] */
     0.026026f,  /* h[12] = h[38] */
     0.047509f,  /* h[13] = h[37] */
     0.059371f,  /* h[14] = h[36] */
     0.053068f,  /* h[15] = h[35] */
     0.026573f,  /* h[16] = h[34] */
    -0.014696f,  /* h[17] = h[33] */
    -0.059072f,  /* h[18] = h[32] */
    -0.093342f,  /* h[19] = h[31] */
    -0.100692f,  /* h[20] = h[30] */
    -0.063837f,  /* h[21] = h[29] */
     0.023819f,  /* h[22] = h[28] */
     0.144020f,  /* h[23] = h[27] */
     0.263480f,  /* h[24] = h[26] */
     0.317461f,  /* h[25]  ← centre tap */
     0.263480f,  /* h[26] = h[24] */
     0.144020f,  /* h[27] = h[23] */
     0.023819f,  /* h[28] = h[22] */
    -0.063837f,  /* h[29] = h[21] */
    -0.100692f,  /* h[30] = h[20] */
    -0.093342f,  /* h[31] = h[19] */
    -0.059072f,  /* h[32] = h[18] */
    -0.014696f,  /* h[33] = h[17] */
     0.026573f,  /* h[34] = h[16] */
     0.053068f,  /* h[35] = h[15] */
     0.059371f,  /* h[36] = h[14] */
     0.047509f,  /* h[37] = h[13] */
     0.026026f,  /* h[38] = h[12] */
     0.003825f,  /* h[39] = h[11] */
    -0.010918f,  /* h[40] = h[10] */
    -0.014753f,  /* h[41] = h[9]  */
    -0.010314f,  /* h[42] = h[8]  */
    -0.003048f,  /* h[43] = h[7]  */
     0.002857f,  /* h[44] = h[6]  */
     0.004776f,  /* h[45] = h[5]  */
     0.003563f,  /* h[46] = h[4]  */
     0.001214f,  /* h[47] = h[3]  */
    -0.000344f,  /* h[48] = h[2]  */
    -0.000721f,  /* h[49] = h[1]  */
    -0.000340f   /* h[50] = h[0]  */
};

uint32_t sdr_fir_totalTime = 0;

/* SDR FIR Filter */
void fir_sdr_neon_filter(float* restrict input, float* restrict output, uint32_t length){
	uint32_t i = 0, k=0;

	for(i = 0; i < N_taps - 1 && i < length; i++){

		float y = 0.0f;

		for(k = 0; k <= i; k++)		y += coeffs[k] * input[i - k];

		output[i] = y;
	}

	for(; i + 16 <= length; i+=16 ){

		float32x4_t acc1 = vdupq_n_f32(0.0f);
		float32x4_t acc2 = vdupq_n_f32(0.0f);
		float32x4_t acc3 = vdupq_n_f32(0.0f);
		float32x4_t acc4 = vdupq_n_f32(0.0f);

		for(k = 0; k + 1 <= N_taps; k+=1){

			float32x4_t inp1 = vld1q_f32(&input[i - k]);
			float32x4_t h1 = vdupq_n_f32(coeffs[k]);
			acc1 = vmlaq_f32(acc1, h1, inp1);

			float32x4_t inp2 = vld1q_f32(&input[(i + 4) - k]);
			float32x4_t h2 = vdupq_n_f32(coeffs[k]);
			acc2 = vmlaq_f32(acc2, h2, inp2);

			float32x4_t inp3 = vld1q_f32(&input[(i + 8) - k]);
			float32x4_t h3 = vdupq_n_f32(coeffs[k]);
			acc3 = vmlaq_f32(acc3, h3, inp3);

			float32x4_t inp4 = vld1q_f32(&input[(i + 12) - k]);
			float32x4_t h4 = vdupq_n_f32(coeffs[k]);
			acc4 = vmlaq_f32(acc4, h4, inp4);

		}

		vst1q_f32(&output[i],      acc1);
		vst1q_f32(&output[i + 4],  acc2);
		vst1q_f32(&output[i + 8],  acc3);
		vst1q_f32(&output[i + 12], acc4);
	}

	for(; i < length; i++){

		float y = 0.0f;

		for(k = 0; k < N_taps; k++)		y += coeffs[k] * input[i - k];

		output[i] = y;
	}
}

/* SDR FIR Process */
void sdr_fir_process(float* i_samples, float* q_samples, uint64_t count){

	uint32_t start = 0;
	uint32_t end = 0;

    printf("SDR FIR Filter Started\n");

    start = getTick();

    fir_sdr_neon_filter(i_samples, SDR_fir_NEON_I_filtered_data, SDR_FIR_NUM_SAMPLES);
    fir_sdr_neon_filter(q_samples, SDR_fir_NEON_Q_filtered_data, SDR_FIR_NUM_SAMPLES);

    end = getTick();

    sdr_fir_totalTime = end - start;

    printf("Processed @ %u ms\n",sdr_fir_totalTime);

    for (int n = 0; n < count; n++) {

    	SDR_fir_raw_data[2 * n] = i_samples[n];
    	SDR_fir_raw_data[2 * n + 1] = q_samples[n];

    	SDR_fir_filtered_data[2 * n] = SDR_fir_NEON_I_filtered_data[n];
    	SDR_fir_filtered_data[2 * n + 1] = SDR_fir_NEON_Q_filtered_data[n];
    }

    printf("Processing complete.  %llu samples filtered.\n\n", count);

}
