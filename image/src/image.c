/*
 * image.c
 *
 *  Created on: 29-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "image.h"

BMPImage	dance[8];
BMPImage	group;

extern bool picState;

extern const uint16_t*  all_frames[ALL_FRAME_COUNT];

/* BMP Image Display Function */
void BMPImage_Display(const BMPImage* image)
{
    uint32_t x, y;
    uint32_t idx;
    uint8_t b, g, r;
    uint16_t colour;

    if (image->width > MAX_WIDTH || image->height > MAX_HEIGHT) return;

    LT24_SETWINDOW(GPIO1IP, image->width, image->height, 0, 0);

		for (y = 0; y < image->height; y++) {
			for (x = 0; x < image->width; x++) {
				idx = (y * image->width + x) * 3U;

				b = image->data[idx + 0];
				g = image->data[idx + 1];
				r = image->data[idx + 2];

				colour = (uint16_t)((((uint16_t)(r >> 3)) << 11) |
									(((uint16_t)(g >> 2)) << 5)  |
									(((uint16_t)(b >> 3))));

				LT24_WRITE(GPIO1IP, true, colour);
			}
		}

	while(1)	if(SWITCHIP_ReadPin(SWITCHIP, IMAGE_UNFIL) || SWITCHIP_ReadPin(SWITCHIP, IMAGE_FIL))	continue; 	else break;

    picState = false;
}

/*  Load BMP Image from SD Card to RAM */
FRESULT BMPToRAM(const char* filename, BMPImage* image)
{
    FIL file;
    FRESULT res;
    UINT bytesRead;
    uint8_t header[54];
    uint32_t rowSize;
    uint32_t rawRowBytes;
    uint8_t pad[3];
    int32_t y;

    if (image == NULL) return FR_INVALID_OBJECT;

    image->width = 0;
    image->height = 0;
    image->dataOffset = 0;
    image->imageSize = 0;
    image->bitsPerPixel = 0;
    image->data = NULL;

    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) return res;

    res = f_read(&file, header, sizeof(header), &bytesRead);
    if (res != FR_OK || bytesRead != sizeof(header)) {
        f_close(&file);
        return FR_INT_ERR;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        f_close(&file);
        return FR_INVALID_OBJECT;
    }

    image->dataOffset =  (uint32_t)header[10]
                       | ((uint32_t)header[11] << 8)
                       | ((uint32_t)header[12] << 16)
                       | ((uint32_t)header[13] << 24);

    image->width =  (uint32_t)header[18]
                  | ((uint32_t)header[19] << 8)
                  | ((uint32_t)header[20] << 16)
                  | ((uint32_t)header[21] << 24);

    image->height =  (uint32_t)header[22]
                   | ((uint32_t)header[23] << 8)
                   | ((uint32_t)header[24] << 16)
                   | ((uint32_t)header[25] << 24);

    image->bitsPerPixel =  (uint16_t)header[28]
                         | ((uint16_t)header[29] << 8);

    if (image->bitsPerPixel != 24U) {
        f_close(&file);
        return FR_INVALID_OBJECT;
    }

    if ((((uint32_t)header[30])
       | ((uint32_t)header[31] << 8)
       | ((uint32_t)header[32] << 16)
       | ((uint32_t)header[33] << 24)) != 0U) {
        f_close(&file);
        return FR_INVALID_OBJECT;
    }

    rawRowBytes = image->width * 3U;
    rowSize = (rawRowBytes + 3U) & ~3U;
    image->imageSize = rawRowBytes * image->height;

    image->data = (uint8_t*)malloc(image->imageSize);
    if (image->data == NULL) {
        f_close(&file);
        return FR_NOT_ENOUGH_CORE;
    }

    res = f_lseek(&file, image->dataOffset);
    if (res != FR_OK) {
        free(image->data);
        image->data = NULL;
        f_close(&file);
        return res;
    }

    for (y = (int32_t)image->height - 1; y >= 0; y--) {
        uint8_t* rowPtr = &image->data[(uint32_t)y * rawRowBytes];

        res = f_read(&file, rowPtr, rawRowBytes, &bytesRead);
        if (res != FR_OK || bytesRead != rawRowBytes) {
            free(image->data);
            image->data = NULL;
            f_close(&file);
            return FR_INT_ERR;
        }

        if (rowSize > rawRowBytes) {
            UINT padBytes = rowSize - rawRowBytes;
            res = f_read(&file, pad, padBytes, &bytesRead);
            if (res != FR_OK || bytesRead != padBytes) {
                free(image->data);
                image->data = NULL;
                f_close(&file);
                return FR_INT_ERR;
            }
        }
    }

    f_close(&file);
    return FR_OK;
}

/*  Load the images from the stored array */
void LT24_DisplayEmbeddedFrame(const uint16_t* frame)
{
    if (frame == NULL) return;

    LT24_SETWINDOW(GPIO1IP, MAX_WIDTH, MAX_HEIGHT, 0, 0);

    for (uint32_t i = 0; i < EMBEDDED_FRAME_PIXELS; i++) 	LT24_WRITE(GPIO1IP, true, frame[i]);

}

/*  Load the images from RAM and display it in boot */
void boot_imgFromRam(){

	BMPToRAM("group.bmp", &group);
	BMPToRAM("dance0.bmp", &dance[0]);
	BMPToRAM("dance1.bmp", &dance[1]);
	BMPToRAM("dance2.bmp", &dance[2]);
	BMPToRAM("dance3.bmp", &dance[3]);
	BMPToRAM("dance4.bmp", &dance[4]);
	BMPToRAM("dance5.bmp", &dance[5]);
	BMPToRAM("dance6.bmp", &dance[6]);
	BMPToRAM("dance7.bmp", &dance[7]);

	for(uint8_t j = 0; j < 3; j++ )	for(uint8_t i = 0; i< 8; i++ )	BMPImage_Display(&dance[i]);

	BMPImage_Display(&group);
}

/*  Load the images from array and display it in boot  */
void boot_img(){
	for(uint8_t j = 0; j < 3; j++ )
		for(uint8_t i = 0; i< 8; i++ )
			LT24_DisplayEmbeddedFrame(all_frames[i]);
}

/* Load Image from array and display */
void displayGroup(){
	LT24_DisplayEmbeddedFrame(all_frames[7]);
	picState = true;
}
