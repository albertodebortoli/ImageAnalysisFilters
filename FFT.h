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

#ifndef FFT_H
#define FFT_H

#include"Image.h"
using namespace std;

void AdjustSizeToPower2(Image* in, int dimX, int dimY, int dimX_original, int dimY_original);
void SwapSpectrum(double* matRe, double* matIm, int dimX, int dimY);
void SwapQuadranti(double* origine, int dimX, int dimY);
int Powerof2(int n, int *m, int *twopm );
void FFT(int dir, int m, double *x, double *y);
void FFT2D(double* matRe, double* matIm, int nx, int ny, int dir);


// Scale the image to next power of 2 size via deep copy 
// Empty pixels are black.

void AdjustSizeToPower2(Image* in, int dimX, int dimY, int dimX_original, int dimY_original)
{
	Image* in2 = new Image(dimX, dimY);
	
	// empty new image
	for (int j = 0; j < (dimX * dimY); j++) {
		in2->SetBn(j, 0);
		for (int i = 0; i < 3; i++)
			in2->SetRgb(j, i, 0);
	}
	
	// copy 'in' in 'in2'
	for (int j = 0; j < dimY_original; j++)
		for (int k = 0; k < dimX_original; k++) {
			in2->SetBn(k + (j * dimX), in->GetBn(k + (j * dimX_original)));
			for (int i = 0; i < 3; i++)
				in2->SetRgb(k + (j * dimX), i, in->GetRgb(k + (j * dimX_original), i));
		}
	
	delete in;
	in = new Image(dimX, dimY);

	// copy back in 'in' the scaled image
	for (int j = 0; j < (dimX * dimY); j++) {
		in->SetBn(j, in2->GetBn(j));
		for (int i = 0; i < 3; i++)
			in->SetRgb(j, i, in2->GetRgb(j, i));
	}
	delete in2;
}


// Call SwapQuadranti on matrices matRe e matIm

void SwapSpectrum(double* matRe, double* matIm, int dimX, int dimY)
{
	SwapQuadranti(matRe, dimX, dimY);
	SwapQuadranti(matIm, dimX, dimY);	
}


// Work on spectrum (matRe e matIm) derived from FFT transform.
// Reorder areas in the correct way to apply filters.

/*
 ___ ___		  ___ ___
|1  |2  |		|4  |3  |
|___|___|		|___|___| 
|3  |4  |   -->  |2  |1  | 
|___|___|		|___|___| 

*/

void SwapQuadranti(double* origine, int dimX, int dimY)
{
	int half_dimY, half_dimX;
	half_dimY = dimY/2;
	half_dimX = dimX/2;

	double* temp = new double[dimX * dimY];

	for (int k = 0; k < (dimX * dimY); k++) {
		temp[k]=0;
	}

	for (int nr = 0; nr < half_dimY; nr++) {
		for (int nc = 0; nc < half_dimX; nc++)
			temp[nc + half_dimX + ((nr + half_dimY) * dimX)] = origine[nc + (nr * dimX)];
		for (int nc=half_dimX; nc<dimX; nc++)
			temp[nc - half_dimX + ((nr + half_dimY) * dimX)] = origine[nc + (nr * dimX)];
		}

	for (int nr = half_dimY; nr < dimY; nr++) {
		for (int nc = half_dimX; nc < dimX; nc++)
			temp[nc - half_dimX + ((nr - half_dimY) * dimX)] = origine[nc + (nr * dimX)];
		for (int nc = 0; nc < half_dimX; nc++)
			temp[nc + half_dimX + ((nr - half_dimY) * dimX)] = origine[nc + (nr * dimX)];
	}

	for (int i = 0; i < (dimX * dimY); i++)
		origine[i] = temp[i];
	
	delete temp;
}


// Compute the nearest power of 2 of 'n' such that
// twopm = 2**m <= n
// Return true if (2**m == n)

int Powerof2(int n, int* m, int* twopm)
{
   if (n <= 1) {
	  *m	 = 0;
	  *twopm = 1;
	  return false;
   }

   *m	 = 1;
   *twopm = 2;

   do {
	  (*m)++;
	  (*twopm) *= 2;
   } while (2 * (*twopm) <= n);

   return (*twopm != n) ? false : true;
}


// Perform 2D FFT in place, given 2 arrays 'matRe' and 'MatIm'
// for complex numbers.
// 'dir' == 1 for forward FFT
// 'dir' == -1 for backward FFT
// 'nx' and 'ny' are the matRe and matIm sizes 
// original code by Paul Bourke
// url: http://local.wasp.uwa.edu.au/~pbourke/other/dft/

void FFT2D(double* matRe, double* matIm, int nx, int ny, int dir)
{
   int i,j;
   int m, twopm;
   double* real;
   double* imag;

   // applica la trasformata prima alle righe
   real = new double[nx];
   imag = new double[nx];

   if (!Powerof2(nx, &m, &twopm)) {
		cout << "\nL'immagine non ha dimensioni potenze di 2\n";
		exit(0);
	}
	
   for (j = 0;j < ny; j++) {
	  for (i = 0; i < nx; i++) {
		 real[i] = matRe[i + (j * nx)];
		 imag[i] = matIm[i + (j * nx)];
	  }
	  FFT(dir, m, real, imag);
	  for (i = 0; i < nx; i++) {
		 matRe[i + (j * nx)] = real[i];
		 matIm[i + (j * nx)] = imag[i];
	  }
   }

   delete real;
   delete imag;

   // apply column transform
   real = new double[ny];
   imag = new double[ny];

   if (!Powerof2(ny, &m, &twopm)) {
		cout << "\nNL'immagine non ha dimensioni potenze di 2\n";
		exit(0);
	}
   for (i = 0;i < nx; i++) {
	  for (j = 0; j < ny; j++) {
		 real[j] = matRe[i + (j * nx)];
		 imag[j] = matIm[i + (j * nx)];
	  }
	  FFT(dir, m, real, imag);
	  for (j = 0; j < ny; j++) {
		 matRe[i + (j * nx)] = real[j];
		 matIm[i + (j * nx)] = imag[j];
	  }
   }
	
	// clamping for avoiding wrap-around effect
	if (dir == -1)
		for (i = 0; i < nx * ny; i++) {
			if (matRe[i] < 0)   matRe[i] = 0;
			if (matRe[i] > 255) matRe[i] = 255;
		}

   delete real;
   delete imag;
}

/*-------------------------------------------------------------------------
   This computes an in-place complex-to-complex FFT
   x and y are the real and imaginary arrays of 2^m points.
   dir =  1 gives forward transform
   dir = -1 gives reverse transform

	 Formula: forward
				  N-1
				  ---
			  1   \		  - j k 2 pi n / N
	  X(n) = ---   >   x(k) e					= forward transform
			  N   /								n=0..N-1
				  ---
				  k=0

	  Formula: reverse
				  N-1
				  ---
				  \		  j k 2 pi n / N
	  X(n) =	   >   x(k) e					= forward transform
				  /								n=0..N-1
				  ---
				  k=0
*/

void FFT(int dir, int m, double *x, double *y)
{
   long nn,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   nn = 1;
   for (i = 0; i < m; i++)
	  nn *= 2;

   /* Do the bit reversal */
   i2 = nn >> 1;
   j = 0;
   for (i = 0; i < nn-1; i++) {
	  if (i < j) {
		 tx = x[i];
		 ty = y[i];
		 x[i] = x[j];
		 y[i] = y[j];
		 x[j] = tx;
		 y[j] = ty;
	  }
	  k = i2;
	  while (k <= j) {
		 j -= k;
		 k >>= 1;
	  }
	  j += k;
   }

   /* Compute the FFT */
   c1 = -1.0;
   c2 = 0.0;
   l2 = 1;
   for (l = 0; l < m; l++) {
	  l1 = l2;
	  l2 <<= 1;
	  u1 = 1.0;
	  u2 = 0.0;
	  for (j = 0; j < l1; j++) {
		 for (i = j; i < nn; i+=l2) {
			i1 = i + l1;
			t1 = u1 * x[i1] - u2 * y[i1];
			t2 = u1 * y[i1] + u2 * x[i1];
			x[i1] = x[i] - t1;
			y[i1] = y[i] - t2;
			x[i] += t1;
			y[i] += t2;
		 }
		 z =  u1 * c1 - u2 * c2;
		 u2 = u1 * c2 + u2 * c1;
		 u1 = z;
	  }
	  c2 = sqrt((1.0 - c1) / 2.0);
	  if (dir == 1)
		 c2 = -c2;
	  c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
	  for (i = 0; i<nn; i++) {
		 x[i] /= (double)nn;
		 y[i] /= (double)nn;
	  }
   }
}
	 
#endif
