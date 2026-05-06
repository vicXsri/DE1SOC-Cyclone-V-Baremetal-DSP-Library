/*
 * image.h
 *
 *  Created on: 29-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef IMAGE_INC_IMAGE_H_
#define IMAGE_INC_IMAGE_H_

#include "main.h"

/* Typedef */
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t dataOffset;
    uint32_t imageSize;
    uint16_t bitsPerPixel;
    uint8_t *data;
} BMPImage;

/* Funtions */
FRESULT BMPToRAM(const char* filename, BMPImage* image);
void BMPImage_Display(const BMPImage* image);
void boot_imgFromRam();
void boot_img();
void displayGroup();
void LT24_DisplayEmbeddedFrame(const uint16_t* frame);

#endif /* IMAGE_INC_IMAGE_H_ */
