/*
 * tester.h
 *
 *  Created on: 23-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef DSP_INC_TESTER_H_
#define DSP_INC_TESTER_H_

#include "main.h"

/* Defines */
#define EPSILON 1e-5f

/* Funtions */
void compare_outputs(float *ref, float *test, uint32_t length);

#endif /* DSP_INC_TESTER_H_ */
