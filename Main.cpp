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

#include <iostream>
#include <string>
#include "FiltersAdj.h"
#include "FiltersPunctual.h"
#include "FiltersConvolution.h"
#include "FiltersStripes.h"
#include "FT.h"
#include "FFT.h"


using namespace std;

int main(int argc, char* argv[])
{    
    // controlla se almeno i parametri di base sono stati passati
    if (argc < 7) {
        cout << "\nParametri assenti o passati in maniera errata\n";
        cout << "Consultare il manuale allegato al progetto per la sintassi corretta\n";
        exit(0);
    }
    
    // controlla se il parametro di filtro  stato passato
    if (argc  ==  7) {
        cout << "\nNessun filtro selezionato\n";
        cout << "Consultare il manuale allegato al progetto per la sintassi corretta\n";
        exit(0);
    }
    
    // alloca una nuova immagine sorgente delle dimensioni passate
	int dimX = atoi(argv[3]);
    int dimY = atoi(argv[4]);
    Image* in = new Image(dimX, dimY);
	Image* out;

    // caricamento dell'immagine, se il secondo parametro di CaricaImmagine  0 viene caricata
    // come fosse una raw in B/N, se  1 viene caricata come fosse in formato RGB interleaved
    switch(*argv[5]) {
        case '0' :  in->CaricaImmagine(argv[1], 0);
                    cout << "\nFile caricato: \t" << argv[1] << endl << "Tipo: \t\tBN" << endl;
                    cout << "Dimensioni: \t" << in->getX() << "x" << in->getY() << " pixel\n";
                    cout << "Size: \t\t" << (in->getX() * in->getY()) << " bytes" << endl;
                    break;
        case '1' :  in->CaricaImmagine(argv[1], 1);
                    cout << "\nFile caricato: \t" << argv[1] << endl << "Tipo: \t\tRGB" << endl;
                    cout << "Dimensioni: \t" << in->getX() << "x" << in->getY() << " pixel\n";
                    cout << "Size: \t\t" << (in->getX() * in->getY() * 3) << " bytes" << endl;
                    break;
        default : cout << "\nErrore: non specificato sorgente BN o RGB (0/1)\n";
	}

    bool type;
    switch(*argv[6]) {
        case '0' :  type = false; break;
        case '1' :  type = true; break;
        default :   type = true;// salva in RGB di default
	}
		
	double* matRe;
	double* matIm;
	matRe = new double[in->getX()*in->getY()];
	matIm = new double[in->getX()*in->getY()];
	
    // in base ai parametri selezionati verranno applicati uno o più filtri in cascata
    // per tutti i filtri l'ultimo parametro type indica se applicare il filtro all'immagine
    // B/N o RGB. In questa maniera si applica il filtro in base al tipo di formato 
    // in cui salvare l'immagine
	for (int i = 7; i < argc; i++) {
        out = new Image(dimX, dimY);
		switch(*argv[i]) {
            case '-' :  if (argv[i+1] != 0 && atoi(argv[i+1]) > 0 && atoi(argv[i+1]) < 255 &&
                           argv[i+2] != 0 && atoi(argv[i+2]) > 0 && atoi(argv[i+2])%2  ==  1) {
                                LowPass(in, out, atoi(argv[i+1]), atoi(argv[i+2]), type); i+=2;
                                cout << "\n Applicato filtro: Passa Basso (Low-Pass)\n";
                            }
                        else{
                            cout << "\n Errore: valori di filtro assenti o scorretti\n";
                            exit(0);
                        }
                        break;
            case 'F' :  if (argv[i+1] != 0 && ((*argv[i+1]  ==  'l') || (*argv[i+1]  ==  'h'))) {
                            if (argv[i+2] != 0 && atoi(argv[i+2]) >= 0) {
                                if (argv[i+3] != 0 && ((*argv[i+3]  ==  'I') || (*argv[i+3]  ==  'C'))) {
                                    int dimX_original = dimX;
                                    int dimY_original = dimY;                                    
                                    int m, twopmX, twopmY;
                                    
                                    // sceglie se applica il filtro ideale (a gradino) o a campana
                                    bool ideal;
                                    if (*argv[i+3]  ==  'I') ideal = true;
                                    if (*argv[i+3]  ==  'C') ideal = false;
                                                       
                                    // controllo che l'immagine abbia dimensioni di una potenza di 2
                                    if (!Powerof2(dimX, &m, &twopmX)) twopmX*=2;
                                    if (!Powerof2(dimY, &m, &twopmY)) twopmY*=2;
                                    if ((twopmX != dimX) || twopmY != dimY) {
                                            dimX = twopmX;
                                            dimY = twopmY;
                                            AdjustSizeToPower2(in, dimX, dimY, dimX_original, dimY_original);
                                            // rialloco le matrici
                                            delete matRe;
                                            delete matIm;
                                            matRe = new double[dimY*dimX];
                                            matIm = new double[dimY*dimX];
                                        }
                                    
                                    if (!type) {   
                                        // copio in in matRe e inizializzo a 0 matIm
                                        for (int j=0; j<dimY; j++)
                                            for (int k=0; k<dimX; k++) {
                                                matRe[k+(j*dimX)] = (double)(in->GetBn(k+(j*dimX)));
                                                matIm[k+(j*dimX)] = 0.0f;
                                            }
                                            
                                        // applica la FT in avanti
                                        FFT2D(matRe, matIm, dimX, dimY, 1);
                                        cout << "\n Applicata FFT forward\n";
                                        
                                        if (*argv[i+1]  ==  'h') {
                                            SwapSpectrum(matRe, matIm, dimX, dimY);
                                            //calcola e salva lo spettro nel file "magnitude.raw"
                                            Magnitude(matRe, matIm, dimX, dimY);
                                            HighPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
                                            cout << "\n Applicato filtro: High-Pass filter\n";
                                            SwapSpectrum(matRe, matIm, dimX, dimY);
                                        }
                                        if (*argv[i+1]  ==  'l') {
                                            SwapSpectrum(matRe, matIm, dimX, dimY);
                                            //calcola e salva lo spettro nel file "magnitude.raw"
                                            Magnitude(matRe, matIm, dimX, dimY);
                                            LowPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
                                            cout << "\n Applicato filtro: Low-Pass filter\n";
                                            SwapSpectrum(matRe, matIm, dimX, dimY);                                       
                                        }
                                        // applica la FT indietro
                                        FFT2D(matRe, matIm, dimX, dimY, -1);
                                        cout << "\n Applicata FFT backward\n";
                                        
                                        for (int j=0; j<dimY_original; j++)
                                            for (int k=0; k<dimX_original; k++) {
                                                out->SetBn(k+(j*dimX_original), (int)(matRe[k+(j*dimX)]));
                                            }

                                    }
                                    else {
                                        // copio in in matRe e inizializzo a 0 matIm
                                        for (int c=0; c<3; c++) {
                                            for (int j=0; j<dimY; j++)
                                                for (int k=0; k<dimX; k++) {
                                                    matRe[k+(j*dimX)] = (double)(in->GetRgb(k+(j*dimX), c));
                                                    matIm[k+(j*dimX)] = 0.0f;
                                                }
                                            // applica la FT in avanti
                                            FFT2D(matRe, matIm, dimX, dimY, 1);
                                            cout << "\n Applicata FFT forward su componente RGB " << c << "\n";
                                            
                                            if (*argv[i+1]  ==  'h') {
                                                SwapSpectrum(matRe, matIm, dimX, dimY);
                                                HighPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
                                                cout << "\n Applicato filtro: High-Pass filter su componente RGB " << c << "\n";
                                                SwapSpectrum(matRe, matIm, dimX, dimY);
                                            }
                                            if (*argv[i+1]  ==  'l') {
                                                SwapSpectrum(matRe, matIm, dimX, dimY);
                                                LowPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
                                                cout << "\n Applicato filtro: Low-Pass filter su componente RGB " << c << "\n";
                                                SwapSpectrum(matRe, matIm, dimX, dimY);
                                            }
                                            // applica la FT indietro
                                            FFT2D(matRe, matIm, dimX, dimY, -1);
                                            cout << "\n Applicata FFT backward su componente RGB " << c << "\n";

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
                                    cout << "\n Errore: selezionare filtraggio ideale o a campana\n";
                                    exit(0);
                                }
                            }
                            else {
                                cout << "\n Errore: selezionare il valore di taglio\n";
                                exit(0);
                            }
                        }
                        else {
                            cout << "\n Errore: valore di filtro assente\n";
                            exit(0);
                        }
                        break;
            case 'f' :  if (argv[i+1] != 0 && ((*argv[i+1]  ==  'l') || (*argv[i+1]  ==  'h'))) {
                            if (argv[i+2] != 0 && atoi(argv[i+2]) >= 0) {
                                if (argv[i+3] != 0 && ((*argv[i+3]  ==  'I') || (*argv[i+3]  ==  'C'))) {
                                    double* matrix = new double[dimX * dimY];
                                    // sceglie se applica il filtro ideale (a gradino) o a campana
                                    bool ideal;
                                    if (*argv[i+3]  ==  'I') ideal = true;
                                    if (*argv[i+3]  ==  'C') ideal = false;
                                    
                                    if (!type) {
                                        if (dimX * dimY > 128*128)
                                        cout << "\nUtilizzare l'FT su immagini di modeste dimensioni\n" <<
                                        "puo' risultare enormemente oneroso in termini di tempo\n" <<
                                        "Si consiglia l'uso dell'FFT in questi casi.\n";
                                        
                                        for (int k=0; k<dimX * dimY; k++)
                                            matrix[k] = in->GetBn(k);
                                            
                                        // applica la FT in avanti
                                        FT(matrix, dimX, dimY, matRe, matIm, 1);
                                        
                                        //calcola e salva lo spettro nel file "magnitude.raw"
                                        Magnitude(matRe, matIm, dimX, dimY);
                                            
                                        if (*argv[i+1]  ==  'h') {
                                            HighPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
                                            cout << "\n Applicato filtro: High-Pass filter\n";
                                        }
                                        if (*argv[i+1]  ==  'l') {
                                            LowPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
                                            cout << "\n Applicato filtro: Low-Pass filter\n";
                                        }
                                        // applica la FT indietro
                                        FT(matrix, dimX, dimY, matRe, matIm, -1);
                                        
                                        for (int k=0; k<dimX * dimY; k++)
                                            out->SetBn(k, (int)matrix[k]);
                                    }
                                    else{
                                        for (int c=0; c<3; c++) {
                                            if (dimX * dimY > 64*64)
                                            cout << "\nUtilizzare l'FT su immagini di modeste dimensioni\n" <<
                                            "puo' risultare enormemente oneroso in termini di tempo\n" <<
                                            "Si consiglia l'uso dell'FFT in questi casi.\n";
                                            
                                            for (int k=0; k<dimX * dimY; k++)
                                                matrix[k] = in->GetRgb(k, c);
                                            
                                            // applica la FT in avanti
                                            FT(matrix, dimX, dimY, matRe, matIm, 1);
                                                                                        
                                            if (*argv[i+1]  ==  'h') {
                                                HighPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
                                                cout << "\n Applicato filtro: High-Pass filter\n";
                                            }
                                            if (*argv[i+1]  ==  'l') {
                                                LowPassFilter(atoi(argv[i+2]), matRe, matIm, dimX, dimY, ideal);
                                                cout << "\n Applicato filtro: Low-Pass filter\n";
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
                                    cout << "\n Errore: selezionare filtraggio ideale o a campana\n";
                                    exit(0);
                                }
                            }
                            else {
                                cout << "\n Errore: selezionare il valore di taglio\n";
                                exit(0);
                            }
                        }
                        else {
                            cout << "\n Errore: valore di filtro assente\n";
                            exit(0);
                        }
                        break;
            case 'e' :  Equalize(in, out, type);
                        cout << "\n Applicato filtro: Equalizezione Luminostia'\n";
                        break;
            case 'i' :  Invert(in, out, type);
                        cout << "\n Applicato filtro: Invert\n";
                        break;
            case 'l' :  if (argv[i+1] != 0 && atof(argv[i+1]) >= 0.0f) {
                            Brightness(in, out, atof(argv[i+1]), type); i++;
                            cout << "\n Applicato filtro: Brightness'\n";
                            }
                        else{
                            cout << "\n Errore: valore di filtro assente\n";
                            exit(0);
                        }
                        break;
            case 'a' :  AutoContrast(in, out, type);
                        cout << "\n Applicato filtro: Contrasto Automatico\n";
                        break;
            case 'g' :  if (argv[i+1] != 0 && atof(argv[i+1]) >= 0.0f) {
                            Gamma(in, out, atof(argv[i+1]), type); i++;
                            cout << "\n Applicato filtro: Gamma Correction\n";
                        }
                        else{
                            cout << "\n Errore: valore di filtro assente\n";
                            exit(0);
                        }
                        break;
            case 'h' :  if (type == true) {
                            if (argv[i+1] != 0 && argv[i+2] != 0 && atof(argv[i+1]) >= 0.0f &&
                                ((*argv[i+2]  ==  'R') || (*argv[i+2]  ==  'G') || (*argv[i+2]  ==  'B'))) {
                                Channel(in, out, atof(argv[i+1]), *argv[i+2]); i=i+2;
                                cout << "\n Applicato filtro: Channel\n";
                            }
                            else{
                                cout << "\n Errore: valore di filtro assente\n";
                                exit(0);
                            }
                        }
                        else{
                            cout << "\n Errore: filtro applicabile sono a immagini RGB\n";
                            exit(0);
                        }
                        break;
            case 'm' :  if (argv[i+1] != 0) {
                            if (*argv[i+1]  ==  'X') {
                                Mirror(in, out, 'X', type); i++;
                                cout << "\n Applicato filtro: Mirror orizzontale\n";
                            }
                            else if (*argv[i+1]  ==  'Y') {
                                Mirror(in, out, 'Y', type); i++;
                                cout << "\n Applicato filtro: Mirror verticale\n";
                            }
                            else{
                                cout << "\n Errore: valore di filtro scorretto\n";
                                exit(0);
                            }
                        }
                        else{
                            cout << "\n Errore: valore di filtro assente\n";
                            exit(0);
                        }
                        break;
			case 'p' :  if (argv[i+1] != 0 && atoi(argv[i+1]) > 0) {
                            Pixelize(in, out, atoi(argv[i+1]), type); i++;
                            cout << "\n Applicato filtro: Pixelazer\n";
                        }
                        else{
                            cout << "\n Errore: valore di filtro assente o scorretto\n";
                            exit(0);
                        }
                        break; 
			
            case 's' :  if (argv[i+1] != 0 && atoi(argv[i+1]) > 0 && atoi(argv[i+1])%2  ==  1) {
                            Smooth(in, out, atoi(argv[i+1]), type); i++;
                            cout << "\n Applicato filtro: Smooth\n";
                        }
                        else{
                            cout << "\n Errore: valore di filtro assente o scorretto\n";
                            exit(0);
                        }
                        break;
			case 'r' :  if (argv[i+1] != 0 && atoi(argv[i+1]) > 0 && atoi(argv[i+1])%2  ==  1) {
                            RemoveNoise(in, out, atoi(argv[i+1]), type); i++;
                            cout << "\n Applicato filtro: Togli rumore sale/pepe\n";
                        }
                        else{
                            cout << "\n Errore: valore di filtro assente o scorretto\n";
                            exit(0);
                        }
                        break;
            case 'c' :  if (argv[i+1] != 0) {
                            if (*argv[i+1]  ==  'S') {
                                Convolution(in, out, 'S', type); i++;
                                cout << "\n Applicato filtro: Sharpen\n";
                            }
                            else if (*argv[i+1]  ==  'B') {
                                Convolution(in, out, 'B', type); i++;
                                cout << "\n Applicato filtro: Blur\n";
                            }
                            else if (*argv[i+1]  ==  'O') {
                                Convolution(in, out, 'O', type); i++;
                                cout << "\n Applicato filtro: Outliner\n";
                            }
                            else{
                                cout << "\n Errore: valore di filtro scorretto\n";
                                exit(0);
                            }
                        }
                        else{
                            cout << "\n Errore: valore di filtro assente\n";
                            exit(0);
                        }
                        break;
            case 'q' :  if (argv[i+1] != 0) {
                            Rotation(in, out, atof(argv[i+1]), type); i++;
                            cout << "\n Applicato filtro: Rotate\n";
                        }
                        else{
                            cout << "\n Errore: valore di filtro assente\n";
                            exit(0);
                        }
                        break;
            case 'z' :  if (argv[i+1] != 0 && argv[i+2] != 0) {
                            if (*argv[i+1]  ==  '+') {
                                Zoom(in, out, '+', atoi(argv[i+2]), type); i=i+2;
                                cout << "\n Applicato filtro: Zoom in\n";
                            }
                            else if (*argv[i+1]  ==  '-') {
                                Zoom(in, out, '-', atoi(argv[i+2]), type); i=i+2;
                                cout << "\n Applicato filtro: Zoom out\n";
                            }
                            else{
                                cout << "\n Errore: valore di filtro scorretto\n";
                                exit(0);
                            }
                        }
                        else{
                            cout << "\n Errore: valore di filtro assente\n";
                            exit(0);
                        }
                        break;
            case 'Z' :  if (argv[i+1] != 0) {
                            Resize(in, out, atoi(argv[i+1]), type); i++;
                            cout << "\n Applicato filtro: Resize\n";
                        }
                        else{
                            cout << "\n Errore: valori di filtro errati o scorretti\n";
                            exit(0);
                        }
                        break;
            case 'b' :  if (type == true) {
                            Grayscale(in, out);
                            cout << "\n Applicato filtro: Grayscale\n";
                        }
                        else{
                            cout << "\n Errore: filtro applicabile sono a immagini RGB\n";
                            exit(0);
                        }
                        break;

            default :   cout << "\n Errore: parametro non corretto\n";
                        exit(0);
                        break;
            }
        delete in;
		in = out;
	}
	
	// salvataggio dell'immagine, se il secondo parametro di SalvaImmagine  0 viene salvata
    // in formato raw in B/N, se  1 viene salvata in formato RGB interleaved
	switch(type) {
        case 0 :    out->SalvaImmagine(argv[2], 0);
                    cout << "\nFile salvato: \t" << argv[2] << endl << "Tipo: \t\tBN" << endl;
                    cout << "Dimensioni: \t" << out->getX() << "x" << out->getY() << " pixel\n";
                    cout << "Size: \t\t" << (out->getX() * out->getY()) << " bytes" << endl;
                    break;
        case 1 :    out->SalvaImmagine(argv[2], 1);
                    cout << "\nFile salvato: \t" << argv[2] << endl << "Tipo: \t\tRGB" << endl;
                    cout << "Dimensioni: \t" << out->getX() << "x" << out->getY() << " pixel\n";
                    cout << "Size: \t\t" << (out->getX() * out->getY() * 3) << " bytes" << endl;
                    break;
        default :   cout << "\nErrore: non specificato destinazione BN o RGB (0/1)\n";
	}
    
    delete out;
    delete matRe;
    delete matIm;
    exit(0);
}
