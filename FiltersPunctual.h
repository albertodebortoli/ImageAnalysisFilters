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
using namespace std;

/** * Equalize filter *
 *
 * Spread the hystogram of gray over all the range of color space. 
 * Find the value to balance the median of the hystogram of gray
 * bringing it to the center. Considering the brightness average
 * of 0.5, compute 0.5 = sum(i = 0; l) ist(i) finding l.
 * Applying on each pixel the following formula:
 * l_0 = sum(i = 0; l_i) ist(i)
 */
void Equalize(Image* in, Image* out, bool type);

/* * Brightness filter *
 *
 * Apply the filter both to grayscale image and color image (RGB)
 * using the formula RGBout = RGBin * (BNout/BNin)
 * 'level' is (BNout/BNin) i.e. (out->GetBn(i) / in->GetBn(i))
 */
void Brightness(Image* in, Image* out, float level, bool type);

/* * Auto Contrast filter *
 *
 * Brighten up bright areas and darken dark one
 * Stretch the grayscale hystogram to bounds using the formula:
 * Lout = a + b * Lin
 * math system is Lout = Loutmax (i.e. 255) and Lout = Loutmin (i.e. 0)
 * with Lin = Linmin and Lin = Linmax results in
 * a = 255 / (Linmax - Linmin)
 * b = (255 * Linmin) / (Linmax - Linmin)
 */
void AutoContrast(Image* in, Image* out, bool type);

/** * Gamma Correction filter *
 *
 * Stand out intermediate areas working on average density using the formula
 * Lout = Lin^esp (nonlinear)
 */
void Gamma(Image* in, Image* out, float esp, bool type);

/** * Gamma Channel Control filter *
 *
 * Stand out intermediate areas working on average density using the formula
 * Lout = Lin^esp (nonlinear)
 */
void Channel(Image* in, Image* out, float esp, char comp);

/** * Mirror filter *
 *
 * Reflect the image
 * horizontally: 'coord' == 'X'
 * vertically: 'coord' == 'Y'
 */
void Mirror(Image* in, Image* out, char coord, bool type);

/** * Pixelazer filter *
 *
 * Apply a filter to 'pixelate' the image that results
 * in a loss of details,
 * 'dim' is the size of the macro pixel desired
 */
void Pixelize(Image* in, Image* out, int dim, bool type);

/** * Grayscale filter *
 *
 * If the image is RGB interleaved, converts it in 
 * RGB interleaved but with grayscale values 
 */
void Grayscale(Image* in, Image* out);

/** * Invert filter *
 *
 * Apply the filter using the formula
 * out = 255 - in
 */
void Invert(Image* in, Image* out, bool type);
