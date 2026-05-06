/*
 * tester.c
 *
 *  Created on: 23-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "tester.h"

/* Compare Scalar vs Vector outputs */
void compare_outputs(float *ref, float *test, uint32_t length)
{
    uint32_t mismatch_count = 0;

    for (uint32_t i = 0; i < length; i++)
    {
        float diff = fabsf(ref[i] - test[i]);

        if (diff > EPSILON)
        {
            if (mismatch_count == 0)
            {
                printf("\nMismatch detected!\n");
                printf("Index\tREF\t\tNEON\t\tDIFF\n");
            }

            printf("%lu\t%.6f\t%.6f\t%.6f\n",
                   (unsigned long)i, ref[i], test[i], diff);

            mismatch_count++;

            if (mismatch_count >= 20)
            {
                printf("...more mismatches\n");
                break;
            }
        }
    }

    if (mismatch_count == 0)
    {
        printf("All values match within tolerance (%.6f)\n", EPSILON);
    }
    else
    {
        printf("Total mismatches (shown): %lu\n", (unsigned long)mismatch_count);
    }
}
