/*
 * Copyright (c) 2008, Alberto De Bortoli at
 * Università degli Studi di Padova. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Università degli Studi di Padova nor the
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

#include "Image.h"

using namespace std;

// costruttore della classe Immagine. Vengono creati e inizializzati i campi
// per la gestione dell'immagine in B/N, in RGB interleaved e vengono 
// memorizzate le dimensioni dell'immagine

Image::Image(int x, int y)
{
	bn = new unsigned char[x*y];
	rgb = new unsigned char[x*y*3];
	isto = new float[256];
	istoR = new float[256];
	istoG = new float[256];
	istoB = new float[256];
	dimX = x;
	dimY = y;
	for (int i = 0; i < 256; i++) {
        isto[i] = 0.0;
        istoR[i] = 0.0;
        istoG[i] = 0.0;
        istoB[i] = 0.0;
    }
}


// Per ogni pixel RGB dell'immagine ne rileva il valore, effettua poi 
// una media dei 3 valori e li salva nell'array BN.

void Image::FitBN()
{
	int pixel = 0;
	for (int i = 0; i < dimX * dimY; i++) {
		pixel = GetRgb(i, 0) + GetRgb(i, 1) + GetRgb(i, 2);
		SetBn(i, pixel/3);
	}
}


// Per ogni pixel BN dell'immagine ne rileva il valore, 
// e lo salva nell'array RGB.

void Image::FitRGB()
{
	for (int i = 0; i < dimX * dimY; i++) {
        for (int j = 0; j < 3; j++)
    		SetRgb(i, j, GetBn(i));
	}
}


// la funziona carica l'immagine da disco al path (assoluto o relativo) indicato
// nel primo parametro. Se il secondo parametro è 0 carica in formato raw B/N
// se è 1 carica nel formato raw RGB interleaved

void Image::CaricaImmagine(char* nome, int tipo)
{
	FILE* fhan;
	if (!(fhan = fopen(nome, "rb"))) {
		cout << "Impossibile leggere il file sorgente\n";
		exit(0);
	}
	
	switch (tipo) {
        case 0  :   fread(bn, dimX * dimY, 1, fhan);
                    // caricando l'immagine in B/N riempio la parte RGB
                    this->FitRGB();
                    break;
        case 1  :   fread(rgb, dimX * dimY * 3, 1, fhan);
                    // caricando l'immagine in RGB riempio la parte B/N
                    this->FitBN();
                    break;
        default :   break;
    }

	fclose(fhan);
}


// la funziona salva l'immagine su disco nel path (assoluto o relativo) indicato
// nel primo parametro. Se il secondo parametro è 0 salva in formato raw B/N
// se è 1 salva nel formato raw RGB interleaved

void Image::SalvaImmagine(char* nome, int tipo)
{
	FILE* fhan;
	if (!(fhan = fopen(nome, "wb"))) {
		cout << "Impossibile scrivere nel file destinazione\n";
		exit(0);
	}
	switch (tipo) {
        case 0  :   fwrite(bn, dimX * dimY, 1, fhan); break;
        case 1  :   fwrite(rgb, dimX * dimY * 3, 1, fhan); break;
        default :   break;
    }
	fclose(fhan);
}


// distruttore dell'oggetto Immagine

Image::~Image()
{
	delete bn;
	delete rgb;
	delete isto;
	delete istoR;
	delete istoG;
	delete istoB;
	dimX = 0;
	dimY = 0;
}
