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
#include "FiltersStripes.h"
#include "FiltersConvolution.h"
using namespace std;

void LowPass(Image* in, Image* out, int threshold, int kernel_dim, bool type)
{
	int dimX = in->getX();
	int dimY = in->getY();
	int media[3];
	int diff, i, j, m_rgb, m_pixel;
	int* rgb;

	if (!type) {
		for (i = 0; i < dimY; i++) {
			for (j = 0; j < dimX; j++) {
				media[0] = *FindAverage(in, j, i, kernel_dim, media, type);
				diff = abs(in->GetBn(j + (i * dimX)) - media[0]);
				
				// the lower the 'threshold' is the stronger is the filter
				if (diff > threshold) out->SetBn(j + (i * dimX), media[0]);
				else out->SetBn(j + (i * dimX), in->GetBn(j + (i * dimX)));
			}
		}
	}
	
	if (type) {
		for (i = 0; i < dimY; i++) {
			for (j = 0; j < dimX; j++) {
				rgb = FindAverage(in, j, i, kernel_dim, media, type);
				m_pixel = (in->GetRgb(j + (i * dimX), 0) + in->GetRgb(j + (i * dimX), 1) + in->GetRgb(j + (i * dimX), 2)) / 3;
				m_rgb = (rgb[0] + rgb[1] + rgb[2])/3;
				diff = abs(m_pixel - m_rgb);
				if (diff > threshold) {
					out->SetRgb(j + (i * dimX), 0, rgb[0]);
					out->SetRgb(j + (i * dimX), 1, rgb[1]);
					out->SetRgb(j + (i * dimX), 2, rgb[2]);
				}
				else {
					out->SetRgb(j + (i * dimX), 0, in->GetRgb(j + (i * dimX), 0));
					out->SetRgb(j + (i * dimX), 1, in->GetRgb(j + (i * dimX), 1));
					out->SetRgb(j + (i * dimX), 2, in->GetRgb(j + (i * dimX), 2));
				}
			}
		}
	}
}

float BellShaped(int x, int y, int Liv, int dimX, int dimY)
{
	float d;
	x-= dimX/2;
	y-= dimY/2;
	d = sqrt(x*x*y*y);
	return (exp(-d/Liv));
}

void LowPassFilter(int Liv, double* matRe, double* matIm, int dimX, int dimY, bool ideal)
{
	int dx, dy;
	for (int y = 0; y < dimY; y++) {
		for (int x = 0 ; x < dimX; x++) {
			
			double k;
			if (ideal)
				k = 1;
			else
				k = BellShaped(x, y, Liv, dimX, dimY);
			
			matRe[x + (y * dimX)] *= k;
			matIm[x + (y * dimX)] *= k;
			dx = x - (dimX / 2);
			dy = y - (dimY / 2);
			if (sqrt(dx * dx + dy * dy) > Liv) {
				matRe[x + (y * dimX)] = 0;
				matIm[x + (y * dimX)] = 0;
			}
		}
	}
}

void HighPassFilter(int Liv, double* matRe, double* matIm, int dimX, int dimY, bool ideal)
{
	int dx, dy;
	for (int y = 0; y < dimY; y++) {
		for (int x = 0 ; x < dimX; x++) {
			double k;
			
			if (ideal)
				k = 1;
			else
				k = BellShaped(x, y, Liv, dimX, dimY);
			
			matRe[x + (y * dimX)] *= (3 -2*k);
			matIm[x + (y * dimX)] *= (3 -2*k);
			dx = x - (dimX / 2);
			dy = y - (dimY / 2);
			if (sqrt(dx * dx + dy * dy) < Liv) {
				matRe[x + (y * dimX)] = 0;
				matIm[x + (y * dimX)] = 0;
			}
		}
	}
}
