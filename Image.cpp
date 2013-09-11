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

Image::Image(int x, int y)
{
	bn = new unsigned char[x*y];
	rgb = new unsigned char[x*y*3];
	hysto = new float[256];
	hystoR = new float[256];
	hystoG = new float[256];
	hystoB = new float[256];
	dimX = x;
	dimY = y;
	for (int i = 0; i < 256; i++) {
		hysto[i] = 0.0;
		hystoR[i] = 0.0;
		hystoG[i] = 0.0;
		hystoB[i] = 0.0;
	}
}

// For each pixel in the color image (RGB) compute the
// average and set the value in the grayscale image
void Image::FitBN()
{
	int pixel = 0;
	for (int i = 0; i < dimX * dimY; i++) {
		pixel = GetRgb(i, 0) + GetRgb(i, 1) + GetRgb(i, 2);
		SetBn(i, pixel/3);
	}
}

// For each pixel in the grayscale image compute the average
// and set the value in the color image (RGB)
void Image::FitRGB()
{
	for (int i = 0; i < dimX * dimY; i++) {
		for (int j = 0; j < 3; j++)
			SetRgb(i, j, GetBn(i));
	}
}

// Load the image from the given path
// 'type' == 0 means raw grayscale mode (1 byte per pixel)
// 'type' == 1 means raw color mode interleaved (3 bytes per pixel)
void Image::LoadImage(char* nome, int type)
{
	FILE* fhan;
	if (!(fhan = fopen(nome, "rb"))) {
		cout << "Can't read source file\n";
		exit(0);
	}
	
	switch (type) {
		case 0  :   fread(bn, dimX * dimY, 1, fhan);
					// load grayscale image, filling RGB part
					this->FitRGB();
					break;
		case 1  :   fread(rgb, dimX * dimY * 3, 1, fhan);
					// load RGB image, filling grayscale part
					this->FitBN();
					break;
		default :   break;
	}

	fclose(fhan);
}

// Save the image on disk to the given path
// 'type' == 0 means raw grayscale mode (1 byte per pixel)
// 'type' == 1 means raw color mode interleaved (3 bytes per pixel)
void Image::SaveImage(char* nome, int type)
{
	FILE* fhan;
	if (!(fhan = fopen(nome, "wb"))) {
		cout << "Impossibile scrivere nel file destinazione\n";
		exit(0);
	}
	switch (type) {
		case 0  :   fwrite(bn, dimX * dimY, 1, fhan); break;
		case 1  :   fwrite(rgb, dimX * dimY * 3, 1, fhan); break;
		default :   break;
	}
	fclose(fhan);
}

Image::~Image()
{
	delete bn;
	delete rgb;
	delete hysto;
	delete hystoR;
	delete hystoG;
	delete hystoB;
	dimX = 0;
	dimY = 0;
}
