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

#ifndef IMAGE_H
#define IMAGE_H

#include<string>
#include<iostream>

using namespace std;

class Image
{
	private:
		// larghezza dell'immagine
		int dimX;
		
        // altezza dell'immagine
		int dimY;
		
        // array che rappresenta l'immagine in B/N, ovvero la luminosita'
		unsigned char* bn;
        
        // array che rappresenta l'immagine in RGB interleaved
		unsigned char* rgb;
		
		// array che rappresenta l'istogramma della luminosità
		float* isto;
		
		// array che rappresenta l'istogramma dei rossi
		float* istoR;
		
		// array che rappresenta l'istogramma dei verdi
		float* istoG;
		
		// array che rappresenta l'istogramma dei blu
		float* istoB;

	public:
		// costruisce una immagine con x in larghezza e y in altezza, tutta nera.
		Image(int x = 0, int y = 0);
		
		// ritorna la larghezza dell'immagine
		int getX() {
            return dimX;
        }
        
		// ritorna l'altezza dell'immagine
		int getY() {
            return dimY;
        }
        
        // imposta il livello di istogramma passato
		void setIsto(int index, float value, char type) {
            switch (type){
                case 'R' : istoR[index] = value; break;
                case 'G' : istoG[index] = value; break;
                case 'B' : istoB[index] = value; break;
                default : isto[index] = value; break;
            }
        }
        
		// ritorna il livello di istogramma passato
		float getIsto(int index, char type) {
            switch (type){
                case 'R' : return istoR[index]; break;
                case 'G' : return istoG[index]; break;
                case 'B' : return istoB[index]; break;
                default : return isto[index]; break;
            }
        }
        
		// calcola la parte BN a partire da una immagine raw RGB interleaved
		void FitBN();

		// calcola la parte RGB a partire da una immagine raw BN
		void FitRGB();
		
		// restituisce la luminosita' del pixel all'indice index
    	unsigned char GetBn(int index) {
            return bn[index];
        }
		
        // imposta la luminosita' del pixel all'indice index con il valore l
		void SetBn(int index, int value) {
            bn[index] = value;
        }
        
		// restituisce il pixel RGB all'indice index, il secondo 
		// parametro indica quale componente RGB si vuole ottenere
		unsigned char GetRgb(int index, int value) {
            return rgb[(index)*3 + value];
        }
        
    	// imposta il pixel RGB all'indice index, il secondo
		// parametro indica quale componente RGB si vuole settare
		void SetRgb(int index, int rgb_index, unsigned char value) {
            rgb[index*3 + rgb_index] = value;
        }
        
        // metodo che inizializza rgb con un file richiesto tramite parametro nome
		void CaricaImmagine(char* nome, int tipo);
		
		// metodo che salva rgb sul file richiesto tramite parametro
		void SalvaImmagine(char* nome, int tipo);
		
		// distruttore dell'oggetto Immagine
		~Image();
};

#endif
