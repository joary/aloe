/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "turbocoder.h"


/** @defgroup lte_encoder lte_encoder
 *
 * The lte_encoder module implements an 3GPP LTE 1/3 Turbo Coder.
 * The input and output interfaces are bitstreams.
 *
 * If several interfaces are defined, one code block is decoded from each interface every timeslot.
 *
 * @{
 */

typedef char input_t;
typedef char output_t;

#define INPUT_MAX_SAMPLES 	5114
#define OUTPUT_MAX_SAMPLES 	RATE*5114+TOTALTAIL+64

#define NOF_INPUT_ITF		1
#define NOF_OUTPUT_ITF		1

/**@} */

//#define GENERATE_COMPLEX


/********* do not need to modify beyond here */


#ifndef INCLUDE_DEFS_ONLY

/* Input and output buffer sizes (in number of samples) */
const int input_max_samples = INPUT_MAX_SAMPLES;
const int output_max_samples = OUTPUT_MAX_SAMPLES;

/* leave these two lines unmodified */
const int input_sample_sz = sizeof(input_t);
const int output_sample_sz = sizeof(output_t);

/* Number of I/O interfaces. All have the same maximum size */
const int nof_input_itf = NOF_INPUT_ITF;
const int nof_output_itf = NOF_OUTPUT_ITF;

#endif