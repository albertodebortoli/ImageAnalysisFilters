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

#ifndef FILTERSPUNCTUAL_H
#define FILTERSPUNCTUAL_H

#include"Image.h"
using namespace std;

void Equalize(Image*, Image*, bool);
void Brightness(Image*, Image*, float, bool);
void AutoContrast(Image*, Image*, bool);
void Gamma(Image*, Image*, float, bool);
void Channel(Image*, Image*, float, char);
void Mirror(Image*, Image*, char, bool);  
void Pixelize(Image*, Image*, int, bool);
void Grayscale(Image*, Image*);
void Invert(Image*, Image*);
		 

// * Equalize filter *
//
// Spread the hystogram of gray over all the range of color space. 
// Find the value to balance the median of the hystogram of gray
// bringing it to the center. Considering the brightness average
// of 0.5, compute 0.5 = sum(i = 0; l) ist(i) finding l.
// Applying on each pixel the following formula:
// l_0 = sum(i = 0; l_i) ist(i)

void Equalize(Image* in, Image* out, bool type)
{
	// between 0 and 1
	float v;

	// fill the hystogram with values between 0 and 1
	CalculateHistograms(in, type);
	
	// work on grayscale image
	if (type == false) {
		for (int i = 0; i < (in->getX() * in->getY()); i++) {
			
			// between 0 and 1
			v = 0.0;
			for (int j = 0; j <= in->GetBn(i); j++) {
				v += in->getIsto(j, 0);
			}
			v *= 255.0;
			out->SetBn(i, (unsigned char)v);
		}
	}
	
	// work on color image (RGB)
	// won't alterate global tint using average on RGB values
	else {
		for (int i = 0; i < (in->getX() * in->getY()); i++) {
			v = 0.0;
			for (int j = 0; j <= in->GetRgb(i, 0); j++) {
				v += ((in->getIsto(j, 'R') + in->getIsto(j, 'G') + in->getIsto(j, 'B')) / 3);
			}
			v *= 255.0;
			out->SetRgb(i, 0, (unsigned char)v);
			v = 0.0;
			for (int j = 0; j <= in->GetRgb(i, 1); j++) {
				v += ((in->getIsto(j, 'R') + in->getIsto(j, 'G') + in->getIsto(j, 'B')) / 3);
			}
			v *= 255.0;
			out->SetRgb(i, 1, (unsigned char)v);
			v = 0.0;
			for (int j = 0; j <= in->GetRgb(i, 2); j++) {
				v += ((in->getIsto(j, 'R') + in->getIsto(j, 'G') + in->getIsto(j, 'B')) / 3);
			}
			v *= 255.0;
			out->SetRgb(i, 2, (unsigned char)v);
		}
	}
}


// * Brightness filter *
//
// Apply the filter both to grayscale image and color image (RGB)
// using the formula RGBout = RGBin * (BNout/BNin)
// 'level' is (BNout/BNin) i.e. (out->GetBn(i) / in->GetBn(i))

void Brightness(Image* in, Image* out, float level, bool type)
{
	float value;

	// increase grayscale brightness
	if (type == false)
	for (int i = 0; i < (in->getX() * in->getY()); i++) {
		value = in->GetBn(i);
		value *= level;
		if (value > 255.0f) value = 255.0f;
		if (value < 0.0f) value = 0.0f;
		out->SetBn(i, (int)(value + 0.5f)); // +0.5f trunc to int
	}

	// increase color brightness
	if (type == true)
	for (int i = 0; i < (in->getX() * in->getY()); i++)
		for (int j = 0; j < 3; j++) {
			value = in->GetRgb(i, j);
			value *= level;
			if (value > 255.0f) value = 255.0f;
			if (value < 0.0f) value = 0.0f;
			out->SetRgb(i, j, (int)value);
		}
}


// * Auto Contrast filter *
//
// Brighten up bright areas and darken dark one
// Stretch the grayscale hystogram to bounds using the formula:
// Lout = a + b * Lin
// math system is Lout = Loutmax (i.e. 255) and Lout = Loutmin (i.e. 0)
// with Lin = Linmin and Lin = Linmax results in
// a = 255 / (Linmax - Linmin)
// b = (255 * Linmin) / (Linmax - Linmin)

void AutoContrast(Image* in, Image* out, bool type)
{
	int value;
	int min, max;
	
	// compute minimum and maximum values in the hystogram
	// (good both for grayscale and color)
	min = in->GetBn(0);
	max = in->GetBn(0);

	for (int i = 0; i < (in->getX()*in->getY()); i++) {
		if (in->GetBn(i) < min)	min = in->GetBn(i);
		if (in->GetBn(i) > max)	max = in->GetBn(i);
	}

	// apply to grayscale image
	if (type == false)
	for (int i = 0; i < (in->getX()*in->getY()); i++) {
		value = in->GetBn(i);
		value = (value * 255) / (max - min);
		value -= (255 * min) / (max - min);
		if (value > 255) value = 255;
		if (value < 0) value = 0;
		out->SetBn(i, value);
	}
	
	// apply to color image (RGB)
	if (type == true)
	for (int i = 0; i < (in->getX() * in->getY()); i++)
		for (int j = 0; j < 3; j++) {
			value = in->GetRgb(i, j);
			value = (value * 255) / (max - min);
			value -= (255 * min) / (max - min);
			if (value > 255) value = 255;
			if (value < 0) value = 0;
			out->SetRgb(i, j, value);
		}
}


// * Gamma Correction filter *
// 
// Stand out intermediate areas working on average density using the formula
// Lout = Lin^esp (nonlinear)

void Gamma(Image* in, Image* out, float esp, bool type)
{
	// apply filter to grayscale image
	if (type == false)
	for (int i = 0; i < (out->getX()*out->getY()); i++) {
		float value = in->GetBn(i);
		value = pow(value / 255.0f, esp);
		value *= 255.0f;
		if (value > 255.0f) value = 255.0f;
		if (value < 0.0f) value = 0.0f;
		out->SetBn(i, (int)(value + 0.5f)); // +0.5f trunc to int
	}

	// apply filter to color image
	if (type == true)
	for (int i = 0; i < (out->getX()*out->getY()); i++)
		for (int j = 0; j < 3; j++) {
			float value = in->GetRgb(i, j);
			value = pow(value / 255.0f, esp);
			value *= 255.0f;
			if (value > 255.0f) value = 255.0f;
			if (value < 0.0f) value = 0.0f;
			out->SetRgb(i, j, (int)(value + 0.5f)); // +0.5f trunc to int
		}
}


// * Gamma Channel Control filter *
//
// Stand out intermediate areas working on average density using the formula
// Lout = Lin^esp (nonlinear)

void Channel(Image* in, Image* out, float esp, char comp)
{
	// apply filter to color image on the given component
	int componente;
	switch (comp) {
		case 'R' : componente = 0; break;
		case 'G' : componente = 1; break;
		case 'B' : componente = 2; break;
		default : componente = 2;
	}

	for (int i = 0; i < (out->getX() * out->getY()); i++) {
		for (int j = 0; j < 3; j++)
			out->SetRgb(i, j, in->GetRgb(i, j));
		
		float value = in->GetRgb(i, componente);
		value = pow(value / 255.0f, esp);
		value *= 255.0f;
		if (value > 255.0f) value = 255.0f;
		if (value < 0.0f) value = 0.0f;
		out->SetRgb(i, componente, (int)(value + 0.5f)); // +0.5f trunc to int
	}
}


// * Mirror filter *
//
// Reflect the image
// horizontally: 'coord' == 'X'
// vertically: 'coord' == 'Y'

void Mirror(Image* in, Image* out, char coord, bool type)
{
	int dimx = out->getX();
	int dimy = out->getY();
	
	int x_new, y_new, value, pixel;
	
	// apply filter to grayscale image
	if (type == false) {
		if (coord == 'X')
			for (int x = 0; x < dimx; x++)
				for (int y = 0; y < dimy; y++) {
					x_new = dimx-1-x;
					value = in->GetBn(x_new + (y * dimx));
					out->SetBn(x + (y * dimx), value);
				}
				
		if (coord == 'Y')
			for (int x = 0; x < dimx; x++)
				for (int y = 0; y < dimy; y++) {
					y_new = dimy-1-y;
					value = in->GetBn(x + (y_new * dimx));
					out->SetBn(x + (y * dimx), value);
				}
	}
	
	// apply filter to color image (RGB)
	if (type == true) {
		if (coord == 'X')
			for (int x = 0; x < dimx; x++)
				for (int y = 0; y < dimy; y++) {
					x_new = dimx - 1 - x;
					for (int z = 0; z < 3; z++) {
						pixel = in->GetRgb(x_new + (y * dimx), z);
						if (pixel < 0) pixel = 0;
						if (pixel > 255) pixel = 255;
						out->SetRgb(x + (y * dimx), z, pixel);
					}
				}

	if (coord == 'Y')
		for (int x = 0; x < dimx; x++)
			for (int y = 0; y < dimy; y++) {
				y_new = dimy - 1 - y;
				for (int z = 0; z < 3; z++) {
					pixel = in->GetRgb(x + (y_new * dimx), z);
					if (pixel < 0) pixel = 0;
					if (pixel > 255) pixel = 255;
					out->SetRgb(x + (y * dimx), z, pixel);
				}
			}
	}		  
}


// * Pixelazer filter *
//
// Apply a filter to 'pixelate' the image that results
// in a loss of details,
// 'dim' is the size of the macro pixel desired

void Pixelize(Image* in, Image* out, int dim, bool type)
{
	int dimX = in->getX();
	int dimY = in->getY();
	
	if (type == false) {
		
		int init = in->GetBn(0);
		init = in->GetBn(0);
	
		// pixelate on X axis
		for (int y = 0; y < dimY; y++)
			for (int x = 0; x < dimX; x++) {
				if (x % dim == 0) init = in->GetBn((y * dimX) + x);
				out->SetBn((y * dimX) + x, init);
			}
			
		// pixelate on Y axis
		for (int x = 0; x < dimX; x++)
			for (int y = 0; y < dimY; y++) {
				if (y % dim == 0) init = out->GetBn((y * dimX) + x);
				out->SetBn((y * dimX) + x, init);
			}
	}  
	
	if (type == true) {	
		
		int RGB[3] = {in->GetRgb(0, 0), in->GetRgb(0, 1), in->GetRgb(0, 2)};
		
		// pixelate on X axis
		for (int y = 0; y < dimY; y++)
			for (int x = 0; x < dimX; x++) {
				if (x % dim  ==  0) {
					RGB[0] = in->GetRgb((y * dimX) + x, 0);
					RGB[1] = in->GetRgb((y * dimX) + x, 1);
					RGB[2] = in->GetRgb((y * dimX) + x, 2);
				}
				out->SetRgb((y * dimX) + x, 0, RGB[0]);
				out->SetRgb((y * dimX) + x, 1, RGB[1]);
				out->SetRgb((y * dimX) + x, 2, RGB[2]);
			}
		
		// pixelate on Y axis
		for (int x = 0; x < dimX; x++)
			for (int y = 0; y < dimY; y++) {
				if (y % dim  ==  0) {
					RGB[0] = out->GetRgb((y * dimX) + x, 0);
					RGB[1] = out->GetRgb((y * dimX) + x, 1);
					RGB[2] = out->GetRgb((y * dimX) + x, 2);
				}
				out->SetRgb((y * dimX) + x, 0, RGB[0]);
				out->SetRgb((y * dimX) + x, 1, RGB[1]);
				out->SetRgb((y * dimX) + x, 2, RGB[2]);
			}
	}
}


// * Grayscale filter *
//
// If the image is RGB interleaved, converts it in 
// RGB interleaved but with grayscale values 

void Grayscale(Image* in, Image* out)
{
	for (int i = 0; i < (in->getX() * in->getY()); i++) {
		for (int j = 0; j < 3; j++)
			out->SetRgb(i, j, in->GetBn(i));
	}	
}


// * Invert filter *
// 
// Apply the filter using the formula
// out = 255 - in

void Invert(Image* in, Image* out, bool type)
{
	int value;
	
	// invert grayscale image values
	if (type == false)
	for (int i = 0; i < (in->getX() * in->getY()); i++) {
		value = in->GetBn(i);
		value = 255 - value;
		if (value > 255) value = 255;
		if (value < 0) value = 0;
		out->SetBn(i, value); // +0.5f trunc to int
	}

	// invert color image values
	if (type == true)
	for (int i = 0; i < (in->getX() * in->getY()); i++) {
		for (int j = 0; j < 3; j++) {
			value = in->GetRgb(i, j);
			value = 255 - value;
			if (value > 255) value = 255;
			if (value < 0) value = 0;
			out->SetRgb(i, j, value);
		}
	}
}

#endif
