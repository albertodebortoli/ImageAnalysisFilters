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

#ifndef FILTERSCONVOLUTION_H
#define FILTERSCONVOLUTION_H

#include"Image.h"
using namespace std;

void Smooth(Image*, Image*, int, bool);
int* FindAverage(Image*, int, int, int, int*, bool);
void NoiseRemoval(Image*, Image*, int, bool);
int* FindMedian(Image*, int, int, int, int*, bool);
void Convolution(Image*, Image*, char, bool);
int* SharpBlurOut(Image*, int, int, int*, int, int*, bool);


// * Smooth filter *
//
// Use FindAverage to compute the average of the range of pixel,
// 'kernel_dim' is the kernel dimension and results in a stronger
// or weaker filter effect.

void Smooth(Image* in, Image* out, int kernel_dim, bool type)
{
	int dimX = in->getX();
	int dimY = in->getY();
	int media[3];
	
	for (int i = 0; i < dimX; i++)
		for (int j = 0; j < dimY; j++)

			// apply filter on grayscale image
			if (type == false) out->SetBn(i+(j*dimX), *FindAverage(in, i, j, kernel_dim, media, type));
			
			// apply filter on color image (RGB)
			else {
				int* rgb = FindAverage(in, i, j, kernel_dim, media, type);
				out->SetRgb(i+(j*dimX), 0, rgb[0]);
				out->SetRgb(i+(j*dimX), 1, rgb[1]);
				out->SetRgb(i+(j*dimX), 2, rgb[2]);
			}
}


// Find the average of the pixels inside the kernel
// 'kernel_dim' is the kernel dimension and results in a stronger
// or weaker filter effect.

int* FindAverage(Image* in, int x, int y, int kernel_dim, int* average, bool type)
{
	int offset = (int)((kernel_dim-1)/2);
	int kernel_dim_total = kernel_dim * kernel_dim;
	int Kernel[kernel_dim_total];
	int KernelRGB[kernel_dim_total][3];
	int x_o, y_o;
	int dimX = in->getX();
	int dimY = in->getY();

	for (y_o = -offset; y_o <= offset; y_o++) {
		if (!(y + y_o < 0 || y + y_o >= dimY))
		for (x_o = -offset; x_o <= offset; x_o++) {
			if (!(x + x_o < 0 || x + x_o >= dimX)) {
				
				// apply filter to the grayscale image
				if (type == 0) {
					Kernel[x_o+offset+((y_o+offset)*kernel_dim)] = in->GetBn((x+x_o)+((y+y_o)*dimX));
				}
				
				// apply filter to the color image
				else {
					KernelRGB[x_o+offset+((y_o+offset)*kernel_dim)][0] = in->GetRgb((x+x_o)+((y+y_o)*dimX), 0);
					KernelRGB[x_o+offset+((y_o+offset)*kernel_dim)][1] = in->GetRgb((x+x_o)+((y+y_o)*dimX), 1);
					KernelRGB[x_o+offset+((y_o+offset)*kernel_dim)][2] = in->GetRgb((x+x_o)+((y+y_o)*dimX), 2);
				}
			}
		}
	}

	// compute the average of the grayscale image
	if (type == false) {
		average[0] = 0;
		for (int i = 0; i < kernel_dim_total; i++)
			average[0] += Kernel[i];

		average[0] /= (kernel_dim_total);
		return average;
	}
	
	// compute the average of the color image (RGB)
	else {
		int tempR = 0;
		for (int i = 0; i < kernel_dim_total; i++)
			tempR += KernelRGB[i][0];

		tempR /= (kernel_dim_total);

		int tempG = 0;
		for (int i = 0; i < kernel_dim_total; i++)
			tempG += KernelRGB[i][1];

		tempG /= (kernel_dim_total);
		
		int tempB = 0;
		for (int i = 0; i < kernel_dim_total; i++)
			tempB += KernelRGB[i][2];

		tempB /= (kernel_dim_total);

		average[0] = tempR;
		average[1] = tempG;
		average[2] = tempB;
	
		return average;
	}
}


// * Remove noise filter *
//
// Use FindMedian to find median around the given pixel. 
// 'kernel_dim' is the kernel dimension and results in a stronger
// or weaker filter effect.

void NoiseRemoval(Image* in, Image* out, int kernel_dim, bool type)
{
	int dimX = in->getX();
	int dimY = in->getY();
	int median[3];

	for (int i = 0; i < dimX; i++)
		for (int j = 0; j < dimY; j++) {
			
			// apply filter to the grayscale image
			if (type == false) {
				out->SetBn(i+(j*dimX), *FindMedian(in, i, j, kernel_dim, median, type));
			}
			
			// apply filter to the color image (RGB)
			else {
				int* rgb = FindMedian(in, i, j, kernel_dim, median, type);
				out->SetRgb(i+(j*dimX), 0, rgb[0]);
				out->SetRgb(i+(j*dimX), 1, rgb[1]);
				out->SetRgb(i+(j*dimX), 2, rgb[2]);
			}
		}
}


// Find the median of the pixels inside the kernel
// 'kernel_dim' is the kernel dimension and results in a stronger
// or weaker filter effect.

int* FindMedian(Image* in, int x, int y, int kernel_dim, int* median, bool type)
{
	int offset = (int)((kernel_dim-1)/2);
	int kernel_dim_total = kernel_dim * kernel_dim;
	int Kernel[kernel_dim_total];
	int KernelRGB[kernel_dim_total][3];
	int x_o, y_o;
	int dimX = in->getX();
	int dimY = in->getY();

	for (y_o = -offset; y_o <= offset; y_o++) {
		if (!(y + y_o < 0 || y + y_o >= dimY))
		for (x_o = -offset; x_o <= offset; x_o++) {
			if (!(x + x_o < 0 || x + x_o >= dimX))
				
				// apply the filter to the grayscale image
				if (type == false) Kernel[x_o + offset + ((y_o + offset) * kernel_dim)] = in->GetBn((x + x_o)+((y + y_o) * dimX));
				
				// apply the filter to the color image (RGB)
				else {
					KernelRGB[x_o+offset+((y_o+offset)*kernel_dim)][0] = in->GetRgb((x+x_o)+((y+y_o)*dimX), 0);
					KernelRGB[x_o+offset+((y_o+offset)*kernel_dim)][1] = in->GetRgb((x+x_o)+((y+y_o)*dimX), 1);
					KernelRGB[x_o+offset+((y_o+offset)*kernel_dim)][2] = in->GetRgb((x+x_o)+((y+y_o)*dimX), 2);
				}
			}
		}
	
	// sort kernel array to get the median in central position
	// using Insertion Sort
	
	// sort the kernel for grayscale image
	if (type == false) {
		for (int i = 1; i < kernel_dim_total; i++) {
			int j = i;
			int t = Kernel[j];
			while (j > 0 && Kernel[j-1] > t) {
				Kernel[j] = Kernel[j-1];
				j--;
			}
			Kernel[j] = t;
		}
		
		median[0] = Kernel[(int)floor(kernel_dim_total/2)];
		return(median);
	}
	
	// sort the kernel for color image (RGB)
	else {
		for (int i = 1; i < kernel_dim_total; i++) {
			int j = i;
			int t = (KernelRGB[j][0] + KernelRGB[j][1] + KernelRGB[j][2]) /3;
			int t_[3] = {KernelRGB[j][0], KernelRGB[j][1], KernelRGB[j][2]};
			
			while (j > 0 && ((KernelRGB[j-1][0] + KernelRGB[j-1][1] + KernelRGB[j-1][2]) /3) > t) {
				KernelRGB[j][0] = KernelRGB[j-1][0];
				KernelRGB[j][1] = KernelRGB[j-1][1];
				KernelRGB[j][2] = KernelRGB[j-1][2];
				j--;
			}
			
			KernelRGB[j][0] = t_[0];
			KernelRGB[j][1] = t_[1];
			KernelRGB[j][2] = t_[2];
		}
		
		median[0] = KernelRGB[(int)floor(kernel_dim_total/2)][0];
		median[1] = KernelRGB[(int)floor(kernel_dim_total/2)][1];
		median[2] = KernelRGB[(int)floor(kernel_dim_total/2)][2];
		return median;
		//(KernelRGB[(int)floor(kernel_dim_total/2)]);
	}
}


// * Convolution filter *
//
// Use SharpBlurOut to compute the value for the output pixels.
// 'effect' can be:
// 'S' means Sharpen
// 'B' means Blur
// 'O' means Outliner

void Convolution(Image* in, Image* out, char effect, bool type)
{
	int dimX = in->getX();
	int dimY = in->getY();
	int value[3] = {0, 0, 0};

	int kernel[9];
	
	// set a kernel for the Sharpen filter
	if (effect == 'S') {
		kernel[0]=0;  kernel[1]=-1;   kernel[2]=0;
		kernel[3]=-1; kernel[4]=5;	kernel[5]=-1;
		kernel[6]=0;  kernel[7]=-1;   kernel[8]=0;
	}
	
	// set a kernel for the Blur filter
	if (effect == 'B') {
		kernel[0]=0;  kernel[1]=1;	kernel[2]=0;
		kernel[3]=1;  kernel[4]=2;	kernel[5]=1;
		kernel[6]=0;  kernel[7]=1;	kernel[8]=0;
	}
	
	// set a kernel for the Outliner filter
	if (effect == 'O') {
		kernel[0]=-1; kernel[1]=-1;   kernel[2]=-1;
		kernel[3]=-1; kernel[4]=8;	kernel[5]=-1;
		kernel[6]=-1; kernel[7]=-1;   kernel[8]=-1;
	}

	if (type == false) {
	for (int x = 0; x < dimX; x++)
		for (int y = 0; y < dimY; y++)
			out->SetBn(x + (y * dimX), *SharpBlurOut(in, x, y, kernel, 9, value, type));
	}
	else {
		for (int x = 0; x < dimX; x++)
			for (int y = 0; y < dimY; y++) {
				int* rgb = SharpBlurOut(in, x, y, kernel, 9, value, type);
				out->SetRgb(x+(y*dimX), 0, rgb[0]);
				out->SetRgb(x+(y*dimX), 1, rgb[1]);
				out->SetRgb(x+(y*dimX), 2, rgb[2]);
			}
	}
		
}


// Apply different filter based on the given kernel
// 'kernel' is the kernel dimension and results in a stronger
// or weaker filter effect.

int* SharpBlurOut(Image* in, int x, int y, int* kernel, int lunghezza, int* value, bool type)
{
	int kernel_dim = (int)sqrt(lunghezza);
	int offset = (int)((kernel_dim-1)/2);
	int dimX = in->getX();
	int dimY = in->getY();

	// compute the number of values not 0 in the kernel
	int scala = 0;
	for (int i = 0; i < lunghezza; i++)
		if (kernel[i] != 0) scala++;

	int x_o, y_o;
	int a;
	int pRGB;
	
	// use the kernel to apply the filter to the grayscale image
	if (type == false) {
		for (y_o = -offset; y_o <= offset; y_o++) {
			if (!(y + y_o < 0 || y + y_o >= dimY))
			for (x_o = -offset; x_o <= offset; x_o++) {
				if (!(x + x_o < 0 || x + x_o >= dimX)) {
					a = in->GetBn(x + x_o + ((y + y_o) * dimX));
					value[0] += (a * kernel[x_o + offset + ((y_o + offset) * kernel_dim)]);
				}
			}
		}

		value[0] /= scala;
		if (value[0] < 0)   value[0] = 0;
		if (value[0] > 255) value[0] = 255;
		return value;
	}
	
	// use the kernel to apply the filter to the color image (RGB)
	else {
		for (int z = 0; z < 3; z++) {
			pRGB = 0;
			for (y_o = -offset; y_o <= offset; y_o++) {
				if (!(y + y_o < 0 || y + y_o >= dimY))
				for (x_o = -offset; x_o <= offset; x_o++) {
					if (!(x + x_o < 0 || x + x_o >= dimX)) {
						a = in->GetRgb(x + x_o + ((y + y_o) * dimX), z);
						pRGB += (a * kernel[x_o + offset + ((y_o + offset) * kernel_dim)]);
					}
				}
			}
			pRGB/= scala;
			if (pRGB < 0)   pRGB = 0;
			if (pRGB > 255) pRGB = 255;
			value[z] = (int)pRGB;
		}

		return value;
	}
}

#endif
