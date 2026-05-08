/*
 * fir_sdr.h
 *
 *  Created on: 13-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 *      		Sanjukta Aparna Venkatachalam
 */

#ifndef DSP_INC_FIR_SDR_H_
#define DSP_INC_FIR_SDR_H_

#include "main.h"



/* Defines */
#define N_taps        51

/* Typedef */
typedef struct {
    float i;
    float q;
} complexFIR_t;

typedef struct {
    float buf_i[N_taps];
    float buf_q[N_taps];
    int   idx;
} fir_state_t;

/* Funtions */
void sdr_fir_process(float* i_samples, float* q_samples, uint64_t count);
static float neon_fir_process_channel(float input, float *buf, const fir_state_t *state);

#endif /* DSP_INC_FIR_SDR_H_ */
