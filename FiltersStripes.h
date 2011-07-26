/*
 * Copyright (c) 2008, Alberto De Bortoli at
 * Universitˆ degli Studi di Padova. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Universitˆ degli Studi di Padova nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
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

#ifndef FILTERSSTRIPES_H
#define FILTERSSTRIPES_H

#include"Image.h"
using namespace std;

void LowPass(Image* in, Image* out, int threshold, int kernel_dim, bool type);
float BellShaped(int x, int y, int Liv, int dimX, int dimY);
void LowPassFilter(int Liv, double* matRe, double* matIm, int dimX, int dimY, bool ideal);
void HighPassFilter(int Liv, double* matRe, double* matIm, int dimX, int dimY, bool ideal);


// Filtro PassaBasso (LowPass)
// il filtro taglia le frequesnze alte facendo passare quelle basse.
// E' una versione particolare del filtro che sfrutta la funzione FindAverage
// per trovare la media del valore dei pixel dell'intorno al pixel passato al secondo 
// e terzo parametro. Il terzo parametro serve per impostare la soglia di taglio e di 
// conseguenza la maggiore o minore aggressività del filtro. Viene inoltre passono per 
// quarto parametro la dimensione del kernel, anch'esso comporterà una maggior o minore
// aggressività del filtro.

// Lo scopo  calcolare la differenza tra il pixel e la media dei pixel el suo
// intorno. Se la differenza tra il valore del pixel e della media  maggiore
// della soglia, significa che vi  un cambiamento netto (alta frequenza) che
// va annullata sostituendo a quel pixel il valore della media dell'intorno.

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
                diff = abs(in->GetBn(j+(i*dimX))-media[0]);
    			// + basso  il valore di soglia più aggressivo  il filtro passa basso
    			if (diff > threshold) out->SetBn(j+(i*dimX), media[0]);
    			else out->SetBn(j+(i*dimX), in->GetBn(j+(i*dimX)));
    		}
    	}
    }
    
    if (type) {
        for (i = 0; i < dimY; i++) {
    		for (j = 0; j < dimX; j++) {
                rgb = FindAverage(in, j, i, kernel_dim, media, type);
                m_pixel = (in->GetRgb(j+(i*dimX), 0) + in->GetRgb(j+(i*dimX), 1) + in->GetRgb(j+(i*dimX), 2))/3;
                m_rgb = (rgb[0] + rgb[1] + rgb[2])/3;
                diff = abs(m_pixel - m_rgb);
                if (diff > threshold) {
                    out->SetRgb(j+(i*dimX), 0, rgb[0]);
                    out->SetRgb(j+(i*dimX), 1, rgb[1]);
                    out->SetRgb(j+(i*dimX), 2, rgb[2]);
                }
                else{
                    out->SetRgb(j+(i*dimX), 0, in->GetRgb(j+(i*dimX), 0));
                    out->SetRgb(j+(i*dimX), 1, in->GetRgb(j+(i*dimX), 1));
                    out->SetRgb(j+(i*dimX), 2, in->GetRgb(j+(i*dimX), 2));
                }
            }
        }
    }

}


// calcola la BellShaped per non avere un affetto a gradino sul taglio
// delle frequenze, ovvero evita il ILPF e il IHPF
// (Ideal Low/High Pass Filtering)

float BellShaped(int x, int y, int Liv, int dimX, int dimY)
{
    float d;
    x-= dimX/2;
    y-= dimY/2;
    d = sqrt(x*x*y*y);
    return (exp(-d/Liv));
}


// Filtro Passa Basso
// effettua un filtro low-pass sulla trasformata FFT
// il parametro ideal indica se applicare un filtro ideale o a BellShaped

void LowPassFilter(int Liv, double* matRe, double* matIm, int dimX, int dimY, bool ideal)
{
    int dx, dy;
    for (int y = 0; y < dimY; y++) {
        for (int x = 0 ; x < dimX; x++) {
            double k;
            if (ideal) k = 1;
            else k = BellShaped(x,y, Liv, dimX, dimY);
                matRe[x+(y*dimX)] *= k;
                matIm[x+(y*dimX)] *= k;
                dx = x -(dimX/2);
                dy = y -(dimY/2);
                if (sqrt(dx*dx + dy*dy) > Liv) {
                    matRe[x+(y*dimX)] = 0;
                    matIm[x+(y*dimX)] = 0;
                }
            }
        }
    }


// Filtro Passa Alto
// effettua un filtro high-pass sulla trasformata FFT
// il parametro ideal indica se applicare un filtro ideale o a BellShaped

void HighPassFilter(int Liv, double* matRe, double* matIm, int dimX, int dimY, bool ideal)
{
    int dx, dy;
    for (int y = 0; y < dimY; y++) {
        for (int x = 0 ; x < dimX; x++) {
            double k;
            if (ideal) k = 1;
            else k = BellShaped(x,y, Liv, dimX, dimY);
                matRe[x+(y*dimX)] *= (3 -2*k);
                matIm[x+(y*dimX)] *= (3 -2*k);
                dx = x -(dimX/2);
                dy = y -(dimY/2);
                if (sqrt(dx*dx + dy*dy) < Liv) {
                    matRe[x+(y*dimX)] = 0;
                    matIm[x+(y*dimX)] = 0;
                }
            }
        }
    }

#endif
