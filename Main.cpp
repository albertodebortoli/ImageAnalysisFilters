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

#include <iostream>
#include <string>
#include "FiltersAdj.h"
#include "FiltersPunctual.h"
#include "FiltersConvolution.h"
#include "FiltersStripes.h"
#include "FT.h"
#include "FFT.h"
#include "Image.h"

using namespace std;

int main(int argc, char* argv[])
{	
	// check base arguments presence
	if (argc < 7) {
		cout << "\nMissing or wrong arguments\n";
		cout << "Check Wiki for usage syntax\n";
		exit(0);
	}
	
	// check if the filter argument is presence
	if (argc == 7) {
		cout << "\nNo filter selected\n";
		cout << "\nCheck Wiki for usage syntax\n" << endl;
		exit(0);
	}
	
	int dimX = atoi(argv[3]);
	int dimY = atoi(argv[4]);
	Image* in = new Image(dimX, dimY);
	Image* out;

	// Load image
	switch(*argv[5]) {
		case '0' :  in->LoadImage(argv[1], 0);
					cout << "\nLoaded file: \t" << argv[1] << endl << "Type: \t\tgrayscale" << endl;
					cout << "Image size: \t" << in->getX() << "x" << in->getY() << " pixel\n";
					cout << "File size: \t\t" << (in->getX() * in->getY()) << " bytes" << endl;
					break;
		case '1' :  in->LoadImage(argv[1], 1);
					cout << "\nLoaded file: \t" << argv[1] << endl << "Type: \t\tcolor (RGB)" << endl;
					cout << "Image size: \t" << in->getX() << "x" << in->getY() << " pixel\n";
					cout << "File size: \t\t" << (in->getX() * in->getY() * 3) << " bytes" << endl;
					break;
		default : cout << "\n\tError: unspecified source grayscale or color RGB (0/1)\n";
	}

	bool type;
	switch(*argv[6]) {
		case '0' :  type = false; break;
		case '1' :  type = true; break;
		default :   type = true;
	}
		
	double* matRe;
	double* matIm;
	matRe = new double[in->getX()*in->getY()];
	matIm = new double[in->getX()*in->getY()];
	
	// based on arguments, filter are applied sequentially
	// for all filters the last argument 'type' indicates application on
	// grayscale or color (RGB) and therefore the destination image type 
	for (int i = 7; i < argc; i++) {
		out = new Image(dimX, dimY);
		switch(*argv[i]) {
			case '-' :  if (argv[i+1] != 0 && atoi(argv[i+1]) > 0 && atoi(argv[i+1]) < 255 &&
						   argv[i+2] != 0 && atoi(argv[i+2]) > 0 && atoi(argv[i+2]) % 2 == 1) {
								LowPass(in, out, atoi(argv[i+1]), atoi(argv[i+2]), type); i+=2;
								cout << "\n\tApplied filter: Low-Pass\n";
							}
						else {
							cout << "\nError: missing or wrong filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'F' :  if (argv[i+1] != 0 && ((*argv[i+1] == 'l') || (*argv[i+1] == 'h'))) {
							if (argv[i+2] != 0 && atoi(argv[i+2]) >= 0) {
								if (argv[i+3] != 0 && ((*argv[i+3] == 'I') || (*argv[i+3] == 'B'))) {
									int dimX_original = dimX;
									int dimY_original = dimY;									
									int m, twopmX, twopmY;
									
									// chose ideal or bell shaped filter
									bool ideal;
									if (*argv[i+3] == 'I') ideal = true;
									if (*argv[i+3] == 'B') ideal = false;
													   
									// adjust size to power of 2 if needed 
									if (!Powerof2(dimX, &m, &twopmX)) twopmX*=2;
									if (!Powerof2(dimY, &m, &twopmY)) twopmY*=2;
									if ((twopmX != dimX) || twopmY != dimY) {
											dimX = twopmX;
											dimY = twopmY;
											AdjustSizeToPower2(in, dimX, dimY, dimX_original, dimY_original);
											delete matRe;
											delete matIm;
											matRe = new double[dimY*dimX];
											matIm = new double[dimY*dimX];
										}
									
									if (!type) {   
										for (int j=0; j<dimY; j++)
											for (int k=0; k<dimX; k++) {
												matRe[k + (j * dimX)] = (double)(in->GetBn(k + (j * dimX)));
												matIm[k + (j * dimX)] = 0.0f;
											}
											
										// apply forward FFT
										FFT2D(matRe, matIm, dimX, dimY, 1);
										cout << "\n\tApplied forward FFT\n";
										
										if (*argv[i+1] == 'h') {
											SwapSpectrum(matRe, matIm, dimX, dimY);
											// compute and save "magnitude.raw"
											Magnitude(matRe, matIm, dimX, dimY);
											HighPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
											cout << "\n\tApplied filter: High-Pass filter\n";
											SwapSpectrum(matRe, matIm, dimX, dimY);
										}
										if (*argv[i+1] == 'l') {
											SwapSpectrum(matRe, matIm, dimX, dimY);
											// compute and save "magnitude.raw"
											Magnitude(matRe, matIm, dimX, dimY);
											LowPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
											cout << "\n\tApplied filter: Low-Pass filter\n";
											SwapSpectrum(matRe, matIm, dimX, dimY);									   
										}
										
										// apply backward FFT
										FFT2D(matRe, matIm, dimX, dimY, -1);
										cout << "\n\tApplied backward FFT\n";
										
										for (int j=0; j<dimY_original; j++)
											for (int k=0; k<dimX_original; k++) {
												out->SetBn(k+(j*dimX_original), (int)(matRe[k+(j*dimX)]));
											}

									}
									else {
										for (int c = 0; c < 3; c++) {
											for (int j = 0; j < dimY; j++)
												for (int k = 0; k < dimX; k++) {
													matRe[k + (j * dimX)] = (double)(in->GetRgb(k + (j * dimX), c));
													matIm[k + (j * dimX)] = 0.0f;
												}
											
											FFT2D(matRe, matIm, dimX, dimY, 1);
											cout << "\n\tApplied FFT forward su componente RGB " << c << "\n";
											
											if (*argv[i+1] == 'h') {
												SwapSpectrum(matRe, matIm, dimX, dimY);
												HighPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
												cout << "\n\tApplied filter: High-Pass filter on RGB component: " << c << endl;
												SwapSpectrum(matRe, matIm, dimX, dimY);
											}
											if (*argv[i+1] == 'l') {
												SwapSpectrum(matRe, matIm, dimX, dimY);
												LowPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
												cout << "\n\tApplied filter: Low-Pass filter on RGB component: " << c << endl;
												SwapSpectrum(matRe, matIm, dimX, dimY);
											}
											
											FFT2D(matRe, matIm, dimX, dimY, -1);
											cout << "\n\tApplied FFT backward on RGB component: " << c << endl;

											for (int j=0; j<dimY_original; j++)
												for (int k=0; k<dimX_original; k++) {
													out->SetRgb(k+(j*dimX_original), c, (int)(matRe[k+(j*dimX)]));
												}
										}
									}
									dimX = dimX_original;
									dimY = dimY_original;
									i+=3;
								}
								else {
									cout << "\n\tError: select filter value (ideal/bellshaped)\n" << endl;
									exit(0);
								}
							}
							else {
								cout << "\n\tError: provide cut off value\n" << endl;
								exit(0);
							}
						}
						else {
							cout << "\n\tError: missing filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'f' :  if (argv[i+1] != 0 && ((*argv[i+1] == 'l') || (*argv[i+1] == 'h'))) {
							if (argv[i+2] != 0 && atoi(argv[i+2]) >= 0) {
								if (argv[i+3] != 0 && ((*argv[i+3] == 'I') || (*argv[i+3] == 'B'))) {
									double* matrix = new double[dimX * dimY];
									// chose ideal or bell shaped filter
									bool ideal;
									if (*argv[i+3] == 'I') ideal = true;
									if (*argv[i+3] == 'B') ideal = false;
									
									if (!type) {
										if (dimX * dimY > 128*128)
										cout << "\nUse FT only on small images to avoid huge computational time\n" <<
										"Use FFT (Fast Fourier Transform) on bigger images.\n";
										
										for (int k=0; k<dimX * dimY; k++)
											matrix[k] = in->GetBn(k);
											
										// apply forward FT
										FT(matrix, dimX, dimY, matRe, matIm, 1);
										
										// compute and save "magnitude.raw"
										Magnitude(matRe, matIm, dimX, dimY);
											
										if (*argv[i+1] == 'h') {
											HighPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
											cout << "\n\tApplied filter: High-Pass filter\n";
										}
										if (*argv[i+1] == 'l') {
											LowPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
											cout << "\n\tApplied filter: Low-Pass filter\n";
										}
										
										// apply backward FT
										FT(matrix, dimX, dimY, matRe, matIm, -1);
										
										for (int k=0; k<dimX * dimY; k++)
											out->SetBn(k, (int)matrix[k]);
									}
									else {
										for (int c = 0; c < 3; c++) {
											if (dimX * dimY > 64*64)
											cout << "\nUse FT only on small images to avoid huge computational time\n" <<
											"Use FFT (Fast Fourier Transform) on bigger images.\n";
											
											for (int k=0; k<dimX * dimY; k++)
												matrix[k] = in->GetRgb(k, c);
											
											// applica la FT in avanti
											FT(matrix, dimX, dimY, matRe, matIm, 1);
																						
											if (*argv[i+1] == 'h') {
												HighPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
												cout << "\n\tApplied filter: High-Pass\n";
											}
											if (*argv[i+1] == 'l') {
												LowPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
												cout << "\n\tApplied filter: Low-Pass\n";
											}
											// applica la FT indietro
											FT(matrix, dimX, dimY, matRe, matIm, -1);
											
											for (int k=0; k<dimX * dimY; k++)
												out->SetRgb(k, c, (int)matrix[k]);
										}
									}
									delete matrix;
									i+=3;
								}
								else {
									cout << "\n\tError: select filter value (ideal/bellshaped)\n" << endl;
									exit(0);
								}
							}
							else {
								cout << "\n\tError: provide cut off value\n" << endl;
								exit(0);
							}
						}
						else {
							cout << "\n\tError: missing filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'e' :  Equalize(in, out, type);
						cout << "\n\tApplied filter: Brightness Equalization'\n";
						break;
			case 'i' :  Invert(in, out, type);
						cout << "\n\tApplied filter: Invert\n";
						break;
			case 'l' :  if (argv[i+1] != 0 && atof(argv[i+1]) >= 0.0f) {
							Brightness(in, out, atof(argv[i+1]), type); i++;
							cout << "\n\tApplied filter: Brightness'\n";
							}
						else {
							cout << "\n\tError: missing filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'a' :  AutoContrast(in, out, type);
						cout << "\n\tApplied filter: Auto Contrast\n";
						break;
			case 'g' :  if (argv[i+1] != 0 && atof(argv[i+1]) >= 0.0f) {
							Gamma(in, out, atof(argv[i+1]), type); i++;
							cout << "\n\tApplied filter: Gamma Correction\n";
						}
						else {
							cout << "\n\tError: missing filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'h' :  if (type == true) {
							if (argv[i+1] != 0 && argv[i+2] != 0 && atof(argv[i+1]) >= 0.0f &&
								((*argv[i+2] == 'R') || (*argv[i+2] == 'G') || (*argv[i+2] == 'B'))) {
								Channel(in, out, atof(argv[i+1]), *argv[i+2]); i=i+2;
								cout << "\n\tApplied filter: Channel\n";
							}
							else {
								cout << "\n\tError: missing filter aruguments\n" << endl;
								exit(0);
							}
						}
						else {
							cout << "\n\tError: filter only for color images\n" << endl;
							exit(0);
						}
						break;
			case 'm' :  if (argv[i+1] != 0) {
							if (*argv[i+1] == 'X') {
								Mirror(in, out, 'X', type); i++;
								cout << "\n\tApplied filter: Mirror (horizontal)\n";
							}
							else if (*argv[i+1] == 'Y') {
								Mirror(in, out, 'Y', type); i++;
								cout << "\n\tApplied filter: Mirror (vertical)\n";
							}
							else {
								cout << "\n\tError: wrong filter aruguments\n" << endl;
								exit(0);
							}
						}
						else {
							cout << "\n\tError: missing filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'p' :  if (argv[i+1] != 0 && atoi(argv[i+1]) > 0) {
							Pixelize(in, out, atoi(argv[i+1]), type); i++;
							cout << "\n\tApplied filter: Pixelazer\n";
						}
						else {
							cout << "\n\tError: missing or wrong filter aruguments\n" << endl;
							exit(0);
						}
						break; 
			
			case 's' :  if (argv[i+1] != 0 && atoi(argv[i+1]) > 0 && atoi(argv[i+1])%2 == 1) {
							Smooth(in, out, atoi(argv[i+1]), type); i++;
							cout << "\n\tApplied filter: Smooth\n";
						}
						else {
							cout << "\n\tError: missing or wrong filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'r' :  if (argv[i+1] != 0 && atoi(argv[i+1]) > 0 && atoi(argv[i+1])%2 == 1) {
							NoiseRemoval(in, out, atoi(argv[i+1]), type); i++;
							cout << "\n\tApplied filter: Noise removal\n";
						}
						else {
							cout << "\n\tError: missing or wrong filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'c' :  if (argv[i+1] != 0) {
							if (*argv[i+1] == 'S') {
								Convolution(in, out, 'S', type); i++;
								cout << "\n\tApplied filter: Sharpen\n";
							}
							else if (*argv[i+1] == 'B') {
								Convolution(in, out, 'B', type); i++;
								cout << "\n\tApplied filter: Blur\n";
							}
							else if (*argv[i+1] == 'O') {
								Convolution(in, out, 'O', type); i++;
								cout << "\n\tApplied filter: Outliner\n";
							}
							else {
								cout << "\n\tError: wrong filter aruguments\n" << endl;
								exit(0);
							}
						}
						else {
							cout << "\n\tError: missing filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'q' :  if (argv[i+1] != 0) {
							Rotation(in, out, atof(argv[i+1]), type); i++;
							cout << "\n\tApplied filter: Rotate\n";
						}
						else {
							cout << "\n\tError: missing filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'z' :  if (argv[i+1] != 0 && argv[i+2] != 0) {
							if (*argv[i+1] == '+') {
								Zoom(in, out, '+', atoi(argv[i+2]), type); i=i+2;
								cout << "\n\tApplied filter: Zoom in\n";
							}
							else if (*argv[i+1] == '-') {
								Zoom(in, out, '-', atoi(argv[i+2]), type); i=i+2;
								cout << "\n\tApplied filter: Zoom out\n";
							}
							else {
								cout << "\n\tError: wrong filter aruguments\n" << endl;
								exit(0);
							}
						}
						else {
							cout << "\n\tError: missing filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'Z' :  if (argv[i+1] != 0) {
							Resize(in, out, atoi(argv[i+1]), type); i++;
							cout << "\n\tApplied filter: Resize\n";
						}
						else {
							cout << "\n\tError: missing or wrong filter aruguments\n" << endl;
							exit(0);
						}
						break;
			case 'b' :  if (type == true) {
							Grayscale(in, out);
							cout << "\n\tApplied filter: Grayscale\n";
						}
						else {
							cout << "\n\tError: filter only for color images\n" << endl;
							exit(0);
						}
						break;

			default :   cout << "\n\tError: wrong argument\n" << endl;
						exit(0);
						break;
			}
		delete in;
		in = out;
	}
	
	// Save image
	switch(type) {
		case 0 :	out->SaveImage(argv[2], 0);
					cout << "\nSaved file: \t" << argv[2] << endl << "Type: \t\tBN" << endl;
					cout << "Image size: \t" << out->getX() << "x" << out->getY() << " pixel\n";
					cout << "File size: \t\t" << (out->getX() * out->getY()) << " bytes" << endl;
					break;
		case 1 :	out->SaveImage(argv[2], 1);
					cout << "\nSaved file: \t" << argv[2] << endl << "Type: \t\tRGB" << endl;
					cout << "Image size: \t" << out->getX() << "x" << out->getY() << " pixel\n";
					cout << "File size: \t\t" << (out->getX() * out->getY() * 3) << " bytes" << endl;
					break;
		default :   cout << "\n\tError: unspecified destination grayscale or color RGB (0/1)\n";
	}
	
	delete out;
	delete matRe;
	delete matIm;
	
	cout << endl;
	
	exit(0);
}
