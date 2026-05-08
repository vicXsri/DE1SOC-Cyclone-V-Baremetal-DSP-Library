/*
 * fir_audio.c
 *
 *  Created on: 15-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 *      		Rachita Madavan
 */

#include "fir_audio.h"

/* ------------------------------------------------
   64-TAP FIR NOISE REDUCTION BANDPASS FILTER
   Fs = 48000 Hz
   Passband = 200 Hz to 7000 Hz
   Removes rumble + hiss
   Keeps natural volume
------------------------------------------------- */

float h[TAPS] =
{
-0.0002f,-0.0003f,-0.0005f,-0.0006f,-0.0006f,-0.0003f, 0.0003f, 0.0011f,
 0.0018f, 0.0020f, 0.0013f,-0.0001f,-0.0020f,-0.0039f,-0.0051f,-0.0048f,
-0.0024f, 0.0017f, 0.0069f, 0.0118f, 0.0145f, 0.0133f, 0.0078f,-0.0013f,
-0.0114f,-0.0190f,-0.0210f,-0.0153f,-0.0016f, 0.0182f, 0.0402f, 0.0578f,
 0.0642f, 0.0578f, 0.0402f, 0.0182f,-0.0016f,-0.0153f,-0.0210f,-0.0190f,
-0.0114f,-0.0013f, 0.0078f, 0.0133f, 0.0145f, 0.0118f, 0.0069f, 0.0017f,
-0.0024f,-0.0048f,-0.0051f,-0.0039f,-0.0020f,-0.0001f, 0.0013f, 0.0020f,
 0.0018f, 0.0011f, 0.0003f,-0.0003f,-0.0006f,-0.0006f,-0.0005f,-0.0003f
};

float AUDIO_fir_raw_data[AUDIO_FIR_NUM_SAMPLES];
float AUDIO_fir_filtered_data[AUDIO_FIR_NUM_SAMPLES];

uint32_t audio_fir_totalTime = 0;

/* FIR ADUIO Filter Function */
void fir_audio_neon_filter(float* restrict input, float* restrict output, uint32_t length){

	uint32_t i = 0, k=0;

	for(i = 0; i < TAPS - 1 && i < length; i++){

		float y = 0.0f;

		for(k = 0; k <= i; k++)		y += h[k] * input[i - k];

		output[i] = y;

	}

	for(; i + 16 <= length; i+=16 ){

		float32x4_t acc1 = vdupq_n_f32(0.0f);
		float32x4_t acc2 = vdupq_n_f32(0.0f);
		float32x4_t acc3 = vdupq_n_f32(0.0f);
		float32x4_t acc4 = vdupq_n_f32(0.0f);

		for(k = 0; k + 1 <= TAPS; k+=1){

			float32x4_t inp1 = vld1q_f32(&input[i - k]);
			float32x4_t h1 = vdupq_n_f32(h[k]);
			acc1 = vmlaq_f32(acc1, h1, inp1);

			float32x4_t inp2 = vld1q_f32(&input[(i + 4) - k]);
			float32x4_t h2 = vdupq_n_f32(h[k]);
			acc2 = vmlaq_f32(acc2, h2, inp2);

			float32x4_t inp3 = vld1q_f32(&input[(i + 8) - k]);
			float32x4_t h3 = vdupq_n_f32(h[k]);
			acc3 = vmlaq_f32(acc3, h3, inp3);

			float32x4_t inp4 = vld1q_f32(&input[(i + 12) - k]);
			float32x4_t h4 = vdupq_n_f32(h[k]);
			acc4 = vmlaq_f32(acc4, h4, inp4);

		}
		vst1q_f32(&output[i],      acc1);
		vst1q_f32(&output[i + 4],  acc2);
		vst1q_f32(&output[i + 8],  acc3);
		vst1q_f32(&output[i + 12], acc4);
	}

	for(; i < length; i++){

		float y = 0.0f;

		for(k = 0; k < TAPS; k++)		y += h[k] * input[i - k];

		output[i] = y;
	}
}

/* FIR ADUIO Filter Benchmark Function */
void audio_process(int16_t *audio, uint64_t count)
{

	uint32_t start = 0;
	uint32_t end = 0;

    printf("AUDIO FIR Filter Started\n");

    for (uint32_t i = 0; i < count; i++)		AUDIO_fir_raw_data[i] = audio[i] / 32768.0f;

   printf("Vector Mode\n");

   start = getTick();

   fir_audio_neon_filter(AUDIO_fir_raw_data, AUDIO_fir_filtered_data, count);

   end = getTick();

   audio_fir_totalTime = end - start;

   printf("Processed in Vector mode @ %u ms\n",audio_fir_totalTime);


   printf("Processing complete.  %llu samples filtered.\n\n", count);

}
