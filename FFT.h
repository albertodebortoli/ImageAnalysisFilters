/*
 * Copyright (c) 2008, Alberto De Bortoli at
 * Università degli Studi di Padova. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *	 * Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	 * Redistributions in binary form must reproduce the above copyright
 *	   notice, this list of conditions and the following disclaimer in the
 *	   documentation and/or other materials provided with the distribution.
 *	 * Neither the name of the Università degli Studi di Padova nor the
 *	   names of its contributors may be used to endorse or promote products
 *	   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Alberto De Bortoli ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL Alberto De Bortoli BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Image.h"
using namespace std;

/**
 * Scale the image to next power of 2 size via deep copy 
 * Empty pixels are black.
 */
void AdjustSizeToPower2(Image* in, int dimX, int dimY, int dimX_original, int dimY_original);

/**
 * Call SwapQuadranti on matrices matRe e matIm
 */
void SwapSpectrum(double* matRe, double* matIm, int dimX, int dimY);

/**
 * Work on spectrum (matRe e matIm) derived from FFT transform.
 * Reorder areas in the correct way to apply filters.
 */
/*
 ___ ___		  ___ ___
|1  |2  |		|4  |3  |
|___|___|		|___|___| 
|3  |4  |   -->  |2  |1  | 
|___|___|		|___|___| 

*/
void SwapQuadranti(double* origine, int dimX, int dimY);

/**
 * Compute the nearest power of 2 of 'n' such that
 * twopm = 2**m <= n
 * Return true if (2**m == n)
 */
int Powerof2(int n, int* m, int* twopm);

/**
 * Perform 2D FFT in place, given 2 arrays 'matRe' and 'MatIm'
 * for complex numbers.
 * 'dir' == 1 for forward FFT
 * 'dir' == -1 for backward FFT
 * 'nx' and 'ny' are the matRe and matIm sizes 
 * original code by Paul Bourke
 * url: http://local.wasp.uwa.edu.au/~pbourke/other/dft/
 */
void FFT2D(double* matRe, double* matIm, int nx, int ny, int dir);

/**
 * This computes an in-place complex-to-complex FFT
 * x and y are the real and imaginary arrays of 2^m points.
 * dir =  1 gives forward transform
 * dir = -1 gives reverse transform
 *
 *  Formula: forward
 *                N-1
 *                ---
 *            1   \		  - j k 2 pi n / N
 *    X(n) = ---   >   x(k) e                   = forward transform
 *            N   /                             n=0..N-1
 *                ---
 *                k=0
 *
 *    Formula: reverse
 *                N-1
 *                ---
 *                \       j k 2 pi n / N
 *    X(n) =       >   x(k) e                   = forward transform
 *                /	                            n=0..N-1
 *                ---
 *                k=0
 */
void FFT(int dir, int m, double *x, double *y);
