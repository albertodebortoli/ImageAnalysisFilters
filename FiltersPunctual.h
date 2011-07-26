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
         

// Filtro di Equalizezione
// Questa funzione consente di andare a cercare di spalmare l'isogramma della
// Brightness` dell'immagine facendo in modo che i colori siano piu` bilanciati
// sullo spazio dei valori consentiti.
// Si trova il valore che va a bilanciare la mediana dell'istogramma della luminosit‡,
// portandola al centro dell'immagine. Consideriamo la media della Brightness` a 0.5
// e calcoliamo: 0.5 = sum(i = 0; l) ist(i) con cui si trova l'indice l.
// Si va a fare una iterazione su ogni pixel in cui andiamo
// ad applicare a ciascuno di questi la formula l_0 = sum(i = 0; l_i) ist(i)

void Equalize(Image* in, Image* out, bool type)
{
	//valore fra 0 e 1
    float v;

    //riempie l'istogramma con valori tra 0 e 1 soltanto ora
    CalculateHistograms(in, type);
    
    //lavora sull'immagine B/N
    if (type == false) {
    	for (int i = 0; i < (in->getX() * in->getY()); i++) {
    		v = 0.0; /* valore fra 0 e 1 */
    		for (int j = 0; j <= in->GetBn(i); j++) {
    			v += in->getIsto(j, 0);
    		}
    		v *= 255.0;
    		out->SetBn(i, (unsigned char)v);
    	}
    }
    
    //lavora sull'immagine RGB
    //fa attenzione a non modificare le tinte globali facendo la media dei
    //valori dei 3 istogrammi separati
    else {
        for (int i = 0; i < (in->getX() * in->getY()); i++) {
    		v = 0.0;
    		for (int j = 0; j <= in->GetRgb(i, 0); j++) {
    			v += ((in->getIsto(j, 'R') + in->getIsto(j, 'G') + in->getIsto(j, 'B')) /3 );
    		}
            v *= 255.0;
            out->SetRgb(i, 0, (unsigned char)v);
    		v = 0.0;
    		for (int j = 0; j <= in->GetRgb(i, 1); j++) {
    			v += ((in->getIsto(j, 'R') + in->getIsto(j, 'G') + in->getIsto(j, 'B')) /3 );
    		}
    		v *= 255.0;
            out->SetRgb(i, 1, (unsigned char)v);
    		v = 0.0;
    		for (int j = 0; j <= in->GetRgb(i, 2); j++) {
    			v += ((in->getIsto(j, 'R') + in->getIsto(j, 'G') + in->getIsto(j, 'B')) /3 );
    		}
    		v *= 255.0;
            out->SetRgb(i, 2, (unsigned char)v);
    	}
    }
}


// Filtro di Luminosit‡
// applica la trasformazione sia all'immagine B/N che RGB
// 'teoricamente' si usa la formula RGBout = RGBin * (BNout/BNin)
// in pratica (BNout/BNin) è il livello passato come terzo parametro

void Brightness(Image* in, Image* out, float Livello, bool type)
{
    float value;

    // aumenta la luminosit‡ della parte B/N
	if (type == false)
    for (int i = 0; i < (in->getX() * in->getY()); i++) {
        value = in->GetBn(i);
		value *= Livello;
		if (value > 255.0f)   value = 255.0f;
		if (value < 0.0f)     value = 0.0f;
        out->SetBn(i, (int)(value + 0.5f)); //+0.5f per il troncamento da float a int
    }

    // aumenta la luminosit‡ della parte RGB
	if (type == true)
    for (int i = 0; i < (in->getX() * in->getY()); i++)
        for (int j = 0; j < 3; j++) {
            value = in->GetRgb(i, j);
            value *= Livello; //livello corrisponde 'teoricamente' a (out->GetBn(i) / in->GetBn(i))
    		if (value > 255.0f)   value = 255.0f;
    		if (value < 0.0f)     value = 0.0f;
    		out->SetRgb(i, j, (int)value);
        }
}


// Filtro di Contrasto Automatico
// prende un'immagine che non sfrutta tutti i livelli e cerca di
// schiarire le zone pi˘ chiare e scurire ulteriormente le zone pi˘ scure
// effettua uno stretch alle estremit‡ dell'istogramma di luminosit‡
// la formula sulla quale si basa tale filtro è Lout=a+b*Lin
// il sistema risulta Lout=Loutmax (ovvero 255) e Lout=Loutmin (ovvero 0)
// con Lin=Linmin e Lin=Linmax calcolate si ottiene
// a=255/(Linmax-Linmin) e b=(255*Linmin)/(Linmax-Linmin)

void AutoContrast(Image* in, Image* out, bool type)
{
	int value;
	int min, max;
	
    // calcola il minimo e il massimo (i margini) dell'istogramma
    // min e max vanno bene sia se applica per B/N o RGB
	min = in->GetBn(0);
    max = in->GetBn(0);

	for (int i = 0; i < (in->getX()*in->getY()); i++) {
        if (in->GetBn(i) < min)    min = in->GetBn(i);
        if (in->GetBn(i) > max)    max = in->GetBn(i);
    }

    // applica il contrasto autometico all'immagine B/N
	if (type == false)
    for (int i = 0; i < (in->getX()*in->getY()); i++) {
		value = in->GetBn(i);
		value = (value * 255) / (max - min);
		value -= (255 * min) / (max - min);
		if (value > 255)   value = 255;
		if (value < 0)     value = 0;
        out->SetBn(i, value);
	}
	
	// applica il contrasto autometico all'immagine RGB
	if (type == true)
	for (int i = 0; i < (in->getX() * in->getY()); i++)
        for (int j = 0; j < 3; j++) {
            value = in->GetRgb(i, j);
    		value = (value * 255) / (max - min);
    		value -= (255 * min) / (max - min);
    		if (value > 255)   value = 255;
    		if (value < 0)     value = 0;
            out->SetRgb(i, j, value);
        }
}


// filtro di Gamma Correction
// risalta o abbatte le zone intermedie lavorando sulla densit‡ media dell'immagine
// applica la formula Lout=Lin^esp che non è lineare e si perde il rapporto di luminosit‡
// l'esponente è passato come terzo parametro

void Gamma(Image* in, Image* out, float esp, bool type)
{
    // applica la correzione gamma all'immagine B/N
	if (type == false)
	for (int i = 0; i < (out->getX()*out->getY()); i++) {
		float value = in->GetBn(i);
		value = pow(value/255.0f, esp);
		value *= 255.0f;
		if (value > 255.0f)   value = 255.0f;
		if (value < 0.0f)     value = 0.0f;
		out->SetBn(i, (int)(value + 0.5f)); //+0.5f per il troncamento da float a int
	}

    // applica la correzione gamma all'immagine RGB
    if (type == true)
    for (int i = 0; i < (out->getX()*out->getY()); i++)
        for (int j = 0; j < 3; j++) {
    		float value = in->GetRgb(i, j);
    		value = pow(value/255.0f, esp);
    		value *= 255.0f;
    		if (value > 255.0f)   value = 255.0f;
    		if (value < 0.0f)     value = 0.0f;
    		out->SetRgb(i, j, (int)(value + 0.5f)); //+0.5f per il troncamento da float a int
        }
}


// filtro di Gamma Channel Control
// risalta o abbatte le zone intermedie lavorando sulla densit‡ media dell'immagine
// il quarto parametro indica su quale canale RGB effettuare il filtro
// applica la formula Lout=Lin^esp che non è lineare e si perde il rapporto di luminosit‡
// l'esponente è passato come terzo parametro

void Channel(Image* in, Image* out, float esp, char comp)
{
    // applica la correzione gamma su un solo canale RGB
    int componente;
    switch (comp) {
        case 'R' : componente = 0; break;
        case 'G' : componente = 1; break;
        default : componente = 2;
    }

    for (int i = 0; i < (out->getX()*out->getY()); i++) {
        for (int j = 0; j < 3; j++)
            out->SetRgb(i, j, in->GetRgb(i, j));
        
		float value = in->GetRgb(i, componente);
		value = pow(value/255.0f, esp);
		value *= 255.0f;
		if (value > 255.0f)   value = 255.0f;
		if (value < 0.0f)     value = 0.0f;
		out->SetRgb(i, componente, (int)(value + 0.5f)); //+0.5f per il troncamento da float a int
    }
}


// filtro di Mirror
// riflette l'immagine sul suo asse orizzontale o verticale a seconda del valora del
// terzo parametro. se è 'x' rilfette orizzontalmente, se è 'y' verticalmente

void Mirror(Image* in, Image* out, char coord, bool type)
{
	int dimx = out->getX();
	int dimy = out->getY();
	
    int x_new, y_new, value, pixel;
	
	// applica la correzione gamma all'immagine B/N
	if (type == false) {
    	// riflette l'immagine sull'asse x
    	if (coord == 'X')
        	for (int x = 0; x < dimx; x++)
                for (int y = 0; y < dimy; y++) {
                    x_new = dimx-1-x;
                    value = in->GetBn(x_new+(y*dimx));
                    out->SetBn(x+(y*dimx), value);
                }
                
        // riflette l'immagine sull'asse y
    	if (coord == 'Y')
        	for (int x = 0; x < dimx; x++)
                for (int y = 0; y < dimy; y++) {
                    y_new = dimy-1-y;
                    value = in->GetBn(x+(y_new*dimx));
                    out->SetBn(x+(y*dimx), value);
                }
    }
    
    // applica la correzione gamma all'immagine RGB
    if (type == true) {
    	// riflette l'immagine sull'asse x
    	if (coord == 'x')
        	for (int x = 0; x < dimx; x++)
                for (int y = 0; y < dimy; y++) {
                    x_new = dimx-1-x;
                    for (int z = 0; z < 3; z++) {
                        pixel = in->GetRgb(x_new+(y*dimx), z);
                        if (pixel < 0)   pixel = 0;
                        if (pixel > 255) pixel = 255;
                        out->SetRgb(x+(y*dimx), z, pixel);
                    }
                }

    // riflette l'immagine sull'asse y
	if (coord == 'y')
    	for (int x = 0; x < dimx; x++)
            for (int y = 0; y < dimy; y++) {
                y_new = dimy-1-y;
                for (int z = 0; z < 3; z++) {
                    pixel = in->GetRgb(x+(y_new*dimx), z);
                    if (pixel < 0)   pixel = 0;
                    if (pixel > 255) pixel = 255;
                    out->SetRgb(x+(y*dimx), z, pixel);
                }
            }
    }          
}


// filtro di Pixelazer
// applica un effetto di sgranatura, il terzo parametro indica di quanti
// pixel saranno i quadrati che danno il senso di sgranatura

void Pixelize(Image* in, Image* out, int dim, bool type)
{
    int dimX = in->getX();
    int dimY = in->getY();
    
    if (type == false) {
        // immagine B/N
        int init = in->GetBn(0);
        init = in->GetBn(0);
    
        // prima lavora sull'asse x per sgranare l'immagine
        for (int y = 0; y < dimY; y++)
            for (int x = 0; x < dimX; x++) {
                if (x%dim  ==  0) init = in->GetBn((y*dimX)+x);
                out->SetBn((y*dimX)+x, init);
            }
        // poi fa lo stesso sull'asse y
        for (int x = 0; x < dimX; x++)
            for (int y = 0; y < dimY; y++) {
                if (y%dim  ==  0) init = out->GetBn((y*dimX)+x);
                out->SetBn((y*dimX)+x, init);
            }
    }  
    
    if (type == true) {    
        // immagine RGB
        int RGB[3] = {in->GetRgb(0, 0), in->GetRgb(0, 1), in->GetRgb(0, 2)};
        
        // prima lavora sull'asse x per sgranare l'immagine
        for (int y = 0; y < dimY; y++)
            for (int x = 0; x < dimX; x++) {
                if (x%dim  ==  0) {
                    RGB[0] = in->GetRgb((y*dimX)+x, 0);
                    RGB[1] = in->GetRgb((y*dimX)+x, 1);
                    RGB[2] = in->GetRgb((y*dimX)+x, 2);
                }
                out->SetRgb((y*dimX)+x, 0, RGB[0]);
                out->SetRgb((y*dimX)+x, 1, RGB[1]);
                out->SetRgb((y*dimX)+x, 2, RGB[2]);
            }
        // poi fa lo stesso sull'asse y
        for (int x = 0; x < dimX; x++)
            for (int y = 0; y < dimY; y++) {
                if (y%dim  ==  0) {
                    RGB[0] = out->GetRgb((y*dimX)+x, 0);
                    RGB[1] = out->GetRgb((y*dimX)+x, 1);
                    RGB[2] = out->GetRgb((y*dimX)+x, 2);
                }
                out->SetRgb((y*dimX)+x, 0, RGB[0]);
                out->SetRgb((y*dimX)+x, 1, RGB[1]);
                out->SetRgb((y*dimX)+x, 2, RGB[2]);
            }
    }
}


// filtro di Grayscale
// se l'immagine di input èè RGB interleaved la converte in formato 
// RGB interleaved in scala di grigi

void Grayscale(Image* in, Image* out)
{
    for (int i = 0; i < (in->getX() * in->getY()); i++) {
        for (int j = 0; j < 3; j++)
    		out->SetRgb(i, j, in->GetBn(i));
	}    
}


// Filtro di Invert
// applica l'Invert dei colori sia all'immagine B/N che RGB
// 'teoricamente' si usa la formula out = 255 - in

void Invert(Image* in, Image* out, bool type)
{
    int value;
    
    // inverte i colori della parte B/N
	if (type == false)
    for (int i = 0; i < (in->getX() * in->getY()); i++) {
        value = in->GetBn(i);
		value = 255 - value;
		if (value > 255)   value = 255;
		if (value < 0)     value = 0;
        out->SetBn(i, value); //+0.5f per il troncamento da float a int
    }

    // inverte i colori della parte RGB
	if (type == true)
    for (int i = 0; i < (in->getX() * in->getY()); i++) {
        for (int j = 0; j < 3; j++) {
            value = in->GetRgb(i, j);
    		value = 255 - value;
    		if (value > 255)   value = 255;
    		if (value < 0)     value = 0;
    		out->SetRgb(i, j, value);
        }
	}
}

#endif
