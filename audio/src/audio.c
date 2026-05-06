/*
 * audio.c
 *
 *  Created on: 24-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "audio.h"

static uint32_t volume = 	0x70;
extern I2C_HandleTypeDef 	hi2c0;

/* Play the audio */
void AUDIO_PLAY(const AUDIO_BufferTypeDef* audio, uint8_t but){

	uint32_t fifoSpace;

	CODECIP_CLEARFIFO(CODECIP, ENABLE, ENABLE);

		for (uint32_t i = 0; i + 1 < audio->dataSize; i += 2) {
			HEXIP_Text(WRITE_PLAY);
			int16_t mono16 	 = (int16_t)read_le16(&audio->audioData[i]);
			int32_t sample24 = ((int32_t)mono16) << 8;
			sample24 *= audio_gain;

			if (sample24 > 8388607) sample24 = 8388607;
			if (sample24 < -8388608) sample24 = -8388608;

			do {
				CODECIP_GETFIFOSPACE(CODECIP, &fifoSpace);
			} while (fifoSpace == 0);

			CODECIP_WRITE(CODECIP,  (uint32_t)sample24, (uint32_t)sample24);

			if(!SWITCHIP_ReadPin(SWITCHIP, but))	break;

			if(BUTTONIP_ReadPin(BUTTONIP, VOL_DEC))		AUDIO_VOLDEC();

			if(BUTTONIP_ReadPin(BUTTONIP, VOL_INC))		AUDIO_VOLINC();
		}
}

/* increase the volume of the audio */
void AUDIO_VOLINC(){

    if(volume < 0x7F)	volume++;
    else				return;

	WM8731_CODEC_WRITE(&hi2c0, CODEC_LTOUTCTRL, volume);
	WM8731_CODEC_WRITE(&hi2c0, CODEC_RTOUTCTRL, volume);
}

/* decrease the volume of the audio */
void AUDIO_VOLDEC(){

    if(volume > 0x70)	volume--;
    else				return;

	WM8731_CODEC_WRITE(&hi2c0, CODEC_LTOUTCTRL, volume);
	WM8731_CODEC_WRITE(&hi2c0, CODEC_RTOUTCTRL, volume);
}

/* Load Audio from SD CARD to RAM */
FRESULT AUDIOToRAM(const char* filename, AUDIO_BufferTypeDef* audio) {
    FIL file;
    FRESULT fr;
    UINT bytesRead;
    unsigned int fifoSpace;
    uint8_t riffHdr[12];
    uint8_t chunkHdr[8];
    uint8_t fmt[16];
    uint16_t audioFormat = 0;
    int foundFmt = 0;
    int foundData = 0;

    fr = f_open(&file, filename, FA_READ);
    if (fr != FR_OK) return fr;

    fr = f_read(&file, riffHdr, sizeof(riffHdr), &bytesRead);
    if (fr != FR_OK || bytesRead != sizeof(riffHdr)) {
        f_close(&file);
        return FR_INVALID_OBJECT;
    }

    if (memcmp(&riffHdr[0], "RIFF", 4) != 0 || memcmp(&riffHdr[8], "WAVE", 4) != 0) {
        f_close(&file);
        return FR_INVALID_OBJECT;
    }

    while (!foundData) {
        fr = f_read(&file, chunkHdr, sizeof(chunkHdr), &bytesRead);
        if (fr != FR_OK || bytesRead != sizeof(chunkHdr)) {
            f_close(&file);
            return FR_INVALID_OBJECT;
        }

        uint32_t chunkSize = read_le32(&chunkHdr[4]);

        if (memcmp(&chunkHdr[0], "fmt ", 4) == 0) {
            if (chunkSize < 16) {
                f_close(&file);
                return FR_INVALID_OBJECT;
            }

            fr = f_read(&file, fmt, 16, &bytesRead);
            if (fr != FR_OK || bytesRead != 16) {
                f_close(&file);
                return FR_INVALID_OBJECT;
            }

            audioFormat   = read_le16(&fmt[0]);
            audio->channels      = read_le16(&fmt[2]);
            audio->sampleRate    = read_le32(&fmt[4]);
            audio->bitsPerSample = read_le16(&fmt[14]);
            foundFmt = 1;

            if (chunkSize > 16) {
                f_lseek(&file, f_tell(&file) + (chunkSize - 16));
            }
        } else if (memcmp(&chunkHdr[0], "data", 4) == 0) {
            audio->dataSize = chunkSize;
            foundData = 1;
            break;
        } else {
            f_lseek(&file, f_tell(&file) + chunkSize);
        }

        if (chunkSize & 1U) {
            f_lseek(&file, f_tell(&file) + 1);
        }
    }

    if (!foundFmt || !foundData) {
        f_close(&file);
        return FR_INVALID_OBJECT;
    }

    if (audioFormat != 1 || audio->channels != 1 || audio->bitsPerSample != 16 || audio->sampleRate != 48000) {
        f_close(&file);
        return FR_INVALID_OBJECT;
    }

    audio->audioData = (uint8_t*)malloc(audio->dataSize);

    if (audio->audioData == NULL) {
        f_close(&file);
        return FR_NOT_ENOUGH_CORE;
    }

    fr = f_read(&file, audio->audioData, audio->dataSize, &bytesRead);
    f_close(&file);

    if (fr != FR_OK || bytesRead != audio->dataSize) {
        free(audio->audioData);
        return FR_DISK_ERR;
    }

    return FR_OK;
}

static uint32_t read_le32(const uint8_t* p) {
    return (uint32_t)p[0]
         | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16)
         | ((uint32_t)p[3] << 24);
}

static uint16_t read_le16(const uint8_t* p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}
