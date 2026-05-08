/*
 * fir_audio.h
 *
 *  Created on: 15-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 *      		Rachita Madavan
 */

#ifndef DSP_INC_FIR_AUDIO_H_
#define DSP_INC_FIR_AUDIO_H_

#include "main.h"

/* Defines */
#define TAPS 64

/* Funtions */
void fir_audio_neon_filter(float* input, float* output, uint32_t length);
void audio_process(int16_t *audio, uint64_t count);

#endif /* DSP_INC_FIR_AUDIO_H_ */
