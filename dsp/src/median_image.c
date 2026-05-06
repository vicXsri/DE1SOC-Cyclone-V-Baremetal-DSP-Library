/*
 * median_image.c
 *
 *  Created on: 29-Apr-2026
 *      Author: Sanjukta Aparna Venkatachalam
 */

#include "median_image.h"

/* Function to clamp the coordianated the Salt and Pepper */
int clamp_coord(int v, int low, int high){
    if (v < low) return low;
    if (v > high) return high;
    return v;
}

/* Function to sort back to orignal */
static void sort9(uint8_t *window){

    for (int j = 0; j < 8; j++) {
        for (int k = j + 1; k < 9; k++) {
            if (window[k] < window[j]) {
                uint8_t temp = window[j];
                window[j] = window[k];
                window[k] = temp;
            }
        }
    }
}

/* Median Filter */
void median_filter(Image *img){

    int w = img->width;
    int h = img->height;

    uint8_t *copy = (uint8_t *)malloc(w * h * 3);
    if (!copy) {
        printf("ERROR: malloc failed\n");
        return;
    }

    memcpy(copy, img->pixels, w * h * 3);

    uint8_t window[9];
    uint32_t idx = 0;
    for (int ch = 0; ch < 3; ch++) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {

                int center_index = (y * w + x) * 3 + ch;

                uint8_t center = copy[center_index];

                if (center != 0 && center != 255) {
                    continue;
                }

                idx = 0;

                for (int j = -1; j <= 1; j++) {
                    for (int i = -1; i <= 1; i++) {
                        int xx = clamp_coord(x + i, 0, w - 1);
                        int yy = clamp_coord(y + j, 0, h - 1);
                        window[idx++] = copy[(yy * w + xx) * 3 + ch];
                    }
                }

                sort9(window);

                img->pixels[center_index] = window[4];
            }
        }
    }

    free(copy);

    printf("Switching median filtering done\n");
}

/* image filter function */
void image_filter(Image *img)
{
	median_filter(img);
}
