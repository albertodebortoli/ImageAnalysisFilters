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

#ifndef IMAGE_H
#define IMAGE_H

#include<string>
#include<iostream>

using namespace std;

class Image
{
	private:
		// width
		int dimX;
		
		// height
		int dimY;
		
		// grayscale representation (brightness values)
		unsigned char* bn;
		
		// color RGB interleaved representation
		unsigned char* rgb;
		
		// brightness hystogram
		float* hysto;
		
		// red hystogram
		float* hystoR;
		
		// green hystogram
		float* hystoG;
		
		// blue hystogram
		float* hystoB;

	public:
		// create an image x*y with each pixel black
		Image(int x = 0, int y = 0);
		
		// getter for width
		int getX() {
			return dimX;
		}
		
		// getter for height
		int getY() {
			return dimY;
		}
		
		// setter for color hystograms
		void setIsto(int index, float value, char type) {
			switch (type){
				case 'R' : hystoR[index] = value; break;
				case 'G' : hystoG[index] = value; break;
				case 'B' : hystoB[index] = value; break;
				default : hysto[index] = value; break;
			}
		}
		
		// getter for color hystograms
		float getIsto(int index, char type) {
			switch (type){
				case 'R' : return hystoR[index]; break;
				case 'G' : return hystoG[index]; break;
				case 'B' : return hystoB[index]; break;
				default : return hysto[index]; break;
			}
		}
		
		// compute grayscale image from color one (RGB interleaved)
		void FitBN();
		
		// compute color image (RGB interleaved) from grayscale one 
		void FitRGB();
		
		// getter for the pixel at the given index
		unsigned char GetBn(int index) {
			return bn[index];
		}
		
		// setter for the pixel at the given index
		void SetBn(int index, int value) {
			bn[index] = value;
		}
		
		// getter for the RGB pixel at the given index 
		// 'value' indicates the RGB component
		// 0 means R
		// 1 means G
		// 2 means B
		unsigned char GetRgb(int index, int value) {
			return rgb[(index) * 3 + value];
		}
		
		// setter for the RGB pixel at the given index 
		// 'rgb_index' indicates the RGB component
		// 0 means R
		// 1 means G
		// 2 means B
		void SetRgb(int index, int rgb_index, unsigned char value) {
			rgb[index * 3 + rgb_index] = value;
		}
		
		// Load the image from the given path
		void LoadImage(char* nome, int tipo);
		
		// Save the image on disk to the given path
		void SaveImage(char* nome, int tipo);
		
		~Image();
};

#endif
