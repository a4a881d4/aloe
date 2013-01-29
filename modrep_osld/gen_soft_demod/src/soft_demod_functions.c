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

#include <stdio.h>
#include <oesr.h>
#include <params.h>
#include <skeleton.h>
#include <math.h>
#include <complex.h>

#define INCLUDE_DEFS_ONLY
#include "gen_soft_demod.h"


/**
 * @ingroup Soft Modulation Demapping based on the approximate
 * log-likelihood algorithm
 * Common algorithm that approximates the log-likelihood ratio. It takes
 * only the two closest constellation symbols into account, one with a '0'
 * and the other with a '1' at the given bit position.
 *
 * \param in input symbols (_Complex float)
 * \param out output symbols (float)
 * \param N Number of input symbols
 * \param M Number of constellation points
 * \param B Number of bits per symbol
 * \param symbols constellation symbols
 * \param S Soft demapping auxiliary matrix
 * \param sigma2 Noise vatiance
 */
void llr_approx(input_t *in, output_t *out, int N, int M, int B,
		_Complex float *symbols, int (*S)[6][32], float sigma2) {
	int i, s, b;
	float num, den;
	float new_num, new_den;
	float idiff0, qdiff0, idiff1, qdiff1;
	int change_sign = -1;

	for (s=0; s<N; s++) {  	/* recevied symbols */
	    for (b=0; b<B; b++) {/* bits per symbol*/
	        /* initiate num[b] and den[b] */
		idiff0 = __real__ in[s] - __real__ symbols[S[0][b][0]];
		qdiff0 = __imag__ in[s] - __imag__ symbols[S[0][b][0]];
	        num = idiff0*idiff0 + qdiff0*qdiff0;

	        idiff1 = __real__ in[s] - __real__ symbols[S[1][b][0]];
	        qdiff1 = __imag__ in[s] - __imag__ symbols[S[1][b][0]];
	        den = idiff1*idiff1 + qdiff1*qdiff1;

	        /* half the constellation symbols have '1'|'0' at any bit pos. */
	        for (i=1; i<M/2; i++) {
	        	idiff0 = __real__ in[s] - __real__ symbols[S[0][b][i]];
	        	qdiff0 = __imag__ in[s] - __imag__ symbols[S[0][b][i]];
	        	new_num = idiff0*idiff0 + qdiff0*qdiff0;

	        	idiff1 = __real__ in[s] - __real__ symbols[S[1][b][i]];
	        	qdiff1 = __imag__ in[s] - __imag__ symbols[S[1][b][i]];
	        	new_den = idiff1*idiff1 + qdiff1*qdiff1;

	        	if (new_num < num) {
	        		num = new_num;
	        	}
	        	if (new_den < den) {
	        		den = new_den;
	        	}
	        }
	        /* Theoretical LLR and approximate LLR values are positive if
	         * symbol(s) with '0' is/are closer and negative if symbol(s)
	         * with '1' are closer.
	         * Change sign if mapping negative to '0' and positive to '1' */
	        out[s*B+b] = change_sign*(den-num)/sigma2;
	    }
	}
}

/**
 * @ingroup Soft Modulation Demapping based on the approximate
 * log-likelihood algorithm
 * Common algorithm that approximates the log-likelihood ratio. It takes
 * only the two closest constellation symbols into account, one with a '0'
 * and the other with a '1' at the given bit position.
 *
 * \param in input symbols (_Complex float)
 * \param out output symbols (float)
 * \param N Number of input symbols
 * \param M Number of constellation points
 * \param B Number of bits per symbol
 * \param symbols constellation symbols
 * \param S Soft demapping auxiliary matrix
 * \param sigma2 Noise vatiance
 */
void llr_exact(input_t *in, output_t *out, int N, int M, int B,
		_Complex float *symbols, int (*S)[6][32], float sigma2) {
	int i, s, b;
	float num, den;
	float new_num, new_den;
	float idiff0, qdiff0, idiff1, qdiff1;
	int change_sign = -1;

	for (s=0; s<N; s++) {  	/* recevied symbols */
	    for (b=0; b<B; b++) {/* bits per symbol*/
	        /* initiate num[b] and den[b] */
		num = 0;
		den = 0;
	        /* half the constellation symbols have '1'|'0' at any bit pos. */
	        for (i=0; i<M/2; i++) {
	        	idiff0 = __real__ in[s] - __real__ symbols[S[0][b][i]];
	        	qdiff0 = __imag__ in[s] - __imag__ symbols[S[0][b][i]];
	        	num += exp(-1*(idiff0*idiff0 + qdiff0*qdiff0)/sigma2);

	        	idiff1 = __real__ in[s] - __real__ symbols[S[1][b][i]];
	        	qdiff1 = __imag__ in[s] - __imag__ symbols[S[1][b][i]];
	        	den += exp(-1*(idiff1*idiff1 + qdiff1*qdiff1)/sigma2);
	        }
	        /* Theoretical LLR and approximate LLR values are positive if
	         * symbol(s) with '0' is/are closer and negative if symbol(s)
	         * with '1' are closer.
	         * Change sign if mapping negative to '0' and positive to '1' */
	        out[s*B+b] = change_sign*log(num/den);
	    }
	}
}
