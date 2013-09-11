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
#include "FT.h"
#include <math.h>
using namespace std;

void Magnitude(double* matRe, double* matIm, int dimX, int dimY)
{
	Image* magnitude = new Image(dimX, dimY);
	float q;

	for (int h = 0; h < (dimX * dimY); h++) {
		q = sqrt((matRe[h]*matRe[h]) + (matIm[h]*matIm[h]));
		q*=255;
		if (q >255) q = 255;
		if (q<0) q = 0;
		magnitude->SetBn(h, (int)q);
	}

	magnitude->SaveImage("magnitude.raw", 0);
	
	cout << "\nSaved file: \t" << "magnitude.raw" << endl << "Type: \t\tgrayscale (range 0-255)" << endl;
	cout << "Image size: \t" << dimX << "x" << dimY << " pixel\n";
	cout << "File size: \t\t" << dimX * dimY << " bytes" << endl;
	delete magnitude;
}

void FT(double* matrix, int dimX, int dimY, double* matRe, double* matIm, int mode)
{
	double Buffer[dimX * dimY];
	double CoeffCos[dimX];
	double CoeffSin[dimX];
	
	int x, y, u, v;
	double R, I, B;
	
	// Forward FT
	if (mode == 1) {
		
		// create image in Buffer[] (alternate sign pixel)
		for (y = 0; y < dimY; y++) {
			for (x = 0; x < dimX; x++) {
				R = matrix[x + (y * dimX)];
				if (((x + y) & 1) == false) Buffer[x + (y * dimX)] = R;
				else Buffer[x + (y * dimX)] = -R;
			}
		}

		//   j theta
		// e		 = cos (theta) + j sin(theta)
		// sin and cos tables
		for (x = 0; x < dimX; x++) {
			CoeffCos[x] = cos(x * 6.28328 / dimX);
			CoeffSin[x] = sin(x * 6.28328 / dimX);
		}
		
		for (v = 0; v < dimY; v++) {
			if ((v % 32) == 31) cout << "\nworking... FT forward... row: " << v << endl;
			for (u = 0; u < dimX; u++) {
				R = 0.0f;
				I = 0.0f;
				for (y = 0; y < dimY; y++) {
					for (x = 0; x < dimX; x++) {
						B = Buffer[x + (y*dimX)];
						
						// sums where B is 'f(x)
						R+= B * CoeffCos[((u*x) + (v*y))%dimX];
						I-= B * CoeffSin[((u*x) + (v*y))%dimX];
					}
				}
				matRe[u+(v*dimX)] = R / (dimX * dimY);
				matIm[u+(v*dimX)] = I / (dimX * dimY);
			}
		}
	}

	// Backward FT
	if (mode == -1) {
		cout << endl;

		for (x = 0; x < dimX; x++) {
			CoeffCos[x] = cos(x * 6.28328 / dimX);
			CoeffSin[x] = sin(x * 6.28328 / dimX);
		}

		for (v = 0; v < dimY; v++) {
			if ((v % 32) == 31) cout << "\nworking... FT backward... row: " << v << endl;
			for (u = 0; u < dimX; u++) {
				B = 0.0f;
				for (y = 0; y < dimY; y++) {
					for (x = 0; x < dimX; x++) {
						R = matRe[x + (y * dimX)];
						I = matIm[x + (y * dimX)];
						B += (R * CoeffCos[((u * x) + (v * y)) % dimX]) - (I * CoeffSin[((u * x) + (v * y)) % dimX]);
					}
				}
				Buffer[u+(v*dimX)]= B;
			}
		}
	
		for (y = 0; y < dimY; y++) {
			for (x = 0; x < dimX; x++) {
				if (((x + y) & 1) == 0) u = (int)Buffer[x + (y * dimX)];
				else u = (int) - Buffer[x + (y * dimX)];
				if (u < 0) u =0;
				if (u > 255) u = 255;
				matrix[x + (y * dimX)] = u;
			}
		}
	}
}
