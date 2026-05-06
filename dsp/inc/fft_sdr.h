/*
 * fft_sdr.h
 *
 *  Created on: 10-Apr-2026
 *      Author: Aarush Arya
 */

#ifndef DSP_INC_FFT_SDR_H_
#define DSP_INC_FFT_SDR_H_

#include "main.h"

/* Typedef */
typedef struct {
    float32_t real;
    float32_t imag;
} complexFFT_t;

/* Funtions */
void sdr_fft_process(complexFFT_t *buffer, uint32_t N);
void calculate_fft(complexFFT_t *buffer, uint32_t N);
void apply_frequency_filter(complexFFT_t *buffer, uint32_t N);
void weight_lut(float *weight_lut, uint32_t N);
void calculate_ifft(complexFFT_t *buffer, uint32_t N);

#endif /* DSP_INC_FFT_SDR_H_ */
