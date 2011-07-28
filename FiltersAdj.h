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

#ifndef FILTERSADJ_H
#define FILTERSADJ_H

#include"Image.h"
#include<math.h>
using namespace std;

int Interpolation(Image*, float, float, int, bool);
void Zoom(Image*, Image*, char, int, bool);
void Rotation(Image*, Image*, float, int, bool);
void CalculateHistograms(Image*, bool);


// Reads pixels from source image with a linear interpolation
// avoiding (almost) aliasing.

int Interpolation(Image* in, float x, float y, int rgb, bool type)
{
	int xi=(int)x;
	int yi=(int)y;

	x=0;
	y=0;

	int dimX = in->getX();
	int dimY = in->getY();

	// check that coordinates are inside the image
	// clamping otherwise
	if (xi < 0)	   	xi = 0;
	if (xi > dimX - 1) 	xi = dimX - 1;
	if (yi < 0)	   	yi = 0;
	if (yi > dimY - 1)   yi = dimY - 1;
	
	int v1, v2, v3, v4;
	if (type == false) {
		// compute linear interpolation
		v1=in->GetBn(xi + (dimX * yi));
		v2=in->GetBn(xi + 1 + (dimX * yi));
		v3=in->GetBn(xi + (dimX * (yi + 1)));
		v4=in->GetBn(xi + 1 + (dimX * (yi + 1)));
	}
	else {
		// compute linear interpolation
		v1=in->GetRgb(xi + (dimX * yi), rgb);
		v2=in->GetRgb(xi + 1 + (dimX * yi), rgb);
		v3=in->GetRgb(xi + (dimX * (yi + 1)), rgb);
		v4=in->GetRgb(xi + 1 + (dimX * (yi + 1)), rgb);
	}

	return (int)((v1 * (1-x) * (1-y)) + (v2 * x * (1-y)) + (v3 * (1-x) * y) + (v4 * x * y));
}


// * Zoom filter *
//
// Based on 'zoom' ('+' or '-'), destination image will be
// zoomed in or out. 'percent' is the value of zooming.

void Zoom(Image* in, Image* out, char zoom, int percent, bool type)
{
	int dimX_new, dimY_new, pixel;
	float n = 100.0/(100-percent);

	if (zoom == '+') {
		dimX_new=(int)(in->getX()*n);
		dimY_new=(int)(in->getY()*n);
	}
	
	if (zoom == '-') {
		dimX_new = (int)(in->getX()/n);
		dimY_new = (int)(in->getY()/n);
	}

	// delete old image and create a new one
	// because size is changed
	delete out;
	out = new Image(dimX_new ,dimY_new);

	for (int y = 0; y < dimY_new; y++)
		for (int x = 0; x < dimX_new; x++) {
			if (type == false) {
				if (zoom == '+')
					pixel = Interpolation(in, x/n, y/n, 0, type);
				if (zoom == '-')
					pixel = Interpolation(in, x*n, y*n, 0, type);
				if (pixel < 0)	pixel=0;
				if (pixel > 255)  pixel=255;
				out->SetBn(x+(y*dimX_new), pixel);
			}
			else{
				for (int z = 0; z < 3; z++) {
					if (zoom == '+')
						pixel = Interpolation(in, x/n, y/n, z, type);
					if (zoom == '-')
						pixel = Interpolation(in, x*n, y*n, z, type);
					if (pixel < 0)	pixel=0;
					if (pixel > 255)  pixel=255;
					out->SetRgb(x + (y * dimX_new), z, pixel);
				}
			}
		}
}


// * Resize filter *
//
// Based on 'width' destination image will be proportionally
// scaled to the given width.

void Resize(Image* in, Image* out, int width, bool type)
{
	int dimX_new, dimY_new, pixel;
	float n = (float)width/(float)in->getX();

	dimX_new = width;
	dimY_new = (int)(in->getY() * n);

	// delete old image and create a new one
	// because size is changed
	delete out;
	out = new Image(dimX_new, dimY_new);

	for (int y = 0; y < dimY_new; y++)
		for (int x = 0; x < dimX_new; x++) {
			if (type == false) {
				pixel = Interpolation(in, x/n, y/n, 0, type);
				if (pixel < 0)	pixel=0;
				if (pixel > 255)  pixel=255;
				out->SetBn(x + (y * dimX_new), pixel);
			}
			else{
				for (int z = 0; z < 3; z++) {
					pixel = Interpolation(in, x/n, y/n, z, type);
					if (pixel < 0)	pixel=0;
					if (pixel > 255)  pixel=255;
					out->SetRgb(x + (y * dimX_new), z, pixel);
				}
			}
		}
}


// * Rotation filter *
//
// Based on '_angle' the destination image will results
// rotated clockwise of the given degree number.

void Rotation(Image* in, Image* out, float _angle, bool type)
{
	if (type == false) {
		int dimX = in->getX();
		int dimY = in->getY();

		// compute radiant angle
		const float angle = (_angle * 2 * 3.14) / 360;
		for (int y = 0; y < dimY; y++)
			for (int x = 0; x < dimX; x++) {
				
				// compute coordinates via sin and cos
				float x_o = (cos(angle) * (x-dimX/2)) - (sin(angle) * (y-dimY/2));
				float y_o = (sin(angle) * (x-dimX/2)) + (cos(angle) * (y-dimY/2));
				
				// read pixel from center, pivoting based on the center of the image
				int pixel = Interpolation(in, x_o + dimX/2, y_o + dimY/2, 0, type);
				if (pixel < 0)   pixel = 0;
				if (pixel > 255) pixel = 255;
				out->SetBn(x + (y * dimX), pixel);
			}
	}
	else{
		int dimX = out->getX();
		int dimY = out->getY();
		
		// compute radiant angle
		const float angle = (_angle * 2 * 3.14) / 360;
		for (int y = 0; y < dimY; y++)
			for (int x = 0; x < dimX; x++)
				for (int z = 0; z < 3; z++) {
					
					// compute coordinates via sin and cos
					float x_o = (cos(angle) * (x-dimX/2)) - (sin(angle) * (y-dimY/2));
					float y_o = (sin(angle) * (x-dimX/2)) + (cos(angle) * (y-dimY/2));
					
					// read pixel from center, pivoting based on the center of the image
					int pixel = Interpolation(in, x_o+dimX/2, y_o+dimY/2, z, type);
					if (pixel < 0)   pixel = 0;
					if (pixel > 255) pixel = 255;
					out->SetRgb(x + (y * dimX), z, pixel);
				}
	}
}


// Compute the RGB channels and brightness histograms

void CalculateHistograms(Image* in, bool type)
{
	int dimX = in->getX();
	int dimY = in->getY();
	
	// works on grayscale hystogram
	if (type  ==  false) {
		for (int i = 0; i < 256; i++) {
			in->setIsto(i, 0.0, '0');
		}
		
		// count the pixel number per birghtness value
		for (int i = 0; i < dimX * dimY; i++) {
			in->setIsto(in->GetBn(i), in->getIsto(in->GetBn(i), 0) + 1.0, 0);
		}
	
		// normalization to 1
		for (int i = 0; i < 256; i++) {
			in->setIsto(i, (in->getIsto(i, '0')/(dimX * dimY)), '0');
		}
	}
	
	// works on RGB hystogram
	else {
		for (int i = 0; i < 256; i++) {
			in->setIsto(i, 0.0, 'R');
			in->setIsto(i, 0.0, 'G');
			in->setIsto(i, 0.0, 'B');
		}

		// count the pixel number per birghtness value
		for (int i = 0; i < dimX * dimY; i++) {
			in->setIsto(in->GetRgb(i, 0), in->getIsto(in->GetRgb(i, 0), 'R') + 1.0, 'R');
			in->setIsto(in->GetRgb(i, 1), in->getIsto(in->GetRgb(i, 1), 'G') + 1.0, 'G');
			in->setIsto(in->GetRgb(i, 2), in->getIsto(in->GetRgb(i, 2), 'B') + 1.0, 'B');
		}

		// normalization to 1
		for (int i = 0; i < 256; i++) {
			in->setIsto(i, (in->getIsto(i, 'R')/(dimX * dimY)), 'R');
			in->setIsto(i, (in->getIsto(i, 'G')/(dimX * dimY)), 'G');
			in->setIsto(i, (in->getIsto(i, 'B')/(dimX * dimY)), 'B');
		}
	}	
	
}

#endif
