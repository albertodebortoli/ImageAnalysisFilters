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
#include <math.h>
using namespace std;

/**
 * Reads pixels from source image with a linear interpolation
 * avoiding (almost) aliasing.
 */
int Interpolation(Image* in, float x, float y, int rgb, bool type);

/** * Zoom filter *
 *
 * Based on 'zoom' ('+' or '-'), destination image will be
 * zoomed in or out. 'percent' is the value of zooming.
 */
void Zoom(Image* in, Image* out, char zoom, int percent, bool type);

/* * Resize filter *
 *
 * Based on 'width' destination image will be proportionally
 * scaled to the given width.
 */
void Resize(Image* in, Image* out, int width, bool type);

/* * Rotation filter *
 *
 * Based on '_angle' the destination image will results
 * rotated clockwise of the given degree number.
 */
void Rotation(Image* in, Image* out, float _angle, bool type);

// Compute the RGB channels and brightness histograms
void CalculateHistograms(Image* in, bool type);
