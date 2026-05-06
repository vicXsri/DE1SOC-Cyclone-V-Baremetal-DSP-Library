/*
 * audio.h
 *
 *  Created on: 24-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef AUDIO_INC_AUDIO_H_
#define AUDIO_INC_AUDIO_H_

#include "main.h"

/* Typedef */
typedef struct {
    uint8_t*  audioData;
    uint32_t  dataSize;
    uint32_t  sampleRate;
    uint16_t  channels;
    uint16_t  bitsPerSample;
} AUDIO_BufferTypeDef;

/* Defines */
#define audio_gain 3

/* Funtions */
void AUDIO_PLAY(const AUDIO_BufferTypeDef* audio, uint8_t but);
FRESULT AUDIOToRAM(const char* filename, AUDIO_BufferTypeDef* audio);
static uint16_t read_le16(const uint8_t* p);
static uint32_t read_le32(const uint8_t* p);
void AUDIO_VOLINC();
void AUDIO_VOLDEC();

#endif /* AUDIO_INC_AUDIO_H_ */
