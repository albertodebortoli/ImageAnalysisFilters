/*
 * Copyright (c) 2008, Alberto De Bortoli at
 * Universitˆ degli Studi di Padova. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *	 * Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	 * Redistributions in binary form must reproduce the above copyright
 *	   notice, this list of conditions and the following disclaimer in the
 *	   documentation and/or other materials provided with the distribution.
 *	 * Neither the name of the Universitˆ degli Studi di Padova nor the
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
#include <math.h>
using namespace std;

/** * Smooth filter *
 *
 * Use FindAverage to compute the average of the range of pixel,
 * 'kernel_dim' is the kernel dimension and results in a stronger
 * or weaker filter effect.
 */
void Smooth(Image* in, Image* out, int kernel_dim, bool type);

/**
 * Find the average of the pixels inside the kernel
 * 'kernel_dim' is the kernel dimension and results in a stronger
 * or weaker filter effect.
 */
int* FindAverage(Image* in, int x, int y, int kernel_dim, int* average, bool type);

/** * Remove noise filter *
 *
 * Use FindMedian to find median around the given pixel. 
 * 'kernel_dim' is the kernel dimension and results in a stronger
 * or weaker filter effect.
 */
void NoiseRemoval(Image* in, Image* out, int kernel_dim, bool type);

/**
 * Find the median of the pixels inside the kernel
 * 'kernel_dim' is the kernel dimension and results in a stronger
 * or weaker filter effect.
 */
int* FindMedian(Image* in, int x, int y, int kernel_dim, int* median, bool type);

/** * Convolution filter *
 *
 * Use SharpBlurOut to compute the value for the output pixels.
 * 'effect' can be:
 * 'S' means Sharpen
 * 'B' means Blur
 * 'O' means Outliner
 */
void Convolution(Image* in, Image* out, char effect, bool type);

/**
 * Apply different filter based on the given kernel
 * 'kernel' is the kernel dimension and results in a stronger
 * or weaker filter effect.
 */
int* SharpBlurOut(Image* in, int x, int y, int* kernel, int lunghezza, int* value, bool type);
