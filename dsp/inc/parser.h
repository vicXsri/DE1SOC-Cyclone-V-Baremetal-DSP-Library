/*
 * parser.h
 *
 *  Created on: 17-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef DSP_INC_PARSER_H_
#define DSP_INC_PARSER_H_

#include "main.h"

/* Defines */
#define SAMPLES_PER_ARRAY 260000

#define SDR_FFT_NUM_STAGES 13U
#define SDR_FFT_NUM_SAMPLES (1UL << SDR_FFT_NUM_STAGES)	  	  //Total SDR FFT samples
#define SDR_FIR_NUM_SAMPLES 10000  	  		  				  //Total SDR FIR samples
#define AUDIO_FIR_NUM_SAMPLES 260000  		  				  //Total AUDIO FIR samples

/* Funtions */
void SDR_FIR_WAV_IQ_Parser(void);
void SDR_FFT_WAV_IQ_Parser(void);
void AUDIO_FIR_WAV_IQ_Parser(void);
void MEDIAN_IMAG_BMP_IQ_Parser(void);

static FRESULT WAV_WriteHeader(FIL* file, uint32_t numSamples);

static void write_le16(uint8_t* p, uint16_t v);
static void write_le32(uint8_t* p, uint32_t v);
static uint16_t bmp_read_u16(const uint8_t *p);
static uint32_t bmp_read_u32(const uint8_t *p);

#endif /* DSP_INC_PARSER_H_ */
