/*
 * Copyright (c) 2008, Alberto De Bortoli at
 * Universit� degli Studi di Padova. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *	 * Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	 * Redistributions in binary form must reproduce the above copyright
 *	   notice, this list of conditions and the following disclaimer in the
 *	   documentation and/or other materials provided with the distribution.
 *	 * Neither the name of the Universit� degli Studi di Padova nor the
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

/** * LowPass filter *
 * 
 * Cuts off high frequencies
 * 'threshold' is the cut off value and 'kernel_dim' is the kernel dimension
 * and result in a stronger or weaker filter effect.
 */
void LowPass(Image* in, Image* out, int threshold, int kernel_dim, bool type);

// Compute the BellShaped for avoiding hard step of frequencies
// i.e. the ILPF and the IHPF (Ideal Low/High Pass Filtering)
float BellShaped(int x, int y, int Liv, int dimX, int dimY);

/** * LowPass filter *
 * 
 * Cuts off high frequencies on FFT transform
 * 'ideal' == true means Ideal filtering
 * 'ideal' == false means BellShaped filtering
 */
void LowPassFilter(int Liv, double* matRe, double* matIm, int dimX, int dimY, bool ideal);

/** * HighPass filter *
 *
 * Cuts off low frequencies on FFT transform
 * 'ideal' == true means Ideal filtering
 * 'ideal' == false means BellShaped filtering
 */
void HighPassFilter(int Liv, double* matRe, double* matIm, int dimX, int dimY, bool ideal);
