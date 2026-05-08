/*
 * median_image.h
 *
 *  Created on: 29-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 *      		Sanjukta Aparna Venkatachalam
 */

#ifndef DSP_INC_MEDIAN_IMAGE_H_
#define DSP_INC_MEDIAN_IMAGE_H_

#include "main.h"

/* Typedef */
typedef struct {
    int      width;
    int      height;
    uint8_t *pixels;
} Image;

/* Funtions */
int clamp_coord(int v, int low, int high);
static void sort9(uint8_t *window);

void median_filter(Image *img);

void image_filter(Image* img);

bool BMP_WriteImage(const char *filename, const Image *img);
bool BMP_ReadImage(const char *filename, Image *img);

#endif /* DSP_INC_MEDIAN_IMAGE_H_ */
