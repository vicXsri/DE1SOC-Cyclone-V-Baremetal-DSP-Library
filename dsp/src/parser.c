/*
 * parser.c
 *
 *  Created on: 17-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "parser.h"

float fir_i_samples[SAMPLES_PER_ARRAY];
float fir_q_samples[SAMPLES_PER_ARRAY];

int16_t audio_raw[SAMPLES_PER_ARRAY];

complexFFT_t sdr_buffer[SAMPLES_PER_ARRAY];
extern float AUDIO_fir_filtered_data[AUDIO_FIR_NUM_SAMPLES];

Image img;

/* SDR FIR WAV IQ Parser */
void SDR_FIR_WAV_IQ_Parser(void)
{

	FATFS fs;
    FIL file;
    FRESULT res;
    UINT bytes_read;

    uint64_t count = 0;

    int16_t sample[2];

    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t totalTime = 0;

    res = f_open(&file, "navtex.wav", FA_READ);

	if (res == FR_OK)
	{
        printf("success opening file: %d\n", res);
	}else{
        printf("Error opening file: %d\n", res);
		while (1);
	}

    f_lseek(&file, 44);

    start = getTick();

    while (1)
    {
        res = f_read(&file, sample, sizeof(sample), &bytes_read);

        if (res != FR_OK || bytes_read < sizeof(sample))
        {
            break;
        }

        fir_i_samples[count] = sample[0] / 32768.0f;
        fir_q_samples[count] = sample[1] / 32768.0f;

        if(count >= SDR_FIR_NUM_SAMPLES) break;

        count++;

    }

    end = getTick();

    totalTime = end - start;

    printf("Finished processing %llu I/Q samples, extracted @ %u ms\n", count, totalTime);

    f_close(&file);
    printf("File Closed !\n");

	sdr_fir_process(fir_i_samples, fir_q_samples, count);

	LEDIP_WritePin(LEDIP, LED_PIN_0, HIGH);

}

/* SDR FFT WAV IQ Parser */
void SDR_FFT_WAV_IQ_Parser(void)
{

	FATFS fs;
    FIL file;
    FRESULT res;
    UINT bytes_read;

    uint64_t count = 0;

    int16_t sample[2];

    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t totalTime = 0;

    res = f_open(&file, "navtex.wav", FA_READ);

	if (res == FR_OK)
	{
        printf("success opening file: %d\n", res);
	}else{
        printf("Error opening file: %d\n", res);
		while (1);
	}

    f_lseek(&file, 44);

    start = getTick();

    while (1)
    {
        res = f_read(&file, sample, sizeof(sample), &bytes_read);

        if (res != FR_OK || bytes_read < sizeof(sample))
        {
            break;
        }

        sdr_buffer[count].imag = sample[0] / 32768.0f;
        sdr_buffer[count].real = sample[1] / 32768.0f;

        if(count >= SDR_FFT_NUM_SAMPLES) break;

        count++;
    }

    end = getTick();

    totalTime = end - start;

    printf("Finished processing %llu I/Q samples, extracted @ %u ms\n", count, totalTime);

    f_close(&file);
    printf("File Closed !\n");

    sdr_fft_process(sdr_buffer, count);

	LEDIP_WritePin(LEDIP, LED_PIN_1, HIGH);

}

/* AUDIO FIR WAV Parser */
void AUDIO_FIR_WAV_IQ_Parser(void){
    FATFS fs;
    FIL fileIn, fileOut;
    FRESULT res;
    UINT bytes_read, bytes_written;

    uint64_t count = 0;
    int16_t sample;
    int16_t filteredSample;

    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t totalTime = 0;

    res = f_open(&fileIn, "audiodata.wav", FA_READ);

    if (res == FR_OK){
        printf("success opening input file: %d\n", res);
    }else{
        printf("Error opening input file: %d\n", res);
        while (1);
    }

    f_lseek(&fileIn, 44);

    start = getTick();

    while (1){
        res = f_read(&fileIn, &sample, sizeof(sample), &bytes_read);

        if (res != FR_OK || bytes_read < sizeof(sample)){
            break;
        }

        audio_raw[count] = sample;

        if (count >= AUDIO_FIR_NUM_SAMPLES) break;
        count++;
    }

    end = getTick();
    totalTime = end - start;

    printf("Finished processing %llu raw samples, extracted @ %u ms\n", count, totalTime);

    f_close(&fileIn);
    printf("Input file closed!\n");

    audio_process(audio_raw, count);

    res = f_open(&fileOut, "audiodatafil.wav", FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK){
        printf("Error opening output file: %d\n", res);
        while (1);
    }

    res = WAV_WriteHeader(&fileOut, (uint32_t)count);
    if (res != FR_OK){
        printf("Error writing WAV header: %d\n", res);
        f_close(&fileOut);
        while (1);
    }

    start = getTick();

    for (uint64_t i = 0; i < count; i++){
        float x = AUDIO_fir_filtered_data[i];

        if (x > 1.0f)  x = 1.0f;
        if (x < -1.0f) x = -1.0f;

        filteredSample = (int16_t)(x * 32767.0f);

        res = f_write(&fileOut, &filteredSample, sizeof(filteredSample), &bytes_written);
        if (res != FR_OK || bytes_written != sizeof(filteredSample)){
            printf("Error writing filtered sample: %d\n", res);
            f_close(&fileOut);
            while (1);
        }
    }

    end = getTick();
    totalTime = end - start;

    printf("Finished writing %llu filtered samples @ %u ms\n", count, totalTime);

    f_close(&fileOut);
    printf("Output file closed!\n");
    LEDIP_WritePin(LEDIP, LED_PIN_2, HIGH);

}

/* AUDIO FIR WAV Header Writer */
static FRESULT WAV_WriteHeader(FIL* file, uint32_t numSamples){

	UINT bytes_written;
    uint8_t hdr[44];
    uint32_t dataSize  = numSamples * 2U;
    uint32_t riffSize  = 36U + dataSize;
    uint32_t byteRate  = 48000U * 2U;
    uint16_t blockAlign = 2U;

    memcpy(&hdr[0],  "RIFF", 4);
    write_le32(&hdr[4],  riffSize);
    memcpy(&hdr[8],  "WAVE", 4);

    memcpy(&hdr[12], "fmt ", 4);
    write_le32(&hdr[16], 16U);
    write_le16(&hdr[20], 1U);
    write_le16(&hdr[22], 1U);
    write_le32(&hdr[24], 48000U);
    write_le32(&hdr[28], byteRate);
    write_le16(&hdr[32], blockAlign);
    write_le16(&hdr[34], 16U);

    memcpy(&hdr[36], "data", 4);
    write_le32(&hdr[40], dataSize);

    return f_write(file, hdr, 44, &bytes_written) == FR_OK && bytes_written == 44 ? FR_OK : FR_DISK_ERR;
}

/* IMAGE MEDIAN BMP Parser */
void MEDIAN_IMAG_BMP_IQ_Parser(void)
{

    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t totalTime = 0;

    if (!BMP_ReadImage("minion.bmp", &img)) {
        while (1);
    }

    start = getTick();

    image_filter(&img);

    end = getTick();
    totalTime = end - start;
    printf("Finished Filter @ %u ms\n", totalTime);

    if (!BMP_WriteImage("minionfil.bmp", &img)) {
        while (1);
    }

    free(img.pixels);
    img.pixels = NULL;

    LEDIP_WritePin(LEDIP, LED_PIN_5, HIGH);

}

/* MEDIAN Write Image BMP */
bool BMP_WriteImage(const char *filename, const Image *img)
{
    FIL file;
    FRESULT res;
    UINT bytes_written;
    uint8_t header[54] = {0};
    uint8_t pad[3] = {0, 0, 0};
    uint32_t rowSize;
    uint32_t pixelBytes;
    uint32_t fileSize;

    rowSize = ((uint32_t)img->width * 3U + 3U) & ~3U;
    pixelBytes = rowSize * (uint32_t)img->height;
    fileSize = 54U + pixelBytes;

    header[0] = 'B';
    header[1] = 'M';

    header[2] = (uint8_t)(fileSize & 0xFF);
    header[3] = (uint8_t)((fileSize >> 8) & 0xFF);
    header[4] = (uint8_t)((fileSize >> 16) & 0xFF);
    header[5] = (uint8_t)((fileSize >> 24) & 0xFF);

    header[10] = 54;

    header[14] = 40;

    header[18] = (uint8_t)(img->width & 0xFF);
    header[19] = (uint8_t)((img->width >> 8) & 0xFF);
    header[20] = (uint8_t)((img->width >> 16) & 0xFF);
    header[21] = (uint8_t)((img->width >> 24) & 0xFF);

    header[22] = (uint8_t)(img->height & 0xFF);
    header[23] = (uint8_t)((img->height >> 8) & 0xFF);
    header[24] = (uint8_t)((img->height >> 16) & 0xFF);
    header[25] = (uint8_t)((img->height >> 24) & 0xFF);

    header[26] = 1;
    header[28] = 24;

    header[34] = (uint8_t)(pixelBytes & 0xFF);
    header[35] = (uint8_t)((pixelBytes >> 8) & 0xFF);
    header[36] = (uint8_t)((pixelBytes >> 16) & 0xFF);
    header[37] = (uint8_t)((pixelBytes >> 24) & 0xFF);

    res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) {
        printf("Error opening BMP output: %d\n", res);
        return false;
    }

    res = f_write(&file, header, sizeof(header), &bytes_written);
    if (res != FR_OK || bytes_written != sizeof(header)) {
        printf("Error writing BMP header: %d\n", res);
        f_close(&file);
        return false;
    }

    for (int32_t y = img->height - 1; y >= 0; y--) {
        for (int32_t x = 0; x < img->width; x++) {
            size_t idx = ((size_t)y * (size_t)img->width + (size_t)x) * 3U;
            uint8_t bgr[3];

            bgr[0] = img->pixels[idx + 2];
            bgr[1] = img->pixels[idx + 1];
            bgr[2] = img->pixels[idx + 0];

            res = f_write(&file, bgr, 3, &bytes_written);
            if (res != FR_OK || bytes_written != 3) {
                printf("Error writing BMP pixel data: %d\n", res);
                f_close(&file);
                return false;
            }
        }

        if (rowSize > (uint32_t)img->width * 3U) {
            UINT padBytes = rowSize - (uint32_t)img->width * 3U;
            res = f_write(&file, pad, padBytes, &bytes_written);
            if (res != FR_OK || bytes_written != padBytes) {
                printf("Error writing BMP padding: %d\n", res);
                f_close(&file);
                return false;
            }
        }
    }

    f_close(&file);
    printf("BMP saved: %s\n", filename);
    return true;
}

/* BMP Read Image */
bool BMP_ReadImage(const char *filename, Image *img)
{
    FIL file;
    FRESULT res;
    UINT bytes_read;
    uint8_t header[54];
    uint8_t bgr[3];
    uint8_t pad[3];
    uint32_t rowSize;
    uint32_t dataOffset;
    int32_t width;
    int32_t height;
    bool bottomUp;

    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        printf("Error opening BMP input: %d\n", res);
        return false;
    }

    res = f_read(&file, header, sizeof(header), &bytes_read);
    if (res != FR_OK || bytes_read != sizeof(header)) {
        printf("Error reading BMP header: %d\n", res);
        f_close(&file);
        return false;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("Invalid BMP signature\n");
        f_close(&file);
        return false;
    }

    dataOffset = bmp_read_u32(&header[10]);
    width      = (int32_t)bmp_read_u32(&header[18]);
    height     = (int32_t)bmp_read_u32(&header[22]);

    if (bmp_read_u16(&header[28]) != 24) {
        printf("Only 24-bit BMP supported\n");
        f_close(&file);
        return false;
    }

    if (bmp_read_u32(&header[30]) != 0) {
        printf("Only uncompressed BMP supported\n");
        f_close(&file);
        return false;
    }

    if (width <= 0 || height == 0) {
        printf("Invalid BMP dimensions\n");
        f_close(&file);
        return false;
    }

    bottomUp = (height > 0);
    if (height < 0) height = -height;

    rowSize = ((uint32_t)width * 3U + 3U) & ~3U;

    img->width = width;
    img->height = height;
    img->pixels = (uint8_t *)malloc((size_t)width * (size_t)height * 3U);

    if (!img->pixels) {
        printf("malloc failed for image buffer\n");
        f_close(&file);
        return false;
    }

    res = f_lseek(&file, dataOffset);
    if (res != FR_OK) {
        printf("Error seeking BMP data: %d\n", res);
        free(img->pixels);
        img->pixels = NULL;
        f_close(&file);
        return false;
    }

    for (int32_t y = 0; y < height; y++) {
        int32_t dstY = bottomUp ? (height - 1 - y) : y;

        for (int32_t x = 0; x < width; x++) {
            size_t idx;

            res = f_read(&file, bgr, 3, &bytes_read);
            if (res != FR_OK || bytes_read != 3) {
                printf("Error reading BMP pixels: %d\n", res);
                free(img->pixels);
                img->pixels = NULL;
                f_close(&file);
                return false;
            }

            idx = ((size_t)dstY * (size_t)width + (size_t)x) * 3U;
            img->pixels[idx + 0] = bgr[2];
            img->pixels[idx + 1] = bgr[1];
            img->pixels[idx + 2] = bgr[0];
        }

        if (rowSize > (uint32_t)width * 3U) {
            UINT padBytes = rowSize - (uint32_t)width * 3U;
            res = f_read(&file, pad, padBytes, &bytes_read);
            if (res != FR_OK || bytes_read != padBytes) {
                printf("Error reading BMP padding: %d\n", res);
                free(img->pixels);
                img->pixels = NULL;
                f_close(&file);
                return false;
            }
        }
    }

    f_close(&file);
    printf("BMP loaded: %d x %d\n", img->width, img->height);
    return true;
}

static void write_le16(uint8_t* p, uint16_t v){
    p[0] = (uint8_t)(v & 0xFFU);
    p[1] = (uint8_t)((v >> 8) & 0xFFU);
}

static void write_le32(uint8_t* p, uint32_t v){
    p[0] = (uint8_t)(v & 0xFFU);
    p[1] = (uint8_t)((v >> 8) & 0xFFU);
    p[2] = (uint8_t)((v >> 16) & 0xFFU);
    p[3] = (uint8_t)((v >> 24) & 0xFFU);
}

static uint16_t bmp_read_u16(const uint8_t *p)
{
    return (uint16_t)(p[0] | (p[1] << 8));
}

static uint32_t bmp_read_u32(const uint8_t *p)
{
    return (uint32_t)(p[0] |
                     (p[1] << 8) |
                     (p[2] << 16) |
                     (p[3] << 24));
}
