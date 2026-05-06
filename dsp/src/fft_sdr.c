
/*
 * fft_sdr.c
 *
 *  Created on: 10-Apr-2026
 *      Author: Aarush Arya
 */

#include "fft_sdr.h"

float SDR_fft_raw_data[SDR_FFT_NUM_SAMPLES * 2];
float SDR_fft_filtered_data[SDR_FFT_NUM_SAMPLES * 2];

float sdr_weight_lut[SDR_FFT_NUM_SAMPLES]  = {0};

float sdr_twiddle_cos_lut[SDR_FFT_NUM_STAGES][SDR_FFT_NUM_SAMPLES / 2] = {0};
float sdr_twiddle_sin_lut[SDR_FFT_NUM_STAGES][SDR_FFT_NUM_SAMPLES / 2] = {0};

uint32_t sdr_fft_totalTime = 0;

/* Apply the Freqency Filter */
void apply_frequency_filter(complexFFT_t *buffer, uint32_t N)
{
    uint32_t i = 0;

    float32x4x2_t inp1, inp2;

    float32x4_t w1, w2;

	for (; i + 8 <= N; i += 8 )
    {

        inp1 = vld2q_f32((float *) &buffer[i	 ]);

        inp2 = vld2q_f32((float *) &buffer[i + 4]);

    	w1 	 = vld1q_f32(&sdr_weight_lut[i]);

    	w2 	 = vld1q_f32(&sdr_weight_lut[i + 4]);

    	inp1.val[0] = vmulq_f32(inp1.val[0], w1);
    	inp1.val[1] = vmulq_f32(inp1.val[1], w1);

    	inp2.val[0] = vmulq_f32(inp2.val[0], w2);
    	inp2.val[1] = vmulq_f32(inp2.val[1], w2);

    	vst2q_f32((float *) &buffer[i    ], inp1);
    	vst2q_f32((float *) &buffer[i + 4], inp2);

    }

	for (; i < N; i++)
    {
        buffer[i].real *= sdr_weight_lut[i];
        buffer[i].imag *= sdr_weight_lut[i];
    }
}

/* Apply the FFT */
void calculate_fft(complexFFT_t *buffer, uint32_t N)
{

    uint32_t temp_i, reversed_i, bot_index, num_elements, dist_elements, i = 0, k = 0, j = 0;

    float temp_R = 0, temp_I = 0, temp_R1, temp_I1;

    float32x4_t	qtwir, qtwii,     /* Twiddle */
				qtempr, qtempi;   /* Temp i & r */

    float32x4x2_t qbot, 		  /* Bottom  */
				  qtop, 		  /* Top  */
				  qtemp;		/* Temp */

    for (uint32_t p = 0; p < N; p++)
    {
        temp_i = p;

        reversed_i = 0;

        for (uint32_t it = 0; it < SDR_FFT_NUM_STAGES; it++)
        {
            reversed_i = (reversed_i << 1);

            reversed_i = reversed_i | (temp_i & 1);

            temp_i = temp_i >> 1;
        }

        if ( p < reversed_i )
        {
            temp_R = buffer[p].real;

            buffer[p].real = buffer[reversed_i].real;
            buffer[reversed_i].real = temp_R;

            temp_I = buffer[p].imag;

            buffer[p].imag = buffer[reversed_i].imag;
            buffer[reversed_i].imag = temp_I;
        }
    }

    for (; i < SDR_FFT_NUM_STAGES; i ++ )
    {

        num_elements = 1 << (i + 1);
        dist_elements = num_elements >> 1;

        for (j = 0; j < N  ; j += num_elements  )
        {
            for (k = j; k + 4 <= j + dist_elements ; k += 4  )
            {
                bot_index = k + dist_elements;

                qbot = vld2q_f32((float *) &buffer[bot_index]);

                qtwii = vld1q_f32(&sdr_twiddle_sin_lut[i][k - j]);
                qtwir = vld1q_f32(&sdr_twiddle_cos_lut[i][k - j]);

                qtempr = vsubq_f32(vmulq_f32(qbot.val[0], qtwir), vmulq_f32(qbot.val[1], qtwii));
                qtempi = vaddq_f32(vmulq_f32(qbot.val[0], qtwii), vmulq_f32(qbot.val[1], qtwir));

                qtop = vld2q_f32((float *) &buffer[k]);

                qtemp.val[0] =  vsubq_f32(qtop.val[0], qtempr);
                qtemp.val[1] =  vsubq_f32(qtop.val[1], qtempi);

                vst2q_f32((float *) &buffer[bot_index] ,qtemp);

                qtemp.val[0] =  vaddq_f32(qtop.val[0], qtempr);
                qtemp.val[1] =  vaddq_f32(qtop.val[1], qtempi);

                vst2q_f32((float *) &buffer[k] ,qtemp);

            }
			for (; k < j + dist_elements; k++)
			{
				bot_index = k + dist_elements;

				temp_R1 = (buffer[bot_index].real * sdr_twiddle_cos_lut[i][k - j]) -
						  (buffer[bot_index].imag * sdr_twiddle_sin_lut[i][k - j]);

				temp_I1 = (buffer[bot_index].real * sdr_twiddle_sin_lut[i][k - j]) +
						  (buffer[bot_index].imag * sdr_twiddle_cos_lut[i][k - j]);

				buffer[bot_index].real = buffer[k].real - temp_R1;
				buffer[bot_index].imag = buffer[k].imag - temp_I1;

				buffer[k].real = buffer[k].real + temp_R1;
				buffer[k].imag = buffer[k].imag + temp_I1;
			}
        }
    }
}

/* Apply the IFFT */
void calculate_ifft(complexFFT_t *buffer, uint32_t N){

	uint32_t i = 0;

	float32x4x2_t qbuff, qtemp;
	float32x4_t qscale;

	float scale = 1.0f / N;

	qscale = vdupq_n_f32(scale);

    for (; i + 4 <= N; i += 4) {
    	qbuff =  vld2q_f32((float *) &buffer[i]);
    	qbuff.val[1] = vnegq_f32(qbuff.val[1]);
    	vst2q_f32((float *) &buffer[i] , qbuff);
    }

    for (; i < N; i++)		buffer[i].imag = -buffer[i].imag;

    calculate_fft(buffer, N);


    for (i = 0; i + 4 <= N; i += 4) {

    	qbuff = vld2q_f32((float *) &buffer[i]);

    	qtemp.val[0] = vmulq_f32(qbuff.val[0], qscale);

    	qbuff.val[1] = vnegq_f32(qbuff.val[1]);
    	qtemp.val[1] = vmulq_f32(qbuff.val[1], qscale);

    	vst2q_f32((float *) &buffer[i] , qtemp);

    }

    for (; i < N; i++) {
        buffer[i].real =  buffer[i].real * scale;
        buffer[i].imag = -buffer[i].imag * scale;
    }
}

/* Calculate Twiddle LUT */
void twiddle_lut(float twiddle_lut_cos[][SDR_FFT_NUM_SAMPLES / 2], float twiddle_lut_sin[][SDR_FFT_NUM_SAMPLES / 2]){

	uint32_t num_elements, dist_elements;
	float theta = 0;
	for (uint32_t stage = 0; stage < SDR_FFT_NUM_STAGES; stage++){

		num_elements  = 1U << (stage + 1);
		dist_elements = num_elements >> 1;

		for (uint32_t t = 0; t < dist_elements; t++){

			theta = ((-PI) * (float)t) / (float)dist_elements;

			sdr_twiddle_cos_lut[stage][t] = cosf(theta);
			sdr_twiddle_sin_lut[stage][t] = sinf(theta);
		}
	}
}

/* Calculate Weight LUT */
void weight_lut(float *weight_lut, uint32_t N){

    float sigma = (float) N / 20.0f;
    float scale = -0.5f / (sigma * sigma);
    float dist[16];
    float dist_from_center;

    uint32_t half = N / 2;

    uint32_t i = 0;

	float32x4_t qscale = vdupq_n_f32(scale);

	float32x4_t qddist;
	float32x4_t qdist[4];
	float32x4_t qs[4];

    for (; i +16 <= N; i+=16)
    {
    	for(uint32_t k =0; k < 16; k++)
    		dist[k] = ((i + k) > half) ? (float)(N - (i + k)) : (float)(i + k);

    	for(uint32_t m =0; m < 4; m++){
    		qddist	  = vld1q_f32(&dist[m * 4]);
    		qdist[m]  = vmulq_f32(qddist , qddist);
    		qs[m] 	  = vmulq_f32(qdist[m] , qscale);
    		vst1q_f32(&dist[m * 4], qs[m]);
    	}

    	for(uint32_t j =0; j < 16; j++)		weight_lut[i + j] = expf(dist[j]);

    }

    for(; i< N; i++){

        if (i > half)	dist_from_center = (float) (N - i);
        else			dist_from_center = (float) i;

        weight_lut[i] = expf(scale * dist_from_center * dist_from_center);
    }

}

/* Proccess th SDR FFT */
void sdr_fft_process(complexFFT_t *buffer, uint32_t N)
{

	uint32_t start = 0, end = 0;

	for(uint32_t k = 0; k < N; k++){
		SDR_fft_raw_data[k * 2] 	= buffer[k].real;
		SDR_fft_raw_data[k * 2 + 1] = buffer[k].imag;
	}

    weight_lut(sdr_weight_lut, N);

	twiddle_lut(sdr_twiddle_cos_lut, sdr_twiddle_sin_lut);

    printf("SDR FFT NEON Filter Started\n");

    start = getTick();

    calculate_fft(buffer, N);

    apply_frequency_filter(buffer, N);

    calculate_ifft(buffer, N);

    end = getTick();

    sdr_fft_totalTime = end - start;

    printf("Processed @ %u ms\n",sdr_fft_totalTime);

    for (int i = 0; i < N; i++) {
    	SDR_fft_filtered_data[i * 2]     =	buffer[i].real;
    	SDR_fft_filtered_data[i * 2 + 1] = buffer[i].imag;
    }

    printf("Processing complete.  %d samples filtered.\n\n", N);
}
